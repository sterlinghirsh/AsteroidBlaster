/**
 * EnergyShot.h
 */

#ifndef __ENERGYSHOT_H__
#define __ENERGYSHOT_H__

#include "Shots/Shot.h"

class Energy;

class EnergyShot : public Shot {
   public:
      EnergyShot(Point3D &posIn, Vector3D dirIn, int _weaponIndex, AsteroidShip* const ownerIn, const GameState* _gameState);
      virtual void draw();
      virtual void update(double timeDiff);
      int particleNum;
      Vector3D particleDirection;
      float chargeTime;
      virtual void hitWall(BoundingWall* wall);
      virtual void updateChargeTime(double newChargeTime);
      float damagePerSecond;
      Energy* weapon;
      CollisionSphere* collisionSphere;
      
      virtual void save(ast::Entity* ent);
      virtual bool saveDiff(const ast::Entity& old, ast::Entity* ent);
      virtual void load(const ast::Entity& ent);
};

#endif
