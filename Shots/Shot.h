/**
 * Shot.h
 * Sterling Hirsh
 * A shot in 3d asteroids.
 */

#ifndef __SHOT_H__
#define __SHOT_H__

#include "Utility/Point3D.h"
#include "Utility/Vector3D.h"
#include "Graphics/GlutUtility.h"
#include "Items/Asteroid3D.h"
#include "Utility/Object3D.h"
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
      Shot(Point3D& posIn, Vector3D dirIn, AsteroidShip* ownerIn);
      virtual ~Shot();

      virtual void draw();
      virtual void update(double timeDiff);
      virtual void handleCollision(Object3D* other);
      virtual void drawInMinimap();
      virtual void debug();
};

#endif