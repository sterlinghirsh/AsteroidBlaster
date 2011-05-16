/**
 * Tractor Beam
 * This pulls in crystals.
 * @author Sterling Hirsh (shirsh@calpoly.edu)
 * @date Valentine's Day <3
 */

#ifndef __TRACTORBEAM_H__
#define __TRACTORBEAM_H__

#include "Weapons/Weapon.h"
#include "Items/AsteroidShip.h"

class AsteroidShip;

class TractorBeam : public Weapon {
   public:
      TractorBeam(AsteroidShip* owner, int _index);
      virtual ~TractorBeam();
      virtual Point3D project(Object3D*);
      virtual bool shouldFire(Point3D*, Point3D*);
      virtual void update(double timeDiff);
      virtual void debug();
      virtual void fire();
      virtual void stopSounds();
   private:
      long unsigned int lastFiredFrame;
      bool soundPlaying;
      int soundHandle;
};

#endif
