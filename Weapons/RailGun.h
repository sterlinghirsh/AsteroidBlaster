/**
 * RailGun: the the second gun.
 * @author Sterling Hirsh (shirsh@calpoly.edu)
 * @date Valentine's Day <3
 * This is an instant hit weapon.
 */

#ifndef __RAILGUN_H__
#define __RAILGUN_H__

#include "Weapons/Weapon.h"

class RailGun : public Weapon {
   public:
      RailGun(AsteroidShip* owner);
      virtual ~RailGun();
      virtual Point3D project(Object3D*);
      virtual void update(double timeDiff);
      virtual void debug();
      virtual void fire();
};

#endif
