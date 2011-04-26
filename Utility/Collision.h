/**
 * Sterling Hirsh
 * 4/25/11
 * This describes a collision between two objects.
 */

#include "Items/Drawable.h"
#include "Utility/CollisionTypes.h"

// Include all collideable objects.

template <class A, class B>
struct Collision {
   A* a;
   B* b;
   double squaredDistance;
   
   // Default constructor.
   Collision() : a(NULL), b(NULL), squaredDistance(0) {}

   // Constructor when a collision is found.
   Collision(A* _a, B* _b) {
    a(_a), b(_b), relationship(_relationship) {
      Vector3D tmp(*a->position, *b->position);
      squaredDistance = a->getComparisonLength();
   }

   Collision* tryExcecute(Drawable* _a, Drawable* _b) {
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
         toReturn = new Collision<A, B>(aResult, bResult, this);
      }

      return toReturn;
   }

   virtual void handleCollision(A *_a, B *_b) = 0;
};

struct ShipAsteroidRelationship : public CollisionRelationship<AsteroidShip, Asteroid3D> {
   void handleCollision(AsteroidShip* ship, Asteroid3D* asteroid) {
      // Handle asteroid-ship collision.
      printf("ship hit asteroid.");
   }
}

// Instantiate elsewhere in a .cpp file.
//CollisionRelationship<Asteroid3D, AsteroidShip> asteroidToShipCollision;
