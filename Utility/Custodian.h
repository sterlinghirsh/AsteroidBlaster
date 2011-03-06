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

// Incomplete class declaration so we can use pointers.
class Object3D;

struct compareByDistance {
   static Object3D* curObject;
   static Vector3D d1;
   static Vector3D d2;
   bool operator() (Object3D* const& lhs, Object3D* const& rhs) const;
};

class Custodian {
   public:
      void update();
      void add(Object3D* objectIn);
      void remove(Object3D* objectIn);
      std::set<Object3D*, compareByDistance>* findCollisions(Object3D* item, bool searchBackwards = false);
      void findAllCollisions();
      std::vector<Object3D*>* getListOfObjects();
      void clear();
      int asteroidCount;
      
      Custodian();

   private:
      std::vector<Object3D*> objectsByMinX;
      std::vector<Object3D*> objectsByMaxX;
      std::list<Object3D*> objectsToAdd;
};

#endif
