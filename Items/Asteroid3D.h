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
#include "Items/Ring.h"
#include <list>
#include <vector>

class Asteroid3D : public Object3D {
   public:
      Mesh3D mesh;
      Vector3D rotationVector;
      double rotationSpeed;
      double rotationAmount;
      double scalex, scaley, scalez;
      double worldSize;
      double sizeX, sizeY, sizeZ, collisionRadius;
      
      Asteroid3D(double r, double worldSizeIn);
      virtual ~Asteroid3D();
      void InitAsteroid(double r, double worldSizeIn);
      void draw();
      void drawStrip(Ring r1, Ring r2, int type);
      void update(double timeDiff);
      virtual void handleCollision(Object3D* other);
      bool handleHit(std::list<Asteroid3D*>& asteroids);
      Asteroid3D* makeChild();
      void newRandomPosition();
      void debug();
      

   private:
      double randRadius(double r);
      std::vector<Ring> _rList;
      int _nPoints;
};

#endif
