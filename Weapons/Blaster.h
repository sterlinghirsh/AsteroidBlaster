/**
 * Blaster! The original Asteroids Weapon.
 * @author Sterling Hirsh (shirsh@calpoly.edu)
 * @date 2/14/2011
 * <3
 */

#ifndef __BLASTER_H__
#define __BLASTER_H__

#include "Weapons/Weapon.h"

class Point3D;
/* Classter Blaster */
class Blaster : public Weapon {
   public:
      Blaster(AsteroidShip* owner, int _index);
      virtual ~Blaster();
      virtual Point3D project(Object3D* target, Vector3D addOn);
      virtual void update(double timeDiff);
      virtual bool shouldFire(Point3D*, Point3D*);
      void godMode(bool enabled);
      virtual void debug();
      virtual void fire();
      virtual double getCoolDownAmount();
      virtual bool isReady();

   protected:
      double shotSpeed;
      double turnSpeed;
      double randomVariationAmount;
      Point3D* lastShotPos;
      unsigned long lastFiredFrame;
};

#endif
