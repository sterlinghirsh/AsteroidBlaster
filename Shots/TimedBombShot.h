/**
 * TimedBombShot.h
 * This bomb lasts for a few seconds before exploding.
 */

#ifndef __TIMEDBOMBSHOT_H__
#define __TIMEDBOMBSHOT_H__

#include "Shots/Shot.h"
#include "Shots/ExplosiveShot.h"

class TimedBombShot : public ExplosiveShot {
   public:
      TimedBombShot(Point3D &posIn, Vector3D dirIn, AsteroidShip* const ownerIn, const GameState* _gamestate);
      virtual ~TimedBombShot();
      virtual void draw();
      virtual void update(double timeDiff);
      virtual void handleCollision(Drawable* other);
      void hitWall(BoundingWall* wall);
      // Called when the shot's isExploded becomes true.
      virtual void explode();

      // In seconds.
      double timeToExplode;
};

#endif

