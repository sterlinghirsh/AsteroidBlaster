/**
 * @file
 * Custodian - Stores where all the stuff is.
 * @author Sterling Hirsh
 * @date 1-26-11
 * CPE 476
 */

#ifndef __CUSTODIAN_H__
#define __CUSTODIAN_H__

#include "Utility/Object3D.h"
#include <vector>
#include <set>

class Custodian {
   public:
      void update();
      void add(Object3D* objectIn);
      void remove(Object3D* objectIn);
      std::set<Object3D*>* findCollisions(Object3D* item, bool searchBackwards = false);
      void findAllCollisions();
      std::vector<Object3D*>* getListOfObjects();

   private:
      std::vector<Object3D*> objectsByMinX;
      std::vector<Object3D*> objectsByMaxX;
};

#endif
