/**
 * ExplosiveShot.h
 * All bombs, missiles, etc will extend this in some way.
 */

#ifndef __EXPLOSIVESHOT_H__
#define __EXPLOSIVESHOT_H__

#include "Shots/Shot.h"

class ExplosiveShot : public Shot {
   public:
      ExplosiveShot(Point3D &posIn, Vector3D dirIn, AsteroidShip* const ownerIn, const GameState* _gameState);
      virtual void draw();
      // update should probably check isExploded to see if it's time to blow up.
      virtual void update(double timeDiff);
      // Called when the ExplosiveShot realizes it has been exploded.
      virtual void explode();
      virtual void handleCollision(Drawable* other);
      virtual void hitWall(BoundingWall* wall);

      //int particleNum;
      double explodeRadius;
      // The frame that the bomb exploded on.
      double frameExploded;


      //Vector3D particleDirection;
      // Set to true whenever the ExplosiveShot should blow up.
      bool isExploded;
};

#endif
