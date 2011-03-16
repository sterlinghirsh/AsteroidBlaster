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
   coolDown = 0;
   name = "Pikachu's Wrath";
   currentFrame = 1; // Start this 1 ahead of lastFiredFame.
   lastFiredFrame = 0; // We use these three to handle the audio.
   soundPlaying = false;
   curAmmo = 2000;
   purchased = false;
   level = 0;
}

Electricity::~Electricity() {
   // Do nothing.
}

void Electricity::update(double timeDiff) {
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
   if(!gameState->godMode && curAmmo <= 0)
      return;

   Point3D start = *ship->position;
   ship->setShakeAmount(0.1);
   gameState->custodian.add(new ElectricityShot(start, ship->shotDirection, ship));
   //std::set<Object3D*>* tempList = gameState->custodian.findCollisions(new ElectricityShot(start, ship->shotDirection, ship));
   lastFiredFrame = currentFrame;
   if(!gameState->godMode) {
      // Take away some ammo
      curAmmo--;
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
