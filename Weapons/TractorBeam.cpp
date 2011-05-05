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
   lastFiredFrame = 0; // We use these three to handle the audio.
   soundPlaying = false;
   curAmmo = -1;
   purchased = true;
   icon = "TractorBeamIcon";
   r = 0;
   g = 1;
   b = 0;
}

TractorBeam::~TractorBeam() {
   // Do nothing.
}

void TractorBeam::update(double timeDiff) {
   // Stop sound if we're no longer firing.
   if (curFrame > lastFiredFrame + 1 && soundPlaying) {
      SoundEffect::stopSoundEffect(soundHandle);
      soundPlaying = false;
   }
}

void TractorBeam::fire() {
   if (!isReady())
      return;

   Point3D start = ship->shotOrigin;
   ship->custodian->add(new TractorBeamShot(start, ship->shotDirection, ship, ship->gameState));
   //std::set<Object3D*>* tempList = gameState->custodian.findCollisions(new TractorBeamShot(start, ship->shotDirection, ship));
   lastFiredFrame = curFrame;
   // We should play sound.
   if (!soundPlaying) {
      soundPlaying = true;
      soundHandle = SoundEffect::playSoundEffect("TractorBeam.wav", true);
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
