/**
 * AsteroidShot.h
 * Sterling Hirsh
 * A shot in 3d asteroids.
 */

#ifndef __ASTEROIDSHOT_H__
#define __ASTEROIDSHOT_H__

#include "Utility/Point3D.h"
#include "Utility/Vector3D.h"
#include "Graphics/GlutUtility.h"
#include "Items/Asteroid3D.h"
#include "Utility/Object3D.h"
#include "Items/AsteroidShip.h"

extern materialStruct ShotMaterial;

// Incomplete declaration so we can use the pointer.
class AsteroidShip;

class AsteroidShot : public Object3D {
   public:
      double timeFired;
      double lifetime; // Seconds
      bool persist;
      static double frequency; // Shots per sec
      AsteroidShip* owner; // Who fired the shot?

      AsteroidShot(Point3D &posIn, Vector3D dirIn, AsteroidShip* const ownerIn );
      virtual void draw();
      virtual void update(double timeDiff);
      virtual void handleCollision(Object3D* other);
};

#endif
