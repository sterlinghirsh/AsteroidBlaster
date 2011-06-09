/**
 * @file
 * The Shooting AI class implementation.
 *
 * <pre>
 * The Shooting AI is in charge of controlling the ships weapons independently
 * of the flyer.
 * The Shooting AI shall:
 *    * Acquire targets quickly
 *    * Use information about targets to choose the best target.
 *    * Destroy targets
 *    * Help the ship stay alive
 *    * Keep the game interesting by simulating human inaccuracies.
 *
 * @author Mike Smith, Taylor Arnicar, Sean Ghiocel, Justin Kuehn
 */

#include "AI/ShootingAI.h"
#include "Items/Asteroid3D.h"
#include "Utility/Quaternion.h"
#include "Weapons/Weapon.h"
#include "Utility/GlobalUtility.h"
#include "Particles/Particle.h"
#include <math.h>
#include <time.h> // For random # generation

#ifdef WIN32
#include "Utility/WindowsMathLib.h"
#endif

// Radians/sec
const double ShootingAI::gunRotSpeed = M_PI / 1.75;

ShootingAI::ShootingAI(AsteroidShip* owner) {
   ship = owner;
   aimingAt = Point3D(0,1,0);
   lastShotPos = Point3D(0,0.9,0);
   target = NULL;
   prevWeapon = NULL;
   targetID = -1;
   // Start the AI as disabled
   enabled = false;
   needToChooseTarget = false;
   
   // These control how fast the AI can switch weapons (in seconds).
   weaponSwitchSpeed = 2.0;
   weaponSwitchTimer.reset();
   weaponSwitchTimer.setCountDown(weaponSwitchSpeed);

   // These control how fast the AI can switch targets (in seconds).
   targetSwitchSpeed = 0.25;
   targetSwitchTimer.reset();
   targetSwitchTimer.setCountDown(targetSwitchSpeed);
   
   // Set the default chosen weapon.
   chosenWeapon = ship->getCurrentWeapon();
   targetIsAShard = false;

   maxDifficulty = 10;

   // Make the difficulty a random int from 1 - maxDifficulty.
   difficulty = rand() % maxDifficulty + 1;
}

/**
 * Deconstructor - cleanup.
 */
ShootingAI::~ShootingAI() {
}

/** Aims at an object.
 *
 * This function should be called multiple frames consecutively in order
 * to actually aim at the object. The gun barrel has a rotation time,
 * and this function takes care of moving the virtual barrel to where it
 * needs to be. This will take care of predicting where the target will
 * be based on its current velocity.
 *
 * @param dt time difference since last frame, for gun turning.
 * @return Whether or not the ship is ready to fire.
 */
bool ShootingAI::aimAt(double dt, Object3D* target) {
   Vector3D wouldHit;
   Vector3D randomVariation;
   double ang = 0;
  
   //------------------------------
   // Add a random variation to the aim.
   // randomVariation is between <-1, -1, -1> and <1, 1, 1>
   randomVariation.randomMagnitude();

   /* maxDifficulty - difficulty ensures that high difficulty means very 
    * little is added onto the randomVariation. Low difficulty means that
    * a large number is added onto the randomVariation.
    * The accuracy scales exponentially. Even a max difficulty AI will have
    * some small randomIAVariationAmount per weapon.
    */
   //randomVariation.scalarMultiplyUpdate(chosenWeapon->randomAIVariationAmount + (maxDifficulty - difficulty)/((float)difficulty / 2));
   randomVariation.scalarMultiplyUpdate(chosenWeapon->randomAIVariationAmount + ((maxDifficulty - difficulty)*(maxDifficulty - difficulty)/15));
   //aim.addUpdate(randomVariation);
   //-----------------------------
    
   Point3D aim = lastShotPos;
   Point3D targetPos = chosenWeapon->project(target, randomVariation);
   Vector3D targetDir = (targetPos - ship->shotOrigin).getNormalized();


   // This section of code does angle interpolation.
   // Find the angle between our vector and where we want to be.
   ang = acos(aim * targetDir);

   // Get our axis of rotation.
   wouldHit = (aim ^ targetDir);
   wouldHit.normalize();

   if (ang > (gunRotSpeed * dt)) {
      Quaternion q;
      q.FromAxis(wouldHit, gunRotSpeed * dt);

      aim = q * aim;
      // Normalize the vector.
      aim.normalize();
   }
   else {
      aim = targetDir;
   }


   // The aiming should be clamped within a radius
   //double forwardDotAim = aim.dot(*ship->forward);
   double forwardDotAim = targetDir.dot(*ship->forward);
   double upDotAim = aim.dot(*ship->up);
   double rightDotAim = aim.dot(*ship->right);
   // (Root 2)/2 radians
   const double minForwardAimAmount = 0.707;

   ship->updateShotDirection(aim);
   lastShotPos = aim;

   bool shouldFire = chosenWeapon->shouldFire(&targetPos, &aim);

   
   /* If the cursor is going outside of the allowable range, choose a new target.
    * Make sure we don't fire.
    */
   if(forwardDotAim < minForwardAimAmount) {
      needToChooseTarget = true;
      shouldFire = false;
   }
   return shouldFire;
}

