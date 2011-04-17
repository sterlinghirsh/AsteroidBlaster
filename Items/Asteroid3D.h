/**
 * Sterling Hirsh
 * Asteroid3D.h
 * A randomly generated asteroid object.
 */

#ifndef __ASTEROID3D_H__
#define __ASTEROID3D_H__

#include "Graphics/Mesh3D.h"
#include "Utility/Point3D.h"
#include "Items/Object3D.h"
#include "Items/Shard.h"
#include <list>
#include <vector>

#define ASTEROID3D_LINE_W 0.5

class Asteroid3D : public Object3D {
   public:
      Mesh3D mesh;
      Vector3D rotationVector;
      double rotationSpeed;
      double rotationAmount;
      double scalex, scaley, scalez;
      double worldSize;
      double initH;
      double health;
      double sizeX, sizeY, sizeZ, collisionRadius;
      double timeSinceExplode;
      bool isExploding;
      
      Asteroid3D(double r, double worldSizeIn, const GameState* _gameState, bool isFirst = false);
      virtual ~Asteroid3D();
      void InitAsteroid(double r, double worldSizeIn);
      void draw();
      void drawGlow();
      void makeStrip(Ring r1, Ring r2);
      void update(double timeDiff);
      virtual void handleCollision(Drawable* other);
      bool handleHit(std::list<Asteroid3D*>& asteroids);
      Shard* makeShard(int num);
      Asteroid3D* makeChild(int num, int dimension);
      void newRandomPosition();
      void drawInMinimap();
      void debug();

   private:
      double randRadius(double r);
      std::vector<Ring> _rList;
      int _nPoints;
      // Holds a random color.
      float cyan, magenta, yellow;
      void dropRandomItem();
};

#endif
