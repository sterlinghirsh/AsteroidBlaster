/**
 * RemoteBomber. Used as an example for other Explosive Weapon implementations.
 * @author Taylor Arnicar
 * @date 4/10/2011
 */

#ifndef __REMOTEBOMBER_H__
#define __REMOTEBOMBER_H__

#include "Weapons/Weapon.h"

class Point3D;
class RemoteBombShot;

class RemoteBomber : public Weapon {
   public:
      RemoteBomber(AsteroidShip* owner, int _index);
      virtual ~RemoteBomber();
      virtual Point3D project(Object3D*, Vector3D);
      virtual void update(double timeDiff);
      virtual bool shouldFire(Point3D*, Point3D*);
      void godMode(bool enabled);
      virtual void debug();
      virtual void fire();
      RemoteBombShot* firedShot;
   
   protected:
      double shotSpeed;
      double turnSpeed;
      double randomVariationAmount;
      Point3D* lastShotPos;
      unsigned long lastFiredFrame;
      
};

#endif
