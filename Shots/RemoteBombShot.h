/**
 * RemoteBombShot.h
 * This bomb lasts for a few seconds before exploding.
 */

#ifndef __REMOTEBOMBSHOT_H__
#define __REMOTEBOMBSHOT_H__

#include "Shots/Shot.h"
#include "Shots/ExplosiveShot.h"

class RemoteBomber;

class RemoteBombShot : public ExplosiveShot {
   public:
      RemoteBombShot(Point3D &posIn, Vector3D dirIn, AsteroidShip* const ownerIn, RemoteBomber* const _weapon, const GameState* _gamestate);
      virtual ~RemoteBombShot();
      virtual void draw();
      virtual void update(double timeDiff);
      virtual void hitWall(BoundingWall* wall);
      // Called when the shot's isExploded becomes true.
      virtual void explode();

      // In seconds.
      double timeToExplode;
      RemoteBomber* weapon;
};

#endif

