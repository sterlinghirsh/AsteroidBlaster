/**
 * Sterling Hirsh
 * Asteroid3D.h
 * A randomly generated asteroid object.
 */

#ifndef __ASTEROID3D_H__
#define __ASTEROID3D_H__

#include "Graphics/Mesh3D.h"
#include "Utility/Point3D.h"
#include "Utility/Object3D.h"
#include <list>

class Asteroid3D : public Object3D {
   public:
      Mesh3D mesh;
      Vector3D rotationVector;
      double rotationSpeed;
      double rotationAmount;
      double scalex, scaley, scalez;
      double sizeX, sizeY, sizeZ, collisionRadius;
      
      Asteroid3D(double r, double worldSizeIn);
      virtual ~Asteroid3D();
      void InitAsteroid(double r, double worldSizeIn);
      void draw();
      void update(double timeDiff);
      bool handleHit(std::list<Asteroid3D*>& asteroids);
      virtual void handleCollision(Object3D* other);
      Asteroid3D* makeChild();
      void newRandomPosition();
      virtual void debug();

   private:
      double randRadius(double r);
      double worldSize;
};

#endif
