#include "Items/Spring.h"
#include "Items/AsteroidShip.h"

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
   if (isAttached) {
      Vector3D displace = (anchor->position)->add(*anchor->getCameraOffset());
      Vector3D springVector = displace.subtract(*item->position);
      double length = springVector.getLength();
      if (length == 0.0) { return; }
      double normalLength = POS_SPRING_LEN;
      double forceScalar = (length - normalLength) / normalLength;
      springVector.scalarMultiplyUpdate(1.0 / length);
      Vector3D forceVector = springVector.scalarMultiply(forceScalar);
      forceVector.scalarMultiplyUpdate(ms * POS_FORCE_SCALE);
      item->position->addUpdate(forceVector);
   }
   // Up vector
   if (isAttached) {
      Vector3D displace = *anchor->up;
      Vector3D springVector = displace.subtract(*item->up);
      double length = springVector.getLength();
      if (length == 0.0) { return; }
      double normalLength = UP_SPRING_LEN;
      double forceScalar = (length - normalLength) / normalLength;
      springVector.scalarMultiplyUpdate(1.0 / length);
      Vector3D forceVector = springVector.scalarMultiply(forceScalar);
      forceVector.scalarMultiplyUpdate(ms * UP_FORCE_SCALE);
      item->up->addUpdate(forceVector);
   }
}
