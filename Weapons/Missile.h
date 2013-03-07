/**
 * Missile! The original Asteroids Weapon.
 * @author Sterling Hirsh (shirsh@calpoly.edu)
 * @date 2/14/2011
 * <3
 */

#ifndef __MISSILE_H__
#define __MISSILE_H__

#include "Weapons/Weapon.h"

struct Point3D;
/* Classter Missile */
class Missile : public Weapon {
   public:
      Missile(AsteroidShip* owner);
      virtual ~Missile();
      virtual Point3D project(Object3D*);
      virtual void update(double timeDiff);
      virtual bool shouldFire(Point3D*, Point3D*);
      void godMode(bool enabled);
      virtual void debug();
      virtual void fire();

   protected:
      double shotSpeed;
      double turnSpeed;
      double randomVariationAmount;
      Point3D* lastShotPos;
};

#endif