/**
 * Called when the AI doesn't have a target. In this case, it should just lock
 * the cursor to the middle of the screen.
 */
void ShootingAI::aimCursorAtMiddle(double dt) {
   Vector3D wouldHit;
   double ang = 0;
   Point3D aim = lastShotPos;

   // This target position is just in front of the ship.
   Point3D targetPos = ship->shotOrigin;

   /* Move the target position a bit in the forward direction to make sure
    * that it's in front of the ship, not behind it.
    */
   ship->forward->movePoint(targetPos);

   Vector3D targetDir = (targetPos - ship->shotOrigin).getNormalized();

   // This section of code does angle interpolation.
   // Find the angle between our vector and where we want to be.
   ang = acos(aim * targetDir);

   // Get our axis of rotation.
   wouldHit = (aim ^ targetDir);
   wouldHit.normalize();

   if (ang > (gunRotSpeed * dt)) {
      Quaternion q;
      q.FromAxis(wouldHit, gunRotSpeed * dt);

      aim = q * aim;
      // Normalize the vector.
      aim.normalize();
   }
   else {
      aim = targetDir;
   }
   
   ship->updateShotDirection(aim);
   lastShotPos = aim;
}

// Choose the appropriate weapon for this situation.
void ShootingAI::chooseWeapon(Object3D* target) {
   Vector3D vec;
   double curWeight = 0.0;
   double maxWeight = -1.0;
   /* Stores the weight of each weapon. At the end of the function, the weapon
    * with the highest weight is chosen.
   */
   
   double weaponWeights[NUMBER_OF_WEAPONS];

   // States whether or not the chosen target is a ship
   bool isAShip = false;

   // Only consider any weapon if it's purchased & has ammo & is cooled down.
   bool considerTractor = (ship->weapons[TRACTOR_WEAPON_INDEX]->purchased && (ship->getWeapon(TRACTOR_WEAPON_INDEX)->curAmmo != 0));
   bool considerBlaster = (ship->weapons[BLASTER_WEAPON_INDEX]->purchased && (ship->getWeapon(BLASTER_WEAPON_INDEX)->curAmmo != 0));
   bool considerRailgun = (ship->weapons[RAILGUN_WEAPON_INDEX]->purchased && (ship->getWeapon(RAILGUN_WEAPON_INDEX)->curAmmo != 0) && ship->getWeapon(RAILGUN_WEAPON_INDEX)->isCooledDown());
   bool considerElectricity = (ship->weapons[ELECTRICITY_WEAPON_INDEX]->purchased && (ship->getWeapon(ELECTRICITY_WEAPON_INDEX)->curAmmo != 0));
   bool considerTimedBomber = (ship->weapons[TIMEDBOMBER_WEAPON_INDEX]->purchased && (ship->getWeapon(TIMEDBOMBER_WEAPON_INDEX)->curAmmo != 0));
   bool considerEnergy = (ship->weapons[ENERGY_WEAPON_INDEX]->purchased && (ship->getWeapon(ENERGY_WEAPON_INDEX)->curAmmo != 0));

   // Zero out the list of weapon weights so we can build them up.
   for(int x = 0; x < NUMBER_OF_WEAPONS; x++)
      weaponWeights[x] = 0.0;

   /*
   printf("ConsiderTractor is %s\n", (considerTractor)? "true":"false");
   printf("ConsiderBlaster is %s\n", (considerBlaster)? "true":"false");
   printf("ConsiderRailgun is %s\n", (considerRailgun)? "true":"false");
   printf("ConsiderElectricity %s\n", (considerElectricity)? "true":"false");
   printf("ConsiderTimedBomber is %s\n", (considerTimedBomber)? "true":"false");
   printf("ConsiderRemoteBomber is %s\n", (considerRemoteBomber)? "true":"false");
   printf("ConsiderEnergy is %s\n", (considerEnergy)? "true":"false");
   */

   if (dynamic_cast<AsteroidShip*>(target) != NULL)
      isAShip = true;

   // If the target is a shard, only ever choose the tractor beam.
   if (considerTractor && dynamic_cast<Shard*>(target) != NULL) {
      selectWeaponUpdateChosen(TRACTOR_WEAPON_INDEX);
      return;
   }

   // Objects that are closer should be weighted higher.
   // dist will be used for each distance weighting calculation.
   vec = (*(target)->position - *ship->position);
   // Make sure dist is positive, so that we don't get a negative weight.
   double dist = vec * vec;
   if (dist < 0.0)
     dist *= -1;

   /* We would consider the tractor beam here, but it's already definitively
    * chosen up above, if the target is a shard.
    */

   // Only add up weights for the blaster if we're considering it.
   if (considerBlaster) {
      // Consider the target's radius.
      weaponWeights[BLASTER_WEAPON_INDEX] += 1.75 * target->radius;

      // Consider the target's distance.
      weaponWeights[BLASTER_WEAPON_INDEX] += 1.01 * dist;

      // Consider the target's velocity. Slower is better.
      weaponWeights[BLASTER_WEAPON_INDEX] += 10 / target->velocity->magnitude();
   }

   // Only add up weights for the railgun if we're considering it.
   if (considerRailgun) {
      // Consider the target's radius. Smaller is much better.
      weaponWeights[RAILGUN_WEAPON_INDEX] += 12 / target->radius;

      // Consider the target's distance. Further is much better.
      weaponWeights[RAILGUN_WEAPON_INDEX] += 1.01 * dist;

      // Consider the target's velocity. Faster is much better.
      weaponWeights[RAILGUN_WEAPON_INDEX] += 0.95 * target->velocity->magnitude();
   }

   // Only add up weights for the electricity gun if we're considering it.
   if (considerElectricity) {
      // Consider the target's radius.
      weaponWeights[ELECTRICITY_WEAPON_INDEX] += 100 / target->radius;

      // Consider the target's distance.
      weaponWeights[ELECTRICITY_WEAPON_INDEX] += 1.5 * dist;

      // Consider the target's velocity. Faster is much better.
      weaponWeights[ELECTRICITY_WEAPON_INDEX] += target->velocity->magnitude();
   }

   // Only add up weights for the timed bomber if we're considering it.
   if (considerTimedBomber) {
      // Consider the target's radius. Bigger is better.
      weaponWeights[TIMEDBOMBER_WEAPON_INDEX] += 2 * target->radius;

      // Consider the target's distance.
      weaponWeights[TIMEDBOMBER_WEAPON_INDEX] +=  1.75 * dist;

      // Consider the target's velocity. Slower is much better.
      weaponWeights[TIMEDBOMBER_WEAPON_INDEX] += 200 / target->velocity->magnitude();

      // The harder the AI is, the less it should use the timed bomber.
      weaponWeights[TIMEDBOMBER_WEAPON_INDEX] -= 3 * (maxDifficulty - difficulty);
   }

   // Only add up weights for the energy weapon if we're considering it.
   if (considerEnergy) {
      // Consider the target's radius.
      weaponWeights[ENERGY_WEAPON_INDEX] += 1.4 * target->radius;

      // Consider the target's distance.
      weaponWeights[ENERGY_WEAPON_INDEX] += dist;

      // Consider the target's velocity. Slower is better.
      weaponWeights[ENERGY_WEAPON_INDEX] += 200 / target->velocity->magnitude();
   }
   
   for(int i = 0; i < NUMBER_OF_WEAPONS; i++)
      //printf("weapons[%d] was weighted as: %f\n", i, weaponWeights[i]);

   // Select the weapon index which has the highest weight.
   selectWeaponUpdateChosen(maxValuedIndexInArray(weaponWeights, NUMBER_OF_WEAPONS));

   int ndx =  maxValuedIndexInArray(weaponWeights, NUMBER_OF_WEAPONS);

   //printf("Chose %d, with weight %f\n", ndx, weaponWeights[ndx]);

}

