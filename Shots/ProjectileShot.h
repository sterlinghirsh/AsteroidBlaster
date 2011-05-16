/**
 * ProjectileShot.h
 */

#ifndef __PROJECTILESHOT_H__
#define __PROJECTILESHOT_H__

#include "Shots/Shot.h"

class ProjectileShot : public Shot {
   public:
      ProjectileShot(Point3D &posIn, Vector3D dirIn, int _weaponIndex, AsteroidShip* const ownerIn, const GameState* _gameState);
      virtual void draw();
      virtual void update(double timeDiff);
      int particleNum;
      Vector3D particleDirection;
      virtual void hitWall(BoundingWall* wall);
      CollisionPoint* collisionPoint;
};

#endif
