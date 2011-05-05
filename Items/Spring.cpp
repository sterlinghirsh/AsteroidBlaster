/**
 * A spring. Connects a camera to a ship, but it can be restructured to
 * use two Object3D's, or really pretty much any two objects.
 * @author Chris Brenton
 * @date 4/19/11
 */
#include "Items/Spring.h"
#include "Items/AsteroidShip.h"

// The scaling factor of the forces (higher is stronger).
#define POS_FORCE_SCALE 15.0
#define UP_FORCE_SCALE 15.0

Spring::Spring(const GameState* _gameState) :
   Object3D(_gameState) {
      isAttached = false;
   }

void Spring::attach(AsteroidShip* anchorIn, Camera* itemIn) {
   anchor = anchorIn;
   item = itemIn;
   isAttached = true;
}

void Spring::update(double ms) {
   // Connect the positions with a spring force.
   if (isAttached) {
      Vector3D displace = (anchor->position)->add(*anchor->getCameraOffset());
      Vector3D springVector = displace.subtract(*item->position);
      double length = springVector.getLength();
      if (length <= 0.001 && length >= -0.001) { return; }
      Vector3D forceVector = springVector.scalarMultiply(ms * POS_FORCE_SCALE);
      item->position->addUpdate(forceVector);
   }
   // Connect the up vectors with a spring force.
   if (isAttached) {
      Vector3D displace = *anchor->up;
      Vector3D springVector = displace.subtract(*item->up);
      double length = springVector.getLength();
      if (length <= 0.001 && length >= -0.001) { return; }
      Vector3D forceVector = springVector.scalarMultiply(ms * UP_FORCE_SCALE);
      item->up->addUpdate(forceVector);
      /*
      double forceScalar = (length - normalLength) / normalLength;
      springVector.scalarMultiplyUpdate(1.0 / length);
      Vector3D forceVector = springVector.scalarMultiply(forceScalar);
      forceVector.scalarMultiplyUpdate(ms * UP_FORCE_SCALE);
      item->up->addUpdate(forceVector);
      */
   }
}
