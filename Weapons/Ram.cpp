/**
 * Tractor Beam
 * This pulls in crystals.
 * @author Sterling Hirsh (shirsh@calpoly.edu)
 * @date Valentine's Day <3
 */

#include "Weapons/Ram.h"
#include "Utility/GlobalUtility.h"
#include "Shots/RamShot.h"
#include "Utility/SoundEffect.h"

Ram::Ram(AsteroidShip* owner) : Weapon(owner) {
   coolDown = 0;
   name = "Charging Rhino";
   currentFrame = 1; // Start this 1 ahead of lastFiredFame.
   lastFiredFrame = 0; // We use these three to handle the audio.
   soundPlaying = false;
   curAmmo = 2000;
   purchased = false;
}

Ram::~Ram() {
   // Do nothing.
}

void Ram::update(double timeDiff) {
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

void Ram::fire() {
   if(!ship->gameState->godMode && curAmmo <= 0)
      return;

   Point3D start = ship->shotOrigin;
   //ship->forward->movePoint(start, 4);
   ship->setShakeAmount(0.0);
   ship->custodian->add(new RamShot(start, *(ship->forward), ship, ship->gameState));
   //std::set<Object3D*>* tempList = gameState->custodian.findCollisions(new RamShot(start, ship->shotDirection, ship));
   lastFiredFrame = currentFrame;
   if(!ship->gameState->godMode) {
      // Take away some ammo
      curAmmo--;
   }
}

void Ram::debug() {
   printf("Ram!\n");
}

Point3D Ram::project(Object3D* object) {
   return *object->position;
}

bool Ram::shouldFire(Point3D* target, Point3D* aim) {
   return true;
}
