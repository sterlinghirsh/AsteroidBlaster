/**
 * Chris Brenton
 * Shard.h
 * A randomly generated shard object.
 */

#ifndef __SHARD_H__
#define __SHARD_H__

#include "Graphics/Mesh3D.h"
#include "Utility/GlobalUtility.h"
#include "Utility/Point3D.h"
#include "Items/Object3D.h"
#include "Items/Ring.h"
#include <list>
#include <vector>

#define SPINS_PER_SEC 0.5
#define NUM_ORBITERS 1
#define NUM_ORBIT_RINGS 8
#define ORBITER_RAD (radius / 3.0)
#define RING_RAD (radius * 4.0)
#define ORBIT_RATE -0.5
#define CUBE_MODE 0
#define DECEL_RATE 0.2
#define ORBITER_CLR glColor3f(0.2, 0.5, 0.8)

class Shard : public Object3D {
   public:
      Mesh3D mesh;
      Vector3D rotationVector;
      double rotationSpeed;
      double rotationAmount;
      double scalex, scaley, scalez;
      double worldSize;
      double sizeX, sizeY, sizeZ, collisionRadius;
      GLuint orbiters;

      Shard(double r, double worldSizeIn);
      virtual ~Shard();
      void genOrbiters();
      void InitShard(double r, double worldSizeIn);
      void draw();
      void drawGlow();
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
