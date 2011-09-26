/**
 * RailGun: the the second gun.
 * @author Sterling Hirsh (shirsh@calpoly.edu)
 * @date Valentine's Day <3
 * This is an instant hit weapon.
 */

#include "Weapons/RailGun.h"
#include "Utility/GlobalUtility.h"
#include "Shots/BeamShot.h"
#include "Utility/Quaternion.h"
#include "Utility/SoundEffect.h"

RailGun::RailGun(AsteroidShip* owner, int _index)
: Weapon(owner, _index) {
   RAILGUN_WEAPON_INDEX = index;
   coolDown = 2; // Seconds
   name = "Railgun";
   curAmmo = -1;
   purchased = false;
   randomAIVariationAmount = 4.5;

   icon = "RailGunIcon";
   r = 1;
   g = 1;
   b = 0;
}

RailGun::~RailGun() {
   // Do nothing.
}

/**
 * Called every frame.
 */
void RailGun::update(double timeDiff) {
   Weapon::update(timeDiff);
}

/**
 * This is what actually shoots. Finally!
 */
void RailGun::fire() {
   // If it's client mode, wait for the shot packet to arrive, 
   // and then add to the game.
   if (ship->gameState->gsm == ClientMode) {
      return;
   }

   if (!isReady())
      return;
   timeLastFired = doubleTime();
   Point3D start = ship->shotOrigin;
   // Move start point by the shotDirection vector, multiplied by a scalar
   ship->setShakeAmount(5.0);
   ship->custodian->add(new BeamShot(start,
            ship->shotDirection, index, ship, ship->gameState));
   // Only take away ammo and play a sound if we're not in godMode
   if (!ship->gameState->godMode) {
      SoundEffect::playSoundEffect("Rail2.wav", ship->position, NULL, ship == ship->gameState->ship);
      //curAmmo--;
   }
}

/**
 * Basic debug function. Just in case!
 */
void RailGun::debug() {
   printf("Railgun!\n");
}

Point3D RailGun::project(Object3D* target, Vector3D addOn) {
   return *target->position;
}

bool RailGun::shouldFire(Point3D* target, Point3D* aim) {
   return  ((*target - ship->shotOrigin).getNormalized() == *aim);
}
