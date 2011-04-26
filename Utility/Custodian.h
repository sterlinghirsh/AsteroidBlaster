/**
 * @file
 * Custodian - Stores where all the stuff is.
 * @author Sterling Hirsh
 * @date 1-26-11
 * CPE 476
 */

#ifndef __CUSTODIAN_H__
#define __CUSTODIAN_H__

#include "Items/Drawable.h"
#include "Items/Object3D.h"
#include <vector>
#include <set>
#include <list>

// Incomplete class declaration so we can use pointers.
class Object3D;
class Drawable;
class GameState;

struct CollisionBase;

struct compareByDistance {
   static Drawable* curObject;
   static Vector3D d1;
   static Vector3D d2;
   bool operator() (CollisionBase* const& lhs, CollisionBase* const& rhs) const;
};

class Custodian {
   public:
      void update();
      void add(Drawable* objectIn);
      void remove(Drawable* objectIn);
      std::set<CollisionBase*, compareByDistance>* findCollisions(Drawable* item, bool searchBackwards = false);
      void findAllCollisions();
      std::vector<Drawable*>* getListOfObjects();
      void clear();
      int asteroidCount;
      int shardCount;
      
      Custodian(const GameState* _gameState);

   private:
      std::vector<Drawable*> objectsByMinX;
      std::vector<Drawable*> objectsByMaxX;
      std::list<Drawable*> objectsToAdd;
      const GameState* gameState;
};

#endif
