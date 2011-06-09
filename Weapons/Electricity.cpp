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

Electricity::Electricity(AsteroidShip* owner, int _index) : Weapon(owner, _index) {
   ELECTRICITY_WEAPON_INDEX = index;
   shotsFired = 0;
   shotsPerSec = 10;
   coolDown = 0;
   name = "Pikachu's Wrath";
   currentFrame = 1; // Start this 1 ahead of lastFiredFame.
   lastFiredFrame = 0; // We use these three to handle the audio.
   soundPlaying = false;
   curAmmo = -1;
   purchased = false;
   soundHandle = -1;
   
   currentHeat = 0;
   overheatLevel = 5;
   heatPerShot = 0.15;
   overheated = false;

   icon = "PikachusWrathIcon";
   r = 1;
   g = 1;
   b = 1;
}

Electricity::~Electricity() {
   // Do nothing.
}

void Electricity::update(double timeDiff) {
   if (currentHeat > 0) {
      currentHeat = std::max(currentHeat - timeDiff, 0.0);
   }

   if (overheated && currentHeat == 0) {
      overheated = false;
   } else if (currentHeat > overheatLevel) {
      overheated = true;
   }

   if (currentFrame != lastFiredFrame) 
      shotsFired = 0;
   // Stop sound if we're no longer firing.
   if (currentFrame == lastFiredFrame && !soundPlaying) {
      // We should play sound.
      soundPlaying = true;
      soundHandle = SoundEffect::playSoundEffect("ElectricitySound", ship->position, ship == ship->gameState->ship, DEFAULT_VOLUME, true);
   } else if (currentFrame != lastFiredFrame && soundPlaying) {
      SoundEffect::stopSoundEffect(soundHandle);
      soundPlaying = false;
   }
   ++currentFrame;
}

void Electricity::fire() {
   // If it's client mode, wait for the shot packet to arrive, 
   // and then add to the game.
   if (ship->gameState->gsm == ClientMode) {
      return;
   }

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
   
   Point3D start = ship->shotOrigin;
   ship->setShakeAmount(0.1f);
   ship->custodian->add(new ElectricityShot(start, ship->shotDirection, index, ship, shotsToFire, ship->gameState));


   lastFiredFrame = currentFrame;
   shotsFired += shotsToFire;

   if(!ship->gameState->godMode) {
      currentHeat += heatPerShot * shotsToFire;
   }
}

void Electricity::debug() {
   printf("Electricity!\n");
}

Point3D Electricity::project(Object3D* object, Vector3D addOn) {
   return *object->position;
}

bool Electricity::shouldFire(Point3D* target, Point3D* aim) {
   return true;
}

void Electricity::stopSounds() {
   if (soundHandle != -1) {
      SoundEffect::stopSoundEffect(soundHandle);
      soundHandle = -1;
   }
}

/**
 * This is for the weapon bar.
 */
double Electricity::getCoolDownAmount() {
   return 1 - clamp(currentHeat / overheatLevel, 0, 1);
}

/**
 * This is for firing.
 */
bool Electricity::isCooledDown() {
   return Weapon::isCooledDown() && !overheated;
}

