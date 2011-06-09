/**
 * Tractor Beam
 * This pulls in crystals.
 * @author Sterling Hirsh (shirsh@calpoly.edu)
 * @date Valentine's Day <3
 */

#ifndef __RAM_H__
#define __RAM_H__
#include "Weapons/Weapon.h"
#include "Items/AsteroidShip.h"

class AsteroidShip;

class Ram : public Weapon {
   public:
      Ram(AsteroidShip* owner, int _index);
      virtual ~Ram();
      virtual Point3D project(Object3D*, Vector3D);
      virtual bool shouldFire(Point3D*, Point3D*);
      virtual void update(double timeDiff);
      virtual void debug();
      virtual void fire();
      virtual double getCoolDownAmount();
      virtual bool isCooledDown();
      
      double currentHeat; // Measured in seconds.
      double overheatLevel; // Also in seconds. When currentHeat hits this, we overheat.
      double heatPerShot; // Added to currentHeat every shot.
      bool overheated;
   private:
      int currentFrame;
      int lastFiredFrame;
      bool soundPlaying;
      int soundHandle;
};

#endif
