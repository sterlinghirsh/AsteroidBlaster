/**
 * Lawn Mower
 * This knocks around asteroids like a base ball bat.
 * @author Sterling Hirsh (shirsh@calpoly.edu)
 * @date 3/5/11
 */

#ifndef __LAWNMOWER_H__
#define __LAWNMOWER_H__

#include "Weapons/Weapon.h"
#include "Items/AsteroidShip.h"

class AsteroidShip;

class LawnMower : public Weapon {
   public:
      LawnMower(AsteroidShip* owner);
      virtual ~LawnMower();
      virtual Point3D project(Object3D*);
      virtual bool shouldFire(Point3D*, Point3D*);
      virtual void update(double timeDiff);
      virtual void debug();
      virtual void fire();
   private:
      int currentFrame;
      int lastFiredFrame;
      bool soundPlaying;
      int soundHandle;
};

#endif

