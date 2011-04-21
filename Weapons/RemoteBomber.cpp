/**
 * RemoteBomber. Used as an example for other Explosive Weapon implementations.
 * @author Taylor Arnicar
 * @date 4/10/2011
 */

#include "Weapons/RemoteBomber.h"
#include "Utility/GlobalUtility.h"
#include "Shots/RemoteBombShot.h"
#include "Utility/Point3D.h"
#include "Utility/SoundEffect.h"

double minExplodeTime = 0.5;

RemoteBomber::RemoteBomber(AsteroidShip* owner) : Weapon(owner) {
   shotSpeed = 30.0; // Units per second
   coolDown = 5; // Seconds
   randomVariationAmount = 0.25; // Units
   name = "Remote Bomber";
   lastShotPos = new Point3D(0, 1, 0);
   curAmmo = -1; // Infinite ammo
   purchased = true; // Start off owning the bomber
   lastFiredFrame = 0;
   firedShot = NULL;
}

RemoteBomber::~RemoteBomber() {
   delete lastShotPos;
}

/**
 * Called every frame.
 * We'll probably keep track of something or other here.
 */
void RemoteBomber::update(double timeDiff) {
   if (firedShot != NULL) {
      timeLastFired = doubleTime();
   }
   if (firedShot != NULL && lastFiredFrame <= curFrame - 2 &&
    doubleTime() - firedShot->timeFired > minExplodeTime) {
      firedShot->shouldExplode = true;
      firedShot = NULL;
   }
}

/**
 * This is what actually shoots. Finally!
 */
void RemoteBomber::fire() {
   static Vector3D randomVariation;
   lastFiredFrame = curFrame;

   if ((!isCooledDown()) || firedShot != NULL)
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
   ship->shotDirection.movePoint(start);
   ship->setShakeAmount(0.1f);
   firedShot = new RemoteBombShot(start,
    shotDirection, ship, this, ship->gameState);
   ship->custodian->add(firedShot);
   // Don't play sound effects in godMode b/c there would be too many.
   if (!ship->gameState->godMode) {
      SoundEffect::playSoundEffect("BlasterShot2.wav");
   }
}

/**
 * Basic debug function. Just in case!
 */
void RemoteBomber::debug() {
   printf("%s\n", name.c_str());
}

/**
 * The job of the weapon is to project the position of
 * the targeted object, and return the point that the
 * AI should aim at in order to hit the target with this weapon.
 */
Point3D RemoteBomber::project(Object3D* target) {
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
      // speed of the bomb.
      time = ship->position->distanceFrom(curTarget) / speed;

      // dp is the distance the asteroid traveled in the time it took for our
      // bomb to get to the point we are considering (curTarget).
      dp = target->velocity->scalarMultiply(time);

      // Move our target to the point where the asteroid would be
      curTarget = dp + targetPos;

      // Get the vector that points to that point.
      wouldHit = curTarget - *ship->position;

      // Normalize, scale by speed of the bomb multiplied by the time that
      // passed (for the bomb to get to the previous point) This vector
      // now points to where our bomb will be when the asteroid is at
      // its position

      wouldHit = wouldHit.getNormalized() * speed * time + *ship->position;

      // Dist is the distance from where our bullet will be to where
      // the asteroid will be.
      dist = wouldHit.distanceFrom(curTarget);
      iterations++;
      // If this distance is greater than the radius (ie, we missed),
      // recalculate!
   } while (dist > target->radius && iterations < 5);

   return curTarget;
}

bool RemoteBomber::shouldFire(Point3D* target, Point3D* aim) {
   return ((*target - *ship->position).getNormalized() - *aim).magnitude() < 0.5;
}

