/**
 * Energy! The original Asteroids Weapon.
 * @author Sterling Hirsh (shirsh@calpoly.edu)
 * @date 2/14/2011
 * <3
 */

#include "Weapons/Energy.h"
#include "Utility/GlobalUtility.h"
#include "Utility/Point3D.h"
#include "Utility/SoundEffect.h"
#include "Shots/EnergyShot.h"

Energy::Energy(AsteroidShip* owner, int _index)
: Weapon(owner, _index) {
   ENERGY_WEAPON_INDEX = index;
   shotSpeed = 80; // Units per second
   coolDown = 0.3; // Seconds
   randomVariationAmount = 1.0; // Units
   name = "Charge Cannon";
   lastShotPos = new Point3D(0, 1, 0);
   curAmmo = -1; // Infinite ammo
   purchased = false;
   lastFiredFrame = 0;
   chargeStartTime = 0;
   chargingShot = NULL;
   soundHandle = -1;
   chargingSoundPlaying = false;
   icon = "ChargeCannonIcon";
   r = 0;
   g = 0.5;
   b = 1;
}

Energy::~Energy() {
   delete lastShotPos;
}

/**
 * Called every frame.
 * We'll probably keep track of something or other here.
 */
void Energy::update(double timeDiff) {
   Weapon::update(timeDiff);
   // Fire when the trigger is released.
   static Vector3D randomVariation;

   // Update the position of the charging shot.
   if (chargingShot != NULL) {
      chargingShot->position->updateMagnitude(ship->shotOrigin);
      chargingShot->updateChargeTime(doubleTime() - chargeStartTime);
      
      if (chargingSoundPlaying && doubleTime() - chargeStartTime > 5.0) {
         SoundEffect::stopSoundEffect(soundHandle);
         soundHandle = SoundEffect::playSoundEffect("ChargeShotLoop", ship->position, ship == ship->gameState->ship, DEFAULT_VOLUME, true);
         chargingSoundPlaying = false;
      }

   } else if (soundHandle != -1) {
      SoundEffect::stopSoundEffect(soundHandle);
      soundHandle = -1;
   }
   
   // If user has stopped charging a shot.
   if (chargingShot != NULL && (lastFiredFrame == curFrame - 2 || ship->isRespawning() )) {
      // Copy the shot direction, set length to shotSpeed (since shotDirection is unit-length).
      Vector3D shotDirection(ship->shotDirection.scalarMultiply(shotSpeed / (1 + std::min(doubleTime() - chargeStartTime, 3.0))));

      // Add a random variation to each of the shots.
      randomVariation.randomMagnitude();
      randomVariation.scalarMultiplyUpdate(randomVariationAmount);
      shotDirection.addUpdate(randomVariation);
      chargingShot->velocity->updateMagnitude(shotDirection);

      // Don't play sound effects in godMode b/c there would be too many.
      SoundEffect::stopSoundEffect(soundHandle);
      soundHandle = -1;
      if (!ship->gameState->godMode) {
         SoundEffect::playSoundEffect("ChargeShotFire", ship->position, ship == ship->gameState->ship);
      }
      
      ship->setShakeAmount((float) std::min((doubleTime() - chargeStartTime) * 0.3, 0.5));
      resetChargingShot();
   }
}

void Energy::resetChargingShot() {
   chargeStartTime = 0;
   chargingShot = NULL;
   // Update timeLastFired with new current time.
   timeLastFired = doubleTime();
}

/**
 * This is what actually shoots. Finally!
 */
void Energy::fire() {
   // If it's client mode, wait for the shot packet to arrive, 
   // and then add to the game.
   if (ship->gameState->gsm == ClientMode) {
      return;
   }

   if (!isReady())
      return;

   lastFiredFrame = curFrame;

   // If we haven't started a shot yet...
   if (chargeStartTime == 0 && chargingShot == NULL) {
      chargeStartTime = doubleTime();  
      Vector3D zeroVelocity(0, 0, 0);
      chargingShot = new EnergyShot(ship->shotOrigin, zeroVelocity, index, ship, this, ship->gameState);
      ship->custodian->add(chargingShot);

      soundHandle = SoundEffect::playSoundEffect("ChargeShotCharge", ship->position);
      chargingSoundPlaying = true;
   }

}

/**
 * Basic debug function. Just in case!
 */
void Energy::debug() {
   printf("Energy!\n");
}

/**
 * The job of the weapon is to project the position of
 * the targeted object, and return the point that the
 * AI should aim at in order to hit the target with this
 * weapon.
 */
Point3D Energy::project(Object3D* target, Vector3D addOn) {
   Point3D wouldHit;
   double time = 0, dist = 0;
   int iterations = 0;

   Point3D targetPos = *target->position;
   targetPos.addUpdate(addOn);
   Point3D curTarget = targetPos;
   Point3D dp;

   // This loop will choose the spot that we want to be shooting at.
   do {
      // time is the distance from the ship to the target according to the
      // speed of the bullet.
      time = ship->position->distanceFrom(curTarget) / shotSpeed;

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

      wouldHit = wouldHit.getNormalized() * shotSpeed * time + *ship->position;

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
bool Energy::shouldFire(Point3D* target, Point3D* aim) {
   return ((*target - ship->shotOrigin).getNormalized() - *aim).magnitude() < 0.5;
}

void Energy::stopSounds() {
   if (soundHandle != -1) {
      SoundEffect::stopSoundEffect(soundHandle);
      soundHandle = -1;
   }
}
