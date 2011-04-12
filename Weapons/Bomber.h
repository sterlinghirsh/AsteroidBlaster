/**
 * Bomber. Used as a base for all other bombs.
 * @author Taylor Arnicar
 * @date 4/10/2011
 */

#ifndef __BOMBER_H__
#define __BOMBER_H__

#include "Weapons/Weapon.h"

struct Point3D;

class Bomber : public Weapon {
   public:
      Bomber(AsteroidShip* owner);
      virtual ~Bomber();
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
