/**
 * Tractor Beam
 * This pulls in crystals.
 * @author Sterling Hirsh (shirsh@calpoly.edu)
 * @date Valentine's Day <3
 */

#include "Weapons/SpikeBall.h"
#include "Utility/GlobalUtility.h"
//#include "Shots/SpikeBallShot.h"
#include "Utility/SoundEffect.h"

SpikeBall::SpikeBall(AsteroidShip* owner) : Weapon(owner) {
   coolDown = 0;
   name = "Charging Rhino";
   currentFrame = 1; // Start this 1 ahead of lastFiredFame.
   lastFiredFrame = 0; // We use these three to handle the audio.
   soundPlaying = false;
   curAmmo = 2000;
   purchased = false;
   level = 0;
}

SpikeBall::~SpikeBall() {
   // Do nothing.
}

void SpikeBall::update(double timeDiff) {
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

void SpikeBall::fire() {
   if(!gameState->godMode && curAmmo <= 0)
      return;

   Point3D start = *ship->position;
   ship->setShakeAmount(0.0);
   //gameState->custodian.add(new SpikeBallShot(start, *(ship->forward), ship));
   //std::set<Object3D*>* tempList = gameState->custodian.findCollisions(new SpikeBallShot(start, ship->shotDirection, ship));
   lastFiredFrame = currentFrame;
   if(!gameState->godMode) {
      // Take away some ammo
      curAmmo--;
   }
}

void SpikeBall::debug() {
   printf("SpikeBall!\n");
}

Point3D SpikeBall::project(Object3D* object) {
   return *object->position;
}

bool SpikeBall::shouldFire(Point3D* target, Point3D* aim) {
   return true;
}
