/**
 * Energy! The original Asteroids Weapon.
 * @author Sterling Hirsh (shirsh@calpoly.edu)
 * @date 2/14/2011
 * <3
 */

#ifndef __ENERGY_H__
#define __ENERGY_H__

#include "Weapons/Weapon.h"

class EnergyShot;
class Point3D;
/* Classter Energy */
class Energy : public Weapon {
   public:
      Energy(AsteroidShip* owner, int _index);
      virtual ~Energy();
      virtual Point3D project(Object3D*, Vector3D);
      virtual void update(double timeDiff);
      virtual bool shouldFire(Point3D*, Point3D*);
      void godMode(bool enabled); // ??? -shirsh
      virtual void debug();
      virtual void fire();
      virtual void resetChargingShot();
      EnergyShot* chargingShot;
      virtual void stopSounds();

   protected:
      double shotSpeed;
      double turnSpeed;
      double randomVariationAmount;
      Point3D* lastShotPos;
      unsigned long lastFiredFrame;
      double chargeStartTime;

      bool chargingSoundPlaying;
      int soundHandle;
};

#endif
