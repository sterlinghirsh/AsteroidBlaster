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
      virtual Point3D project(Object3D*, Vector3D);
      virtual bool shouldFire(Point3D*, Point3D*);
      virtual void update(double timeDiff);
      virtual void debug();
      virtual void fire();
      int shotsFired;
      int shotsPerSec;
      unsigned shotid;
      float timeStartedFiring;
      virtual void stopSounds();
      
      virtual double getCoolDownAmount();
      
      virtual void save(ast::Weapon* weap);
      virtual bool saveDiff(const ast::Weapon& old, ast::Weapon* weap);
      virtual void load(const ast::Weapon& weap);
   private:
      int currentFrame;
      int lastFiredFrame;
      bool soundPlaying;
      SoundChannel* soundHandle;
};

#endif
