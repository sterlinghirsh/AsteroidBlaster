/**
 * Blaster! The original Asteroids Weapon.
 * @author Sterling Hirsh (shirsh@calpoly.edu)
 * @date 2/14/2011
 * <3
 */

#include "Weapons/Blaster.h"
#include "Graphics/GlutUtility.h"
#include "Shots/ProjectileShot.h"
#include "Utility/Point3D.h"
#include "Utility/Quaternion.h"

Blaster::Blaster(AsteroidShip* owner) 
 : Weapon(owner) {
   shotSpeed = 40; // Units per second
   coolDown = 0.08; // Seconds
   randomVariationAmount = 2; // Units
   name = "Blaster";
   lastShotPos = new Point3D(0, 1, 0);
}

Blaster::~Blaster() {
   free(lastShotPos);
}

/**
 * Called every frame.
 * We'll probably keep track of something or other here.
 */
void Blaster::update(double timeDiff) {
   // Do noting, yet
}

/**
 * This is what actually shoots. Finally!
 */
void Blaster::fire() {
   static Vector3D randomVariation;
   if (!isCooledDown())
      return;
   // Update timeLastFired with new current time.
   timeLastFired = doubleTime();
   // Copy the ship's position for the start point.
   Point3D start = *ship->position;
   // Copy the shot direction, set length to shotSpeed (since shotDirection is unit-length).
   Vector3D shotDirection(ship->shotDirection.scalarMultiply(shotSpeed));
   // Add a random variation to each of the shots.
   randomVariation.randomMagnitude();
   randomVariation.scalarMultiplyUpdate(randomVariationAmount);
   shotDirection.addUpdate(randomVariation);
   ship->shotDirection.movePoint(start);
   gameState->custodian.add(new ProjectileShot(start,
    shotDirection, ship));
}

/**
 * Basic debug function. Just in case!
 */
void Blaster::debug() {
   printf("Blaster!\n");
}

bool Blaster::aimAt(double dt, Object3D* target) {
   Point3D wouldHit;
   double speed = 40;
   double time = 0, dist = 0, ang = 0;
   int iterations = 0;
  
   Point3D aim = ship->shotDirection; 
   Point3D targetPos = *target->position;
   Point3D curTarget = targetPos;
   Point3D dp;
   
   // This section of code does angle interpolation.
   // Find the angle between our vector and where we want to be.
   ang = acos(aim * *lastShotPos);

   // Get our axis of rotation.
   wouldHit = (aim ^ *lastShotPos).normalize();

   // If the difference is more than the radius of the target,
   // we need to adjust where we are aiming.
   if (ang != 0) {
      Quaternion q;
      q.FromAxis(Vector3D(wouldHit.x, wouldHit.y, wouldHit.z), 
       ang < getTurnSpeed() * dt ? ang : getTurnSpeed() * dt);

      aim = q * aim;
      // Normalize the vector.
      aim = aim.normalize();
   }
   
   // This loop will choose the spot that we want to be shooting at.
   do {
      // time is the distance from the ship to the target according to the
      // speed of the bullet.
      time = ship->position->distanceFrom(curTarget) / speed;
      
      // dp is the distance the asteroid traveled in the time it took for our
      // bullet to get to the point we are considering (curTarget).
      dp = target->velocity->scalarMultiply(time);

      // Move our target to the point where the asteroid would be
      curTarget = dp + targetPos;

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
   *lastShotPos = (curTarget - *ship->position).normalize();

   ship->updateShotDirection(aim);

   return fabs((aim - (*lastShotPos)).magnitude()) < 1;//0.05;

}

double Blaster::getTurnSpeed() {
   return 24 * M_PI;
}
