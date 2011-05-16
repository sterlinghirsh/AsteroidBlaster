/**
 * Shot.h
 * Sterling Hirsh
 * A shot in 3d asteroids.
 */

#ifndef __SHOT_H__
#define __SHOT_H__

#include "Utility/Point3D.h"
#include "Utility/Vector3D.h"
#include "Utility/GlobalUtility.h"
#include "Items/Asteroid3D.h"
#include "Items/Object3D.h"
#include "Items/AsteroidShip.h"

extern materialStruct ShotMaterial;

// Incomplete declaration so we can use the pointer.
class AsteroidShip;

class Shot : public Object3D {
   public:
      double timeFired;
      double lifetime; // Seconds
      bool persist;
      static double frequency; // Shots per sec
      AsteroidShip* owner; // Who fired the shot?
      Shot(Point3D& posIn, Vector3D dirIn, int _weaponIndex, AsteroidShip* ownerIn, const GameState* _gameState);
      virtual ~Shot();

      virtual void draw();
      virtual void update(double timeDiff);
      virtual void drawInMinimap();
      virtual void debug();
      bool isBeam;

      int weaponIndex;

      double damage;

      Point3D getWallIntersectionPoint(BoundingWall* wall);
      virtual double getDamage(Object3D* collidedObject);
};

#endif
