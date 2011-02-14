/**
 * Weapon Class for AsteroidBlaster
 * @author Sterling Hirsh (shirsh@calpoly.edu)
 * This produces shots.
 * Well, subclasses of this will.
 * Pew Pew!
 */

#ifndef __WEAPON_H__
#define __WEAPON_H__

//#include "Shots/AsteroidShot.h"

enum WeaponType {
   PROJECTILE,
   BEAM,
   OTHER
};

class Weapon {
   virtual ~Weapon();
   virtual void fire();
   protected:
      WeaponType type; 
};

#endif
