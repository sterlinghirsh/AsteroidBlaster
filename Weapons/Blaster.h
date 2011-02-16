/**
 * Blaster! The original Asteroids Weapon.
 * @author Sterling Hirsh (shirsh@calpoly.edu)
 * @date 2/14/2011
 * <3
 */

#ifndef __BLASTER_H__
#define __BLASTER_H__

#include "Weapons/Weapon.h"

struct Point3D;
/* Classter Blaster */
class Blaster : public Weapon {
   public:
      Blaster(AsteroidShip* owner);
      virtual ~Blaster();
      virtual bool aimAt(double, Object3D*);
      virtual void update(double timeDiff);
      virtual void debug();
      virtual void fire();
      virtual double getTurnSpeed();

   protected:
      double shotSpeed;
      double turnSpeed;
      double randomVariationAmount;
      Point3D* lastShotPos;
};

#endif
