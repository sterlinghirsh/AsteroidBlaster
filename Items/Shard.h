/**
 * Chris Brenton
 * Shard.h
 * A randomly generated shard object.
 */

#ifndef __SHARD_H__
#define __SHARD_H__

#include "Utility/GlobalUtility.h"
#include "Utility/Point3D.h"
#include "Items/Object3D.h"
#include <list>
#include <vector>
#include <boost/serialization/base_object.hpp>
#include <boost/serialization/export.hpp>

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
      Vector3D rotationVector;
      double rotationSpeed;
      double rotationAmount;
      double scalex, scaley, scalez;
      double worldSize;
      double sizeX, sizeY, sizeZ, collisionRadius;

      Shard(double r, double worldSizeIn, const GameState* _gameState);
      virtual ~Shard();
      void InitShard(double r, double worldSizeIn);
      void draw();
      void drawShard();
      void update(double timeDiff);
      bool handleHit(std::list<Shard*>& asteroids);
      void drawInMinimap();
      void debug();
      GLuint cubeList;
      float transparentZ;

   private:
      double randRadius(double r);
      int _nPoints;
      void drawOtherOrbiters();
      double orbiterOffset;


   // Serialization
   public:
      template<class Archive> 
            void serialize(Archive & ar, const unsigned int version) {
         std::cout << "lol the classic got to here, shard" << std::endl;
         ar & boost::serialization::base_object<Object3D>(*this);
         ar & rotationVector;
         ar & rotationSpeed;
         ar & rotationAmount;
         ar & scalex;
         ar & scaley;
         ar & scalez;
         ar & worldSize;
         ar & sizeX;
         ar & sizeY;
         ar & sizeZ;
         ar & collisionRadius;
      }
};

//BOOST_CLASS_EXPORT(Shard);

#endif
