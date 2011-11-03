/** * Tractor Beam
 * This pulls in crystals.
 * @author Sterling Hirsh (shirsh@calpoly.edu)
 * @date Valentine's Day <3
 */

#include "Weapons/TractorBeam.h"
#include "Utility/GlobalUtility.h"
#include "Shots/TractorBeamShot.h"
#include "Utility/SoundEffect.h"

#include "Network/gamestate.pb.h"

TractorBeam::TractorBeam(AsteroidShip* owner, int _index) : Weapon(owner, _index) {
   TRACTOR_WEAPON_INDEX = index;
   coolDown = 0;
   name = "Tractor Beam";
   lastFiredFrame = 0; // We use these three to handle the audio.
   soundPlaying = false;
   curAmmo = -1;
   purchased = true;
   randomAIVariationAmount = 4;
   range = 40;
   soundHandle = NULL;
   shotid = 0;

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
      if (shotid != 0) {
         Object3D* shot = (*ship->custodian)[shotid];
         if (shot != NULL)
            shot->shouldRemove = true;
      }
      shotid = 0;
   }
}

void TractorBeam::fire() {
   if (!isReady())
      return;

   if (ship->gameState->gsm != ClientMode) {
      TractorBeamShot* shot;
      if (shotid != 0) {
         shot = static_cast<TractorBeamShot*>((*ship->custodian)[shotid]);
         if (shot == NULL) {
            // Oops!
            shotid = 0;
         } else {
            // Update shot.
            shot->setPosAndDir(ship->shotOrigin, ship->shotDirection);
            shot->up->clone(ship->up);
         }
      }
      // Catch the oops from before.
      if (shotid == 0) {
         Point3D start = ship->shotOrigin;
         shot = new TractorBeamShot(start, ship->shotDirection, index, ship, ship->gameState);
         ship->custodian->add(shot);
         shotid = shot->id;
      }
   }

   lastFiredFrame = curFrame;
   // We should play sound.
   if (!soundPlaying) {
      soundPlaying = true;
      soundHandle = SoundEffect::playSoundEffect("TractorBeam.wav", ship->position, ship->velocity, ship == ship->gameState->ship, 0.2f, true);
   }
}

void TractorBeam::debug() {
   printf("TractorBeam!\n");
}

Point3D TractorBeam::project(Object3D* target, Vector3D addOn) {
   return *target->position;
}

bool TractorBeam::shouldFire(Point3D* target, Point3D* aim) {
   return true;
}

void TractorBeam::stopSounds() {
   if (soundHandle != NULL) {
      SoundEffect::stopSoundEffect(soundHandle);
      soundHandle = NULL;
      soundPlaying = false;
   }
}

void TractorBeam::save(ast::Weapon* weap) {
   Weapon::save(weap);
   weap->set_shotid(shotid);
}

void TractorBeam::load(const ast::Weapon& weap) {
   Weapon::load(weap);
   if (weap.has_shotid())
      shotid = weap.shotid();
}
