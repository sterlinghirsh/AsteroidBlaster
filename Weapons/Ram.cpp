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
#include "Text/GameMessage.h"

Ram::Ram(AsteroidShip* owner, int _index) : Weapon(owner, _index) {
   RAM_WEAPON_INDEX = index;
   coolDown = 0;
   name = "Charging Rhino";
   currentFrame = 1; // Start this 1 ahead of lastFiredFame.
   lastFiredFrame = 0; // We use these three to handle the audio.
   soundPlaying = false;
   curAmmo = 2000;
   purchased = false;
   icon = "RamIcon";
   
   currentHeat = 0;
   overheatLevel = 10;
   heatPerShot = 3.0;
   overheated = false;
   
   r = 0.5;
   g = 0;
   b = 0.5;
}

Ram::~Ram() {
   // Do nothing.
}

void Ram::update(double timeDiff) {
   if(currentFrame == lastFiredFrame) {
      if (!ship->gameState->godMode) {
         currentHeat += (heatPerShot/(double)level) * timeDiff;
      }
   } else if (currentHeat > 0) {
      currentHeat = std::max(currentHeat - timeDiff, 0.0);
   }

   if (overheated && currentHeat == 0) {
      overheated = false;
   } else if (currentHeat > overheatLevel) {
      overheated = true;
   }
   
   if (currentFrame == lastFiredFrame && !soundPlaying) {
      // We should play sound.
      soundPlaying = true;
      soundHandle = SoundEffect::playSoundEffect("Pulse", ship->position, ship == ship->gameState->ship, DEFAULT_VOLUME, true);
   } else if (currentFrame != lastFiredFrame && soundPlaying) {
      SoundEffect::stopSoundEffect(soundHandle);
      soundPlaying = false;
   }
   ++currentFrame;
   
   if (overheated && this->ship == ship->gameState->ship)
      GameMessage::Add("Charging Rhino overheated!", 30, 0);
}

void Ram::fire() {
   if(!ship->gameState->godMode && curAmmo <= 0)
      return;
   if (!isReady())
      return;
   Point3D start = ship->shotOrigin;
   //ship->forward->movePoint(start, 4);
   ship->setShakeAmount(0.0);
   /*
   ship->custodian->add(new RamShot(start, *(ship->forward), index, ship, ship->gameState));
   */
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

Point3D Ram::project(Object3D* object, Vector3D addOn) {
   return *object->position;
}

bool Ram::shouldFire(Point3D* target, Point3D* aim) {
   return true;
}

/**
 * This is for the weapon bar.
 */
double Ram::getCoolDownAmount() {
   return 1 - clamp(currentHeat / overheatLevel, 0, 1);
}

/**
 * This is for firing.
 */
bool Ram::isCooledDown() {
   return Weapon::isCooledDown() && !overheated;
}
