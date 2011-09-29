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
#include "Text/GameMessage.h"

Electricity::Electricity(AsteroidShip* owner, int _index) : Weapon(owner, _index) {
   ELECTRICITY_WEAPON_INDEX = index;
   shotsFired = 0;
   shotsPerSec = 10;
   coolDown = 0;
   name = "Electricity Gun";
   currentFrame = 1; // Start this 1 ahead of lastFiredFame.
   lastFiredFrame = 0; // We use these three to handle the audio.
   soundPlaying = false;
   curAmmo = -1;
   purchased = false;
   soundHandle = NULL;
   
   overheatLevel = 5;
   heatPerShot = 0.15;

   icon = "PikachusWrathIcon";
   r = 1;
   g = 1;
   b = 1;
}

Electricity::~Electricity() {
   // Do nothing.
}

void Electricity::update(double timeDiff) {
   Weapon::update(timeDiff);

   if (currentFrame != lastFiredFrame) 
      shotsFired = 0;
   // Stop sound if we're no longer firing.
   if (currentFrame == lastFiredFrame && !soundPlaying) {
      // We should play sound.
      soundPlaying = true;
      soundHandle = SoundEffect::playSoundEffect("ElectricitySound", ship->position, ship->velocity, ship == ship->gameState->ship, 0.5f, true);
   } else if (currentFrame != lastFiredFrame && soundPlaying) {
      SoundEffect::stopSoundEffect(soundHandle);
      soundPlaying = false;
   }
   ++currentFrame;

   if (isOverheated() && this->ship == ship->gameState->ship)
      GameMessage::Add("Electricity overheated!", 30, 0);
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

   addHeat(heatPerShot * shotsToFire);
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
   if (soundHandle != NULL) {
      SoundEffect::stopSoundEffect(soundHandle);
      soundHandle = NULL;
      soundPlaying = false;
   }
}

/**
 * This is for the weapon bar.
 */
double Electricity::getCoolDownAmount() {
   return 1 - clamp(currentHeat / overheatLevel, 0, 1);
}
