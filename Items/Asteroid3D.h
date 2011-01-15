/**
 * Sterling Hirsh
 * Asteroid3D.h
 * A randomly generated asteroid object.
 */

#ifndef __ASTEROID3D_H__
#define __ASTEROID3D_H__

#include "Graphics/Mesh3D.h"
#include "Utility/Point3D.h"
#include <list>

class Asteroid3D {
   public:
      double radius;
      Mesh3D mesh;
      Vector3D rotationVector;
      double rotationSpeed;
      double rotationAmount;
      double scalex, scaley, scalez;
      double worldSize;
      Point3D position;
      Vector3D direction;
      double sizeX, sizeY, sizeZ, collisionRadius;
      
      Asteroid3D(double r, double worldSizeIn);
      void InitAsteroid(double r, double worldSizeIn);
      void draw(bool drawSmooth);
      void updatePosition(double timeDiff);
      bool handleHit(std::list<Asteroid3D*>& asteroids);
      Asteroid3D* makeChild();

   private:
      double randRadius(double r);
};

#endif
