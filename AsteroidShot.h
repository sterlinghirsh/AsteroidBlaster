/**
 * AsteroidShot.h
 * Sterling Hirsh
 * A shot in 3d asteroids.
 */

#ifndef __ASTEROIDSHOT_H__
#define __ASTEROIDSHOT_H__

#include "Point3D.h"
#include "Vector3D.h"
#include "GlutUtility.h"
#include "Asteroid3D.h"

extern materialStruct ShotMaterial;

class AsteroidShot {
   public:
      double timeFired;
      double lifetime; // Seconds
      bool persist;
      Point3D position;
      Vector3D direction;
      static double frequency; // Shots per sec

      AsteroidShot(Point3D posIn, Vector3D dirIn);
      virtual void draw();
      virtual void updatePosition(double timeDiff);
      virtual bool checkHit(Asteroid3D* asteroid);
};

#endif
