/**
 * Lawn Mower
 * This knocks around Asteroids like a bat.
 * @author Sterling Hirsh (shirsh@calpoly.edu)
 * @date 3/5/11
 */

#include "Weapons/LawnMower.h"
#include "Utility/GlobalUtility.h"
#include "Shots/LawnMowerShot.h"
#include "Utility/SoundEffect.h"

LawnMower::LawnMower(AsteroidShip* owner) : Weapon(owner) {
   coolDown = 0;
   name = "Lawn Mower";
   currentFrame = 1; // Start this 1 ahead of lastFiredFame.
   lastFiredFrame = 0; // We use these three to handle the audio.
   soundPlaying = false;
   curAmmo = 2000;
   purchased = false;
}

LawnMower::~LawnMower() {
   // Do nothing.
}

void LawnMower::update(double timeDiff) {
   // Stop sound if we're no longer firing.
   /*
      // Lawn mower sound does not exist yet.
   if (currentFrame == lastFiredFrame && !soundPlaying) {
      // We should play sound.
      soundPlaying = true;
      soundHandle = SoundEffect::playSoundEffect("LawnMower.wav", true);
   } else if (currentFrame != lastFiredFrame && soundPlaying) {
      SoundEffect::stopSoundEffect(soundHandle);
      soundPlaying = false;
   }
   */
   ++currentFrame;
}

void LawnMower::fire() {
   if(!gameState->godMode && curAmmo <= 0)
      return;

   const double swingAmount = 0.5; // Radians from center.
   const double swingPeriod = 2; // Seconds.

   Point3D start = *ship->position;
   Vector3D shotDirection(*ship->forward);
   shotDirection.rotate(swingAmount * sin(M_PI * doubleTime() / swingPeriod), 
    *ship->up);
   gameState->custodian.add(new LawnMowerShot(start, shotDirection, ship));
   //std::set<Object3D*>* tempList = gameState->custodian.findCollisions(new LawnMowerShot(start, ship->shotDirection, ship));
   lastFiredFrame = currentFrame;
   if(!gameState->godMode) {
      // Take away some ammo
      curAmmo--;
   }
}

void LawnMower::debug() {
   printf("LawnMower!\n");
}

Point3D LawnMower::project(Object3D* object) {
   return *object->position;
}

bool LawnMower::shouldFire(Point3D* target, Point3D* aim) {
   return true;
}

