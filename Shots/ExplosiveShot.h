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
      // update should probably check isExploded to see if it's time to blow up.
      virtual void update(double timeDiff);
      // Called when the ExplosiveShot realizes it has been exploded.
      virtual void explode();
      virtual void handleCollision(Drawable* other);
      virtual void hitWall(BoundingWall* wall);

      //int particleNum;
      double explodeRadius;

      //Vector3D particleDirection;
      // Set to true whenever the ExplosiveShot should blow up.
      bool isExploded;
      /* Set to true if a bomb runs into an asteroid.
       * This will cause the asteroid to explode in the next frame,
       * if it has not already.
       */
      bool shouldExplode;
      virtual double getDamage(Object3D* other);
};

#endif
