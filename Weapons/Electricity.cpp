/**
 * Tractor Beam
 * This pulls in crystals.
 * @author Sterling Hirsh (shirsh@calpoly.edu)
 * @date Valentine's Day <3
 */

#include "Weapons/Electricity.h"
#include "Utility/GlobalUtility.h"
#include "Shots/ElectricityShot.h"
#include "Utility/SoundEffect.h"

Electricity::Electricity(AsteroidShip* owner) : Weapon(owner) {
   shotsFired = 0;
   shotsPerSec = 30;
   coolDown = 0;
   name = "Pikachu's Wrath";
   currentFrame = 1; // Start this 1 ahead of lastFiredFame.
   lastFiredFrame = 0; // We use these three to handle the audio.
   soundPlaying = false;
   curAmmo = -1;
   purchased = false;

   icon = "PikachusWrathIcon";
   r = 1;
   g = 1;
   b = 1;
}

Electricity::~Electricity() {
   // Do nothing.
}

void Electricity::update(double timeDiff) {
   if (currentFrame != lastFiredFrame) 
      shotsFired = 0;
   // Stop sound if we're no longer firing.
   if (currentFrame == lastFiredFrame && !soundPlaying) {
      // We should play sound.
      soundPlaying = true;
      soundHandle = SoundEffect::playSoundEffect("Pikachu.wav", true);
   } else if (currentFrame != lastFiredFrame && soundPlaying) {
      SoundEffect::stopSoundEffect(soundHandle);
      soundPlaying = false;
   }
   ++currentFrame;
}

void Electricity::fire() {
   if (!isReady())
      return;

   if (shotsFired == 0) {
      timeStartedFiring = doubleTime();
   }
   
   double curTime = doubleTime();
   double timeFired = curTime - timeStartedFiring;
   int shotsToFire = 0;
   //printf("Time fired: %f\n", timeStartedFiring);
   if (timeFired == 0) {
      shotsToFire = 1;
   }
   else {
      while (((shotsFired + shotsToFire) / timeFired) <= shotsPerSec
            /* && ((curAmmo - shotsToFire) > 0)*/) {
         shotsToFire++;
      }
   }
   
   /*
   if(!ship->gameState->godMode && curAmmo <= 0)
      return;
      */

   Point3D start = ship->shotOrigin;
   ship->setShakeAmount(0.1f);
   ship->custodian->add(new ElectricityShot(start, ship->shotDirection, ship, shotsToFire, ship->gameState));
   //std::set<Object3D*>* tempList = gameState->custodian.findCollisions(new ElectricityShot(start, ship->shotDirection, ship));
   lastFiredFrame = currentFrame;
   shotsFired += shotsToFire;
   if(!ship->gameState->godMode) {
      // Take away some ammo
      /*curAmmo -= shotsToFire; */
   }

}

void Electricity::debug() {
   printf("Electricity!\n");
}

Point3D Electricity::project(Object3D* object) {
   return *object->position;
}

bool Electricity::shouldFire(Point3D* target, Point3D* aim) {
   return true;
}
