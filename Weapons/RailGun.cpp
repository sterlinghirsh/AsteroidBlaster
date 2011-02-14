/**
 * RailGun: the the second gun.
 * @author Sterling Hirsh (shirsh@calpoly.edu)
 * @date Valentine's Day <3
 * This is an instant hit weapon.
 */

#include "Weapons/RailGun.h"
#include "Graphics/GlutUtility.h"
#include "Shots/BeamShot.h"

RailGun::RailGun(AsteroidShip* owner)
 : Weapon(owner) {
    coolDown = 2; // Seconds
}

RailGun::~RailGun() {
   // Do nothing.
}

/**
 * Called every frame.
 * We'll probably keep track of something or other here.
 */
void RailGun::update(double timeDiff) {
   // Do nothing.
}

/**
 * This is what actually shoots. Finally!
 */
void RailGun::fire() {
   if (!isCooledDown())
      return;
   timeLastFired = doubleTime();
   Point3D start = *ship->position;
   gameState->custodian.add(new BeamShot(start,
    ship->shotDirection, ship));
}

/**
 * Basic debug function. Just in case!
 */
void RailGun::debug() {
   printf("RailGun!\n");
}

bool RailGun::aimAt(Object3D* target) {
   return false;
}