// Helper function, reused by chooseWeapon.
void ShootingAI::selectWeaponUpdateChosen(int weaponIndex) {
   ship->selectWeapon(weaponIndex);
   chosenWeapon = ship->getCurrentWeapon();
}

Object3D* ShootingAI::chooseTarget() {
   /* Make the AI choose from a list of Targetable objects instead of Drawable 
    * objects, which are inside the view frustum.
    */
   std::list<Object3D*>* targets;
   targets = ship->getRadar()->getTargetableViewFrustumReading();
   std::list<Object3D*>::iterator targets_iterator;
   Point3D* ship_position = ship->position;
   Vector3D vec;
   double curWeight = 0.0;
   double maxWeight = -1.0;
   const double distWeight = 600;
   const double radiusWeight = 2.2;
   const double proximityWeight = 2;
   /* The default weighting multiplier for an enemy ship. This is increased
    * if the target's kill to death ratio is high, and decreased if it is low.
    * It's also decreased if this AI's KTD ratio is high, and increased if it's low.
    */
   double playerWeight = 1;
   bool isAShip = false;
   bool isAShard = false;
   
   // printf("size of targets list is: %d\n", targets->size());
   // If the list of targets from the viewFrustum does not exist, give the AI no target.
   if(targets == NULL) {
      printf("targets list was null.\n");
      return NULL;
   }

   targets_iterator = targets->begin();
   Object3D* closest = NULL;
   AsteroidShip* consideredShip = NULL;
   Object3D* thisObj = NULL;

   for ( ; targets_iterator != targets->end(); ++targets_iterator) {
      //printf("starting null check.\n");
      if (*targets_iterator == NULL) {
         //printf("targets_iterator was null!\n\n\n\n");
         continue;
      }

      // Don't consider any target that's too far away from the ship's forward vector
      Vector3D targetDir = (*(*targets_iterator)->position - ship->shotOrigin).getNormalized();

      double forwardDotAim = targetDir.dot(*ship->forward);
      const double minForwardAimAmount = 0.707;

      if(forwardDotAim < minForwardAimAmount) {
         continue;
      }
      
      
      curWeight = 0.0;
      isAShip = false;
      isAShard = false;

      if((consideredShip = dynamic_cast<AsteroidShip*>(*targets_iterator)) != NULL) {
         isAShip = true;
         // Reset this to 1 for this target.
         playerWeight = 1;
         // Make sure we don't get a divide by 0 error.
         if(consideredShip->deaths != 0) {
            playerWeight = consideredShip->kills / consideredShip->deaths;
            if(playerWeight = 0)
               playerWeight = 1;
            // Don't ever add too much of a weight for a player, even if their KTD ratio is high.
            if(playerWeight > 1.6)
               playerWeight = 1.6;
            // Don't ever add too small of a weight for a player, even if they're playing poorly.
            else if(playerWeight < 0.3)
               playerWeight = 0.3;
         }
     }

     else if(dynamic_cast<Shard*>(*targets_iterator) != NULL) {
        // Shards are more important if the ship is low on health.
        if (ship->health < 50)
           curWeight += 15;
        /* If we don't have many unbanked shards, we want to provide some 
         * buffer room. Picking up more shards is important.
         */
        if (ship->unbankedShards <= 3)
           curWeight += 35;
        /* Otherwise, we've got a solid buffer of unbanked shards waiting 
         * to be banked, so other targets are higher priority.
         */
        else
           curWeight += 10;
        isAShard = true;
     }

     // If the target is not an asteroid, shard, or ship, skip it.
     if (dynamic_cast<Asteroid3D*>(*targets_iterator) == NULL && !isAShard && !isAShip) {
         continue;
      }

     // If it's a shard and it's further than 30 units away, skip it.
     if (isAShard && (*targets_iterator)->position->distanceFrom(*ship_position) > ship->weapons[TRACTOR_WEAPON_INDEX]->range)
        continue;

     if (isAShip) {
        /* Don't shoot at myself, or the enemy if it's respawning, or if they just spawned.
         * This is to avoid spawn camping.
         */
        if (consideredShip->id == ship->id || consideredShip->isRespawning() || consideredShip->getAliveTime() < 3) {
           continue;
        }

        // Only add weight if the considered ship is not our own ship.
        if (consideredShip->id != ship->id) {
           curWeight += playerWeight * 20;
        }
     }

     //printf("curWeight started at %f\n", curWeight);
     /*curWeight = radiusWeight / ((*targets_iterator)->radius);
     vec = (*(*targets_iterator)->position - *ship->position);
     curWeight += distWeight / (vec * vec);
     vec.normalize();
     curWeight += vec * lastShotPos * proximityWeight;
     */

     // Larger objects should have a higher priority.
     curWeight += radiusWeight *((*targets_iterator)->radius);

     // Objects that are closer should be weighted higher.
     vec = (*(*targets_iterator)->position - *ship->position);
     // Make sure vec is positive, so that we don't get a negative curWeight.
     double tmp = vec * vec;
     if (tmp < 0.0)
        tmp *= -1;
     curWeight += distWeight / (tmp);
     curWeight += vec * lastShotPos * proximityWeight;
      
     //printf("curweight summed up to %f\n", curWeight);

     if (isAShard)
        curWeight *= ship->getWeapon(TRACTOR_WEAPON_INDEX)->curAmmo != 0;
      
     if (maxWeight < 0 || curWeight > maxWeight) {
        maxWeight = curWeight;
        closest = dynamic_cast<Object3D*> (*targets_iterator);
     }
   }

   delete targets;
   
   // No need to choose a new target any more
   needToChooseTarget = false;

   return closest;
}

