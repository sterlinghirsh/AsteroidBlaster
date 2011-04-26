/**
 * Sterling Hirsh
 * 4/25/11
 * This describes a collision between two objects.
 */

#include "Items/Drawable.h"
#include "Utility/CollisionTypes.h"
#include "Items/AsteroidShip.h"
#include "Items/Asteroid3D.h"

#ifndef __COLLISION_H__
#define __COLLISION_H__

// Include all collideable objects.
struct CollisionBase {
   double squaredDistance;
   CollisionBase() : squaredDistance(0) {}
   CollisionBase(double _squaredDistance) : squaredDistance(_squaredDistance) {}
   virtual CollisionBase* tryExecute(Drawable* _a, Drawable* _b) =0;
   virtual void handleCollision() = 0;
};

template <class A, class B>
struct Collision : public CollisionBase {
   A* a;
   B* b;
   
   // Default constructor.
   Collision() : CollisionBase(0), a(NULL), b(NULL)  {}

   // Constructor when a collision is found.
   Collision(A* _a, B* _b) :
    a(_a), b(_b) {
      Vector3D tmp(*a->position, *b->position);
      squaredDistance = tmp.getComparisonLength();
   }

   virtual Collision<A, B>* tryExecute(Drawable* _a, Drawable* _b) {
      Collision* toReturn = NULL;;
      A *aResult = dynamic_cast<A *>(_a);
      B *bResult = dynamic_cast<B *>(_b);

      bool canExecute = false;

      // Did the cast work?
      if (aResult != NULL && bResult != NULL) {
         canExecute = true;
      } else {
         aResult = dynamic_cast<A *>(_b);
         bResult = dynamic_cast<B *>(_a);
         canExecute = aResult != NULL && bResult != NULL;
      }

      if (canExecute && detectCollision(aResult, bResult)) {
         toReturn = new Collision<A, B>(aResult, bResult);
      }

      return toReturn;
   }

   virtual void handleCollision();
};

// Instantiate elsewhere in a .cpp file.
//CollisionRelationship<Asteroid3D, AsteroidShip> asteroidToShipCollision;

#endif
