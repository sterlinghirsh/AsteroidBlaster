/**
 * @file
 * Custodian - Stores where all the stuff is.
 * @author Sterling Hirsh
 * @date 1-26-11
 * CPE 476
 */

#ifndef __CUSTODIAN_H__
#define __CUSTODIAN_H__

#include "Items/Object3D.h"
#include <vector>
#include <set>
#include <list>
#include <map>

// Incomplete class declaration so we can use pointers.
class Object3D;
class GameState;
class Asteroid3D;
class AsteroidShip;
class Shard;
class Shot;

struct CollisionBase;

struct compareByDistance {
   static Object3D* curObject;
   static Vector3D d1;
   static Vector3D d2;
   bool operator() (CollisionBase* const& lhs, CollisionBase* const& rhs) const;
};

namespace ast {
   class Entity;
}

class Custodian {
   //public variables------------------------------
   public:
      // mapping of client id to ship ID
      std::map<unsigned, AsteroidShip*> shipsByClientID;

      std::set<Asteroid3D*> asteroids;
      std::set<Shard*> shards;
      std::set<Shot*> shots;
      std::set<AsteroidShip*> ships;
      Object3D* updateObjectFromEntity(const ast::Entity& ent);

   //private variables------------------------------
   private:
      unsigned nextID;
      std::vector<Object3D*> objectsByMinX;
      std::vector<Object3D*> objectsByMaxX;
      std::map<unsigned, Object3D*> objectsByID;
      std::list<Object3D*> objectsToAdd;
      const GameState* gameState;


   //public functions------------------------------
   public:
      Custodian(const GameState* _gameState);

      Object3D* operator[] (unsigned i);

      void cleanup();
      void update();
      void add(Object3D* objectIn);
      CollisionBase* getCollision(Object3D* a, Object3D* b);
      std::set<CollisionBase*, compareByDistance>* findCollisions(Object3D* item, bool searchBackwards = false);
      void findAllCollisions();
      std::vector<Object3D*>* getListOfObjects();
      void clear();
      int asteroidCount;
      int shardCount;
      unsigned getNextID();
      const std::map<unsigned, Object3D*>& getObjectsByID();
      


   //private functions------------------------------
   private:
      void remove(Object3D* objectIn);
};

#endif
