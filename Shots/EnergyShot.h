/**
 * EnergyShot.h
 */

#ifndef __ENERGYSHOT_H__
#define __ENERGYSHOT_H__

#include "Shots/Shot.h"

class Energy;

class EnergyShot : public Shot {
   public:
      EnergyShot(Point3D &posIn, Vector3D dirIn, AsteroidShip* const ownerIn, Energy* const weaponIn, const GameState* _gameState);
      virtual void draw();
      virtual void update(double timeDiff);
      int particleNum;
      Vector3D particleDirection;
      double chargeTime;
      virtual void hitWall(BoundingWall* wall);
      virtual void updateChargeTime(double newChargeTime);
      double damagePerSecond;
      Energy* weapon;
};

#endif
