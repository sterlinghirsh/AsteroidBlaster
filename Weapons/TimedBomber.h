/**
 * TimedBomber. Used as an example for other Explosive Weapon implementations.
 * @author Taylor Arnicar
 * @date 4/10/2011
 */

#ifndef __TIMEDBOMBER_H__
#define __TIMEDBOMBER_H__

#include "Weapons/Weapon.h"

class Point3D;

class TimedBomber : public Weapon {
   public:
      TimedBomber(AsteroidShip* owner, int _index);
      virtual ~TimedBomber();
      virtual Point3D project(Object3D*, Vector3D);
      virtual void update(double timeDiff);
      virtual bool shouldFire(Point3D*, Point3D*);
      void godMode(bool enabled);
      virtual void debug();
      virtual void fire();
   
   protected:
      double shotSpeed;
      double randomVariationAmount;
      Point3D* lastShotPos;
};

#endif
