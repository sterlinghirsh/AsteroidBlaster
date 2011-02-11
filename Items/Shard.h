/**
 * Chris Brenton
 * Shard.h
 * A randomly generated shard object.
 */

#ifndef __SHARD_H__
#define __SHARD_H__
//#ifndef __ASTEROID3D_H__
//#define __ASTEROID3D_H__

#include "Graphics/Mesh3D.h"
#include "Utility/Point3D.h"
#include "Utility/Object3D.h"
#include "Items/Ring.h"
#include <list>
#include <vector>

#define SPINS_PER_SEC 2

class Shard : public Object3D {
   public:
      Mesh3D mesh;
      Vector3D rotationVector;
      double rotationSpeed;
      double rotationAmount;
      double scalex, scaley, scalez;
      double worldSize;
      double sizeX, sizeY, sizeZ, collisionRadius;

      Shard(double r, double worldSizeIn);
      virtual ~Shard();
      void InitShard(double r, double worldSizeIn);
      void draw();
      void makeStrip(Ring r1, Ring r2);
      void update(double timeDiff);
      virtual void handleCollision(Object3D* other);
      bool handleHit(std::list<Shard*>& asteroids);
      void drawInMinimap();
      void debug();

   private:
      double randRadius(double r);
      std::vector<Ring> _rList;
      int _nPoints;
};

#endif
