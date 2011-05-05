/**
 * Blaster! The original Asteroids Weapon.
 * @author Sterling Hirsh (shirsh@calpoly.edu)
 * @date 2/14/2011
 * <3
 */

#include "Weapons/Blaster.h"
#include "Utility/GlobalUtility.h"
#include "Shots/ProjectileShot.h"
#include "Utility/Point3D.h"
#include "Utility/SoundEffect.h"

Blaster::Blaster(AsteroidShip* owner)
: Weapon(owner) {
   shotSpeed = 40; // Units per second
   coolDown = 0.15; // Seconds
   randomVariationAmount = 1.5; // Units
   name = "Blaster";
   lastShotPos = new Point3D(0, 1, 0);
   curAmmo = -1; // Infinite ammo
   purchased = true; // Start off owning the blaster
   icon = "BlasterIcon";
   r = 1;
   g = 0;
   b = 0;
}

Blaster::~Blaster() {
   delete lastShotPos;
}

/**
 * Called every frame.
 * We'll probably keep track of something or other here.
 */
void Blaster::update(double timeDiff) {
   // Do nothing, yet
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
   Point3D start = ship->shotOrigin;
   // Copy the shot direction, set length to shotSpeed (since shotDirection is unit-length).
   Vector3D shotDirection(ship->shotDirection.scalarMultiply(shotSpeed));
   // Add a random variation to each of the shots.
   randomVariation.randomMagnitude();
   randomVariation.scalarMultiplyUpdate(randomVariationAmount);
   shotDirection.addUpdate(randomVariation);
   ship->shotDirection.movePoint(start, 3);
   ship->setShakeAmount(0.05f);
   ship->custodian->add(new ProjectileShot(start,
            shotDirection, ship, ship->gameState));
   // Don't play sound effects in godMode b/c there would be too many.
   if (!ship->gameState->godMode) {
      SoundEffect::playSoundEffect("BlasterShot2.wav");
   }
}

/**
 * Basic debug function. Just in case!
 */
void Blaster::debug() {
   printf("Blaster!\n");
}

/**
 * The job of the weapon is to project the position of
 * the targeted object, and return the point that the
 * AI should aim at in order to hit the target with this
 * weapon.
 */
Point3D Blaster::project(Object3D* target) {
   Point3D wouldHit;
   double speed = 40;
   double time = 0, dist = 0;
   int iterations = 0;

   Point3D targetPos = *target->position;
   Point3D curTarget = targetPos;
   Point3D dp;

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
      wouldHit.normalize();
      wouldHit = wouldHit * speed * time + *ship->position;

      // Dist is the distance from where our bullet will be to where
      // the asteroid will be.
      dist = wouldHit.distanceFrom(curTarget);
      iterations++;
      // If this distance is greater than the radius (ie, we missed),
      // recalculate!
   } while (dist > target->radius && iterations < 5);

   return curTarget;
}

/**
 * Tell the Shooting AI whether or not it should fire at the target given the
 * target and where it's aiming.
 * This leads to the AI leading it shots.
 */
bool Blaster::shouldFire(Point3D* target, Point3D* aim) {
   Vector3D targetToShip = *target - ship->shotOrigin;
   targetToShip.normalize();
   return (targetToShip - *aim).magnitude() < 0.5;
}

