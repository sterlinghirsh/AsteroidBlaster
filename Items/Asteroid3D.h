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
#include "Items/Ring.h"
#include <list>
#include <vector>

#define ASTEROID3D_LINE_W 0.5

class AsteroidShip;

class Asteroid3D : public Object3D {
   //public variables------------------------------
   public:
      Mesh3D mesh;
      double scalex, scaley, scalez;
      double worldSize;
      double initH;
      double health;
      double sizeX, sizeY, sizeZ, collisionRadius;
      Vector3D* newAcceleration;
      Vector3D* newVelocity;

      bool energyHitAsteroid;
      double timeLastHitByEnergy;
      double damagePerSecond;

      bool isExploding;
      double timeSinceExplode;
      
      AsteroidShip* lastHitShotOwner;

      CollisionSphere* collisionSphere;
      
      
   //private variables------------------------------
   private:   
      std::vector<Ring> _rList;
      int _nPoints;
      // Holds a random color.
      float cyan, magenta, yellow;
   
   //public functions------------------------------
   public:
      // Constructor
      Asteroid3D(double r, double worldSizeIn, const GameState* _gameState, bool isFirst = false);
      // Destructor
      virtual ~Asteroid3D();
      
      void InitAsteroid(double r, double worldSizeIn);
      void draw();
      void drawGlow();
      void drawEnergyEffect();
      void makeStrip(Ring r1, Ring r2);
      void update(double timeDiff);
      Shard* makeShard(int num);
      Asteroid3D* makeChild(int num, int dimension);
      void newRandomPosition();
      void drawInMinimap();
      void debug();
      
   //private functions------------------------------
   private:
      double randRadius(double r);
      void dropRandomItem();
};

#endif
