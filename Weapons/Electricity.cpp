/**
 * Electricity
 * This kills the crab.
 * @author Sterling Hirsh (shirsh@calpoly.edu)
 * @date Valentine's Day <3
 */

#include "Weapons/Electricity.h"
#include "Utility/GlobalUtility.h"
#include "Shots/ElectricityShot.h"
#include "Utility/SoundEffect.h"
#include "Text/GameMessage.h"
#include "Network/gamestate.pb.h"

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
   shotid = 0;
   
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

   // Stop sound if we're no longer firing.
   if (currentFrame == lastFiredFrame) {
      if (!soundPlaying) {
         // We should play sound.
         soundPlaying = true;
         soundHandle = SoundEffect::playSoundEffect("ElectricitySound", 
          ship->position, ship->velocity, 
          ship == ship->gameState->ship, 0.5f, true);
      }
   } else if (currentFrame != lastFiredFrame) {
      if (ship->gameState->gsm != ClientMode) {
         shotsFired = 0;
      }
      if (soundPlaying) {
         SoundEffect::stopSoundEffect(soundHandle);
         soundPlaying = false;
      }
      if (ship->gameState->gsm != ClientMode) {
         if (shotid != 0) {
            Object3D* shot = (*ship->custodian)[shotid];
            if (shot != NULL)
               shot->shouldRemove = true;
         }
      }
   }
   ++currentFrame;

   if (isOverheated() && this->ship == ship->gameState->ship)
      GameMessage::Add("Electricity overheated!", 30, 0);
}

void Electricity::fire() {
   if (!isReady())
      return;
   
   lastFiredFrame = currentFrame;

   // If it's client mode, wait for the shot packet to arrive, 
   // and then add to the game.
   if (ship->gameState->gsm != ClientMode) {
      if (shotsFired == 0) {
         timeStartedFiring = ship->gameState->getGameTime();
      }
      
      double curTime = ship->gameState->getGameTime();
      double timeFired = curTime - timeStartedFiring;
      int shotsToFire = 0;

      if (timeFired == 0) {
         shotsToFire = 1;
      } else {
         while (((shotsFired + shotsToFire) / timeFired) <= shotsPerSec) {
            shotsToFire++;
         }
      }
      
      ElectricityShot* shot;

      if (shotid != 0) {
         shot = static_cast<ElectricityShot*>((*ship->custodian)[shotid]);
         if (shot == NULL) {
            // Oops!
            shotid = 0;
         } else {
            // Update shot.
            shot->setPosAndDir(ship->shotOrigin, ship->shotDirection);
            shot->setStrength(shotsToFire);
         }
      }
      // Catch the oops from before.
      if (shotid == 0) {
         Point3D start = ship->shotOrigin;
         shot = new ElectricityShot(start, 
          ship->shotDirection, index, ship, ship->gameState);
         ship->custodian->add(shot);
         shotid = shot->id;
      }

      shotsFired += shotsToFire;
      addHeat(heatPerShot * shotsToFire);
   }

   ship->setShakeAmount(0.1f);
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

void Electricity::save(ast::Weapon* weap) {
   Weapon::save(weap);
   weap->set_shotsfired(shotsFired);
   weap->set_timestartedfiring(timeStartedFiring);
   weap->set_shotid(shotid);
}

void Electricity::load(const ast::Weapon& weap) {
   Weapon::load(weap);
   if (weap.has_shotsfired())
      shotsFired = weap.shotsfired();
   if (weap.has_timestartedfiring())
      timeStartedFiring = weap.timestartedfiring();
   if (weap.has_shotid())
      shotid = weap.shotid();
}

