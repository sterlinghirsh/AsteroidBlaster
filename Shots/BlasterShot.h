/**
 * BlasterShot.h
 */

#ifndef __BLASTERSHOT_H__
#define __BLASTERSHOT_H__

#include "Shots/Shot.h"

class BlasterShot : public Shot {
   public:
      BlasterShot(Point3D &posIn, Vector3D dirIn, int _weaponIndex, AsteroidShip* const ownerIn, const GameState* _gameState);
      virtual ~BlasterShot();
      virtual void draw();
      virtual void update(double timeDiff);
      int particleNum;
      Vector3D particleDirection;
      virtual void hitWall(BoundingWall* wall);
      CollisionPoint* collisionPoint;
      int soundHandle;
};

#endif
