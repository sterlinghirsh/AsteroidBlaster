/**
 * Tractor Beam
 * This pulls in crystals.
 * @author Sterling Hirsh (shirsh@calpoly.edu)
 * @date Valentine's Day <3
 */

#include "Weapons/TractorBeam.h"
#include "Utility/GlobalUtility.h"
#include "Shots/TractorBeamShot.h"
#include "Utility/SoundEffect.h"

TractorBeam::TractorBeam(AsteroidShip* owner) : Weapon(owner) {
   coolDown = 0;
   name = "Tractor Beam";
   currentFrame = 1; // Start this 1 ahead of lastFiredFame.
   lastFiredFrame = 0; // We use these three to handle the audio.
   soundPlaying = false;
   curAmmo = 2000;
   purchased = false;
}

TractorBeam::~TractorBeam() {
   // Do nothing.
}

void TractorBeam::update(double timeDiff) {
   // Stop sound if we're no longer firing.
   if (currentFrame == lastFiredFrame && !soundPlaying) {
      // We should play sound.
      soundPlaying = true;
      soundHandle = SoundEffect::playSoundEffect("TractorBeam.wav", true);
   } else if (currentFrame != lastFiredFrame && soundPlaying) {
      SoundEffect::stopSoundEffect(soundHandle);
      soundPlaying = false;
   }
   ++currentFrame;
}

void TractorBeam::fire() {
   if(!gameState->godMode && curAmmo <= 0)
      return;

   Point3D start = *ship->position;
   gameState->custodian.add(new TractorBeamShot(start, ship->shotDirection, ship));
   //std::set<Object3D*>* tempList = gameState->custodian.findCollisions(new TractorBeamShot(start, ship->shotDirection, ship));
   lastFiredFrame = currentFrame;
   if(!gameState->godMode) {
      // Take away some ammo
      curAmmo--;
   }
}

void TractorBeam::debug() {
   printf("TractorBeam!\n");
}

Point3D TractorBeam::project(Object3D* object) {
   return *object->position;
}

bool TractorBeam::shouldFire(Point3D* target, Point3D* aim) {
   return true;
}
