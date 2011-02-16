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

// Radians/sec
const double ShootingAI::gunRotSpeed = 12 * 3.14159265;

ShootingAI::ShootingAI(AsteroidShip* owner) {
   ship = owner;
   aimingAt = Point3D(0,1,0);
   lastShotPos = Point3D(0,0.9,0);
   // Start the AI as disabled
   enabled = false;
   //TODO possibly more stuff here.
   lastTarget == NULL;
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
 * TODO: Make this work. I (Sterling) was getting huge values for wouldHit.
 * This is almost certainly a bug.
 */
int ShootingAI::aimAt(double dt, Object3D* target) {
   Point3D wouldHit;
   double speed = 20;//chosenWeapon->getSpeed();
   double time = 0, dist = 0, ang = 0;
   int iterations = 0;
   // Change in position
   Vector3D dp;
   
   Point3D targetPos = *target->position;
   Point3D curTarget = targetPos;
   // This section of code does angle interpolation.
   // Find the angle between our vector and where we want to be.
   ang = acos(aimingAt * lastShotPos);

   // Get our axis of rotation.
   wouldHit = (aimingAt ^ lastShotPos).normalize();

   // If the difference is more than the radius of the target,
   // we need to adjust where we are aiming.
   if (fabs(fabs(ang) - gunRotSpeed*dt) > 0.01) {
      Quaternion q;
      q.FromAxis(Vector3D(wouldHit.x, wouldHit.y, wouldHit.z), gunRotSpeed*dt);

      aimingAt = q * aimingAt;
      // Normalize the vector.
      aimingAt = aimingAt.normalize();
      //ship->fire(false);
   }
   //else
      ship->fire(true);
   
   // This loop will choose the spot that we want to be shooting at.
   do {
      // time is the distance from the ship to the target according to the
      // speed of the bullet.
      time = ship->position->distanceFrom(curTarget) / speed;
      
      // dp is the distance the asteroid traveled in the time it took for our
      // bullet to get to the point we are considering (curTarget).
      dp = target->velocity->scalarMultiply(time);

      // Move our target to the point where the asteroid would be
      curTarget = Point3D(targetPos.x + dp.xMag, targetPos.y + dp.yMag,
       targetPos.z + dp.zMag);

      // Get the vector that points to that point.
      wouldHit = curTarget - *ship->position;

      // Normalize, scale by speed of the bullet multiplied by the time that
      // passed (for the bullet to get to the previous point) This vector
      // now points to where our bullet will be when the asteroid is at
      // its position

      wouldHit = wouldHit.normalize() * speed * time + *ship->position;

      // Dist is the distance from where our bullet will be to where
      // the asteroid will be.
      dist = wouldHit.distanceFrom(curTarget);
      iterations++;
      // If this distance is greater than the radius (ie, we missed),
      // recalculate!
   } while (dist > target->radius && iterations < 30);

   // By the end of the loop, curTarget is the point that we need to aim
   // at in order to hit our target.
   lastShotPos = (curTarget - *ship->position).normalize();
   ship->updateShotDirection(aimingAt);

   return 0;
}

// kinda useless right now.
void ShootingAI::chooseWeapon( int weapon ) {
    ship->selectWeapon( weapon );
    //chosenWeapon = ship->getWeapon( weapon );
}

Object3D* ShootingAI::chooseTarget() {
   std::list<Object3D*>* targets = ship->getRadar()->getNearbyReading();
   std::list<Object3D*>::iterator targets_iterator;
   Point3D* ship_position = ship->position;
   double curDist, shortestDist = -1;
   
   targets_iterator = targets->begin();
   Object3D* closest = NULL;
   for ( ; targets_iterator != targets->end(); targets_iterator++) {
      // get closest asteroid
      //if (*targets_iterator == NULL || *targets_iterator == ship || NULL == (asteroid = dynamic_cast<Asteroid3D*>(*targets_iterator)) || asteroid->isShard  )
      // Trying this with shard subclass
      // IF THE AI BREAKS, REMOVE THIS
      if (*targets_iterator == NULL ||
       dynamic_cast<Asteroid3D*>(*targets_iterator) == NULL) {
      // END OF NEW CODE
         continue;
      }
      
      curDist = (*targets_iterator)->position->distanceFrom( *ship_position );
      if (shortestDist < 0 || curDist < shortestDist) {
         shortestDist = curDist;
         closest = *targets_iterator;
      } 
   }

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
   
   // choose weapon railgun
   chooseWeapon(0);

   Object3D* target = chooseTarget();
   if (target == NULL && target != lastTarget) {
      printf("Could not find a target!\n");
      lastTarget = target;
   }
   else if (target != lastTarget) {
      printf("Switching targets..\n");
      if (!lastTarget) {
         printf("No previous target.\n");
      }
      printf("New target at: ");
      target->position->print();
      lastTarget = target;
   }
   // choose target

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
