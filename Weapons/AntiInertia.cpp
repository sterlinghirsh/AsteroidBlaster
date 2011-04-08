/**
 * Anti Inertia
 * Slows down everything that collides with the beam
 * @author Ryuho Kudo
 * @date 3-16-11
 */

#include "Weapons/AntiInertia.h"
#include "Utility/GlobalUtility.h"
#include "Shots/AntiInertiaShot.h"
#include "Utility/SoundEffect.h"

AntiInertia::AntiInertia(AsteroidShip* owner) : Weapon(owner) {
   coolDown = 0;
   name = "Anti Inertia Beam";
   currentFrame = 1; // Start this 1 ahead of lastFiredFame.
   lastFiredFrame = 0; // We use these three to handle the audio.
   soundPlaying = false;
   curAmmo = 2000;
   purchased = false;
}

AntiInertia::~AntiInertia() {
   // Do nothing.
}

void AntiInertia::update(double timeDiff) {
   // Stop sound if we're no longer firing.
   if (currentFrame == lastFiredFrame && !soundPlaying) {
      // We should play sound.
      soundPlaying = true;
      soundHandle = SoundEffect::playSoundEffect("Pulse", true);
   } else if (currentFrame != lastFiredFrame && soundPlaying) {
      SoundEffect::stopSoundEffect(soundHandle);
      soundPlaying = false;
   }
   ++currentFrame;
}

void AntiInertia::fire() {
   if(!ship->gameState->godMode && curAmmo <= 0)
      return;

   Point3D start = ship->shotOrigin;
   ship->custodian->add(new AntiInertiaShot(start, ship->shotDirection, ship, ship->gameState));
   lastFiredFrame = currentFrame;
   if(!ship->gameState->godMode) {
      // Take away some ammo
      curAmmo--;
   }
}

void AntiInertia::debug() {
   printf("AntiInertia!\n");
}

Point3D AntiInertia::project(Object3D* object) {
   return *object->position;
}

bool AntiInertia::shouldFire(Point3D* target, Point3D* aim) {
   return true;
}
