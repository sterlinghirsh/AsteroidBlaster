/**
 * Tractor Beam
 * This pulls in crystals.
 * @author Sterling Hirsh (shirsh@calpoly.edu)
 * @date Valentine's Day <3
 */

#ifndef __ELECTRICITY_H__
#define __ELECTRICITY_H__
#include "Weapons/Weapon.h"
#include "Items/AsteroidShip.h"

class AsteroidShip;

class Electricity : public Weapon {
   public:
      Electricity(AsteroidShip* owner, int _index);
      virtual ~Electricity();
      virtual Point3D project(Object3D*);
      virtual bool shouldFire(Point3D*, Point3D*);
      virtual void update(double timeDiff);
      virtual void debug();
      virtual void fire();
      int shotsFired;
      int shotsPerSec;
      double timeStartedFiring;
      virtual void stopSounds();
   private:
      int currentFrame;
      int lastFiredFrame;
      bool soundPlaying;
      int soundHandle;
};

#endif
