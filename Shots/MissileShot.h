/**
 * MissileShot.h
 */

#ifndef __MISSILESHOT_H__
#define __MISSILESHOT_H__

#include "Shots/Shot.h"

class MissileShot : public Shot {
   public:
      MissileShot(Point3D &posIn, Vector3D dirIn, AsteroidShip* const ownerIn, const GameState* _gameState);
      virtual void draw();
      virtual void update(double timeDiff);
      int particleNum;
      Vector3D particleDirection;
      virtual void hitWall(BoundingWall* wall);
};

#endif
