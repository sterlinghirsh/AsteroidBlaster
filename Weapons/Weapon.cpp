/**
 * Weapon Class for AsteroidBlaster
 * @author Sterling Hirsh (shirsh@calpoly.edu)
 * This is a superclass to real weapons.
 */

#include "Weapons/Weapon.h"
#include "Utility/Object3D.h"
#include "Items/AsteroidShip.h"
#include "Utility/Custodian.h"

/**
 * Initialize the ship and timeLastFired.
 */
Weapon::Weapon(AsteroidShip* owner)
 : ship(owner), timeLastFired(0) {
   // Do nothing else.
}

Weapon::~Weapon() {
   // Do nothing.
}

/**
 * Returns true if the weapon is ready to be fired.
 */
bool Weapon::isCooledDown() {
   return doubleTime() > timeLastFired + coolDown;
}

