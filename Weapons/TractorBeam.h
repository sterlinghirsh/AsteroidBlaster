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
      virtual Point3D project(Object3D* target, Vector3D addOn);
      virtual bool shouldFire(Point3D*, Point3D*);
      virtual void update(double timeDiff);
      virtual void debug();
      virtual void fire();
      virtual void stopSounds();
      virtual void save(ast::Weapon* weap);
      virtual void load(const ast::Weapon& weap);
   private:
      long unsigned int lastFiredFrame;
      bool soundPlaying;
      SoundChannel* soundHandle;
      unsigned shotid;
};

#endif
