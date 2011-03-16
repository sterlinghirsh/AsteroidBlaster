/**
 * Anti Inertia
 * Slows down everything that collides with the beam
 * @author Ryuho Kudo
 * @date 3-16-11
 */

#ifndef __ANTIINERTIA_H__
#define __ANTIINERTIA_H__
#include "Weapons/Weapon.h"
#include "Items/AsteroidShip.h"

class AsteroidShip;

class AntiInertia : public Weapon {
   public:
      AntiInertia(AsteroidShip* owner);
      virtual ~AntiInertia();
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
