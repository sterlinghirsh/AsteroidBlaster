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
#include <math.h>

#ifdef WIN32
#include "Utility/WindowsMathLib.h"
#endif

// Radians/sec
const double ShootingAI::gunRotSpeed = M_PI / 2.0;

ShootingAI::ShootingAI(AsteroidShip* owner) {
   ship = owner;
   aimingAt = Point3D(0,1,0);
   lastShotPos = Point3D(0,0.9,0);
   // Start the AI as disabled
   enabled = false;
   //TODO possibly more stuff here.
   lastTarget = NULL;
   prevWeapon = NULL;
   
   // These control how fast the AI can switch weapons.
   weaponSwitchSpeed = 2.0;
   weaponSwitchTimer.reset();
   weaponSwitchTimer.setCountDown(weaponSwitchSpeed);
   
   // Set the default chosen weapon.
   chosenWeapon = ship->getCurrentWeapon();
}

double min(double a, double b) {
   return a < b ? a : b;
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
 * @return no idea, really. Just leaving this open in case I think of something
 */
int ShootingAI::aimAt(double dt, Object3D* target) {
   Vector3D wouldHit;
   Vector3D randomVariation;
   double ang = 0;
  
   Point3D aim = lastShotPos;
   Point3D targetPos = chosenWeapon->project(target);
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
   bool shouldFire = chosenWeapon->shouldFire(&targetPos, &aim);

   // Add a random variation to the aim.
   randomVariation.randomMagnitude();
   randomVariation.scalarMultiplyUpdate(chosenWeapon->randomAIVariationAmount);
   aim.addUpdate(randomVariation);
   
   ship->fire(shouldFire);

   return 0;
}

// Choose the appropriate weapon for this situation.
void ShootingAI::chooseWeapon(Object3D** target) {
   if (*target != NULL) {
      if ((dynamic_cast<Shard*>(*target)) != NULL) {
         ship->selectWeapon(TRACTOR_WEAPON_INDEX);
      }
      else if (ship->getWeapon(RAILGUN_WEAPON_INDEX)->purchased && (*target)->radius < 10 && ship->getWeapon(RAILGUN_WEAPON_INDEX)->isCooledDown()
       && ship->getWeapon(RAILGUN_WEAPON_INDEX)->curAmmo > 0) {
         ship->selectWeapon(RAILGUN_WEAPON_INDEX);
      }
      else {
         ship->selectWeapon(BLASTER_WEAPON_INDEX);
      }
   }

   chosenWeapon = ship->getCurrentWeapon();
}

Object3D* ShootingAI::chooseTarget() {
   // Make the AI choose from a list of Targetable objects instead of Drawable objects, which are inside the view frustum.
   std::list<Drawable*>* targets;
   targets = ship->getRadar()->getTargetableViewFrustumReading();
   std::list<Drawable*>::iterator targets_iterator;
   Point3D* ship_position = ship->position;
   Vector3D vec;
   double curWeight = 0.0;
   double maxWeight = -1.0;
   const double distWeight = 600;
   const double radiusWeight = 1;
   const double proximityWeight = 3;
   bool isAShip = false;
   bool isAShard = false;
   
   // If the list of targets from the viewFrustum does not exist, give the AI no target.
   if(targets == NULL) {
      printf("targets list was null.\n");
      return NULL;
   }

   targets_iterator = targets->begin();
   Object3D* closest = NULL;
   AsteroidShip* consideredShip = NULL;

   for ( ; targets_iterator != targets->end(); ++targets_iterator) {
      curWeight = 0.0;
      //printf("starting null check.\n");
      if (*targets_iterator == NULL) {
         //printf("targets_iterator was null!\n\n\n\n");
         continue;
      }
      isAShip = false;
      isAShard = false;

      if (dynamic_cast<Particle*>(*targets_iterator) != NULL) {
        //printf("continuing b/c of a particle!!! \n");
        continue;
     }
      
      if(dynamic_cast<AsteroidShip*>(*targets_iterator) != NULL)
         isAShip = true;

      if(dynamic_cast<Shard*>(*targets_iterator) != NULL)
         isAShard = true;

      if (dynamic_cast<Asteroid3D*>(*targets_iterator) == NULL && dynamic_cast<Shard*>(*targets_iterator) == NULL && dynamic_cast<AsteroidShip*>(*targets_iterator) == NULL) {
         continue;
      }

      if (isAShard && (*targets_iterator)->position->distanceFrom(*ship_position) > 30)
         continue;

      if (isAShip) {
         consideredShip = dynamic_cast<AsteroidShip*> (*targets_iterator);
         /* Don't shoot at myself, or the enemy if it's respawning, or if they just spawned.
          * This is to avoid spawn camping.
          */
         if (consideredShip->id == ship->id || consideredShip->isRespawning() || consideredShip->getAliveTime() < 3)
            continue;
      }

      //printf("curWeight started at %f\n", curWeight);
      /*curWeight = radiusWeight / ((*targets_iterator)->radius);
      vec = (*(*targets_iterator)->position - *ship->position);
      curWeight += distWeight / (vec * vec);
      vec.normalize();
      curWeight += vec * lastShotPos * proximityWeight;
      */

      // Larger objects should have a higher priority.
      curWeight += 2*((*targets_iterator)->radius);

      // Objects that are closer should be weighted higher.
      vec = (*(*targets_iterator)->position - *ship->position);
      // Make sure vec is positive, so that we don't get a negative curWeight.
      double tmp = vec * vec;
      if (tmp < 0.0)
         tmp *= -1;
      curWeight += distWeight / (tmp);
      //curWeight += vec * lastShotPos * proximityWeight;
      
      // Maybe add to the weight if the target is an AsteroidShip.
      if (isAShip) {
          consideredShip = dynamic_cast<AsteroidShip*> (*targets_iterator);

         // Only add weight if the considered ship is not our own ship.
         if (consideredShip->id != ship->id) {
            curWeight += 10;
            //printf("added 10 b/c target is a ship.\n", curWeight);
         }
      }
      
      //printf("curweight summed up to %f\n", curWeight);

      if (isAShard)
         curWeight *= ship->getWeapon(2)->curAmmo != 0;
      
      if (maxWeight < 0 || curWeight > maxWeight) {
         maxWeight = curWeight;
         closest = dynamic_cast<Object3D*> (*targets_iterator);
      }
   }

   delete targets;
   return closest;
}

int ShootingAI::think(double dt) {
   if(!enabled) {
      return 0;
   }
   
   // TODO this is where stuff goes! Should probably figure out the order of
   // the functions.

   /* Also, do we want to try and rechoose a target every think (ie frame)?
    * If not, it would make the shooter a little quicker, but may end up
    * shooting at bad targets (if the gun is pointed in the opposite direction,
    * it will take time to rotate it around to shoot the target, then
    * time to kill the asteroid, and throughout all this time a new asteroid
    * could have become a better target, or even a critical target)
    *
    * Just an idea, we will talk it over.
    */

   /* This order is tentative. We will probably change it. */
   
   Object3D* target = chooseTarget();

   if (target == NULL && target != lastTarget) {
      //printf("Could not find a target!\n");
      lastTarget = target;
   }
   else if (target != lastTarget) {
      //printf("Switching targets..\n");
      if (!lastTarget) {
         //printf("No previous target.\n");
      }
      //printf("New target at: ");
      //target->position->print();
      lastTarget = target;
   }

   // If it's been more than weaponSwitchSpeed seconds, we can switch weapons.
   if (weaponSwitchTimer.isRunning && weaponSwitchTimer.getTimeLeft() <= 0) {
      prevWeapon = ship->getCurrentWeapon();
      chooseWeapon(&target);
      // If we chose a new weapon, reset the timer so we can't switch again soon.
      if (prevWeapon != chosenWeapon) {
         //weaponSwitchTimer.setCountDown(weaponSwitchSpeed);
         weaponSwitchTimer.restartCountDown();
         //printf(" it was a new weapon, so we reset the timer.\n");
      }
   }

   if (target != NULL) {
      target->setTargeted(true);
      aimAt(dt, target);
   }
   else {
      ship->fire(false);
   }

   // Think has a return value just in case it needs to.
   return 0;
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