void ShootingAI::think(double dt) {
   // Whether or not the ship should fire.
   bool shouldFire = false;

   // If the ShootingAI isn't enabled, or there's no gamestate, skip shooting.
   if(!enabled || ship->gameState == NULL) {
      return;
   }
   
   /* Also, do we want to try and rechoose a target every think (ie frame)?
    * If not, it would make the shooter a little quicker, but may end up
    * shooting at bad targets (if the gun is pointed in the opposite direction,
    * it will take time to rotate it around to shoot the target, then
    * time to kill the asteroid, and throughout all this time a new asteroid
    * could have become a better target, or even a critical target)
    *
    * Just an idea, we will talk it over.
    */
   if (targetID != -1) {
      target = ship->gameState->custodian[targetID];
   }
   
   /* If there is no target, or we need to choose a target b/c the old one
    * went off screen, or if the timer is up and we're allowed to switch
    * targets.
    */
   if (target == NULL || needToChooseTarget || (targetSwitchTimer.isRunning && targetSwitchTimer.getTimeLeft() <= 0)) {
      target = chooseTarget();
      // If there was nothing on screen, target could have wound up NULL
      if (target != NULL) {
         targetID = target->id;
         targetSwitchTimer.restartCountDown();
         // If the chosen target is a Shard, remember that till the next time we choose a target.
         if (dynamic_cast<Shard*>(target) != NULL)
            targetIsAShard = true;
         else
            targetIsAShard = false;
      }
   }

   // If it's been more than weaponSwitchSpeed seconds, we can switch weapons.
   // Don't bother choosing a weapon if target is NULL
   if (target != NULL && weaponSwitchTimer.isRunning && weaponSwitchTimer.getTimeLeft() <= 0) {
      prevWeapon = ship->getCurrentWeapon();
      chooseWeapon(target);
      // If we chose a new weapon, reset the timer so we can't switch again soon.
      if (prevWeapon != chosenWeapon) {
         weaponSwitchTimer.restartCountDown();
      }
   }

   // If we've got an actual target
   if (target != NULL) {
      // aimAt aims the cursos and returns whether or not the ship is ready to fire.
      shouldFire = aimAt(dt, target);
      // Make sure we don't fire incorrect weapons at shards.
      if (targetIsAShard) {
         // If the current weapon is not a tractor beam.
         if (ship->currentWeapon != TRACTOR_WEAPON_INDEX) {
            // Don't fire.
            shouldFire = false;
         }
      }
      ship->fire(shouldFire);
   }
   // Otherwise we've got no target
   else {
      // Aim the cursor in front of the ship
      aimCursorAtMiddle(dt);
      // Make sure we stop firing.
      ship->fire(false);
   }
}

void ShootingAI :: enable() {
   enabled = true;
}

void ShootingAI :: disable() {
   enabled = false;
   ship->fire(false);
}

bool ShootingAI::isEnabled() {
   return enabled;
}
