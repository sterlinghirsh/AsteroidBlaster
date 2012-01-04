/**
 * A spring. Connects a camera to a ship, but it can be restructured to
 * use two Object3D's, or really pretty much any two objects.
 * @author Chris Brenton
 * @date 4/19/11
 */
#include "Items/Spring.h"
#include "Items/AsteroidShip.h"

// DEBUG
#include <fstream>
extern std::ofstream debugoutput;

// The scaling factor of the forces (higher is stronger).
#define POS_FORCE_SCALE 15.0
#define UP_FORCE_SCALE 15.0

Spring::Spring(const GameState* _gameState) :
   Object3D(_gameState),
   positionOffset(0, 0, 0) {
      isAttached = false;
   }

void Spring::attach(AsteroidShip* anchorIn, Camera* cameraIn) {
   anchor = anchorIn;
   camera = cameraIn;
   isAttached = true;
}

void Spring::update(double timeDiff) {
   // Connect the positions with a spring force.
   bool firstPerson = anchor->getCurrentView() == VIEW_FIRSTPERSON_SHIP;

   if (isAttached) {
      /*
      if (firstPerson) {
         camera->position->updateMagnitude((anchor->position)->add(*anchor->getCameraOffset()));
      } else {
         // Sterling's method 2 (Smoothest)
         Vector3D targetPosition((anchor->position)->add(*anchor->getCameraOffset()));
         targetPosition.addUpdate(camera->up->scalarMultiply(2.5));

         Vector3D positionDiff = targetPosition - *camera->position;
         positionDiff.scalarMultiplyUpdate(timeDiff * 2);
         
         positionOffset.addUpdate(positionDiff);

         *camera->position = targetPosition + positionOffset;
      }
      */
         // Sterling's method 1
      if (firstPerson) {
         camera->position->updateMagnitude((anchor->position)->add(*anchor->getCameraOffset()));
      } else {
         Vector3D targetVelocity = ((anchor->position)->add(*anchor->getCameraOffset()) - *camera->position);
         targetVelocity.addUpdate(camera->up->scalarMultiply(2.5));
         targetVelocity.scalarMultiplyUpdate(POS_FORCE_SCALE);
         // *camera->velocity = targetVelocity;
         *camera->velocity = anchor->velocity->lerp(targetVelocity, 0.5);

         camera->velocity->movePoint(*camera->position, timeDiff);
      }

      /*
         // Chris's method
      if (firstPerson) {
         camera->position->updateMagnitude(displace);
      } else {
         displace.addUpdate(camera->up->scalarMultiply(2.5));
         Vector3D springVector = displace.subtract(*camera->position);
         double length = springVector.getLength();
         if (length >= 0.001 || length <= -0.001) {
            Vector3D forceVector = springVector.scalarMultiply(timeDiff * POS_FORCE_SCALE);
            camera->position->addUpdate(forceVector);
         }
      }
      */
   }
   // Connect the up vectors with a spring force.
   if (isAttached && (firstPerson ||
    ((anchor->isBarrelRollingLeft <= 0) && (anchor->isBarrelRollingRight <= 0)))) {
         
      // Sterling's method 2
      /*
         Vector3D targetUp(anchor->up);

         Vector3D upDiff = targetUp - *camera->up;
         upDiff.scalarMultiplyUpdate(timeDiff * 2);
         
         upOffset.addUpdate(upDiff);

         *camera->up = (targetUp + upOffset).getNormalized();
         */

         // Sterling's method
      Vector3D displace = *anchor->up;
      Vector3D springVector = displace.subtract(*camera->up);
      double length = springVector.getLength();
      if (length >= 0.001 || length <= -0.001) {
         Vector3D forceVector = springVector.scalarMultiply(timeDiff * UP_FORCE_SCALE);
         camera->up->addUpdate(forceVector);
      }
      /*
         // Chris's method

      double forceScalar = (length - normalLength) / normalLength;
      springVector.scalarMultiplyUpdate(1.0 / length);
      Vector3D forceVector = springVector.scalarMultiply(forceScalar);
      forceVector.scalarMultiplyUpdate(timeDiff * UP_FORCE_SCALE);
      camera->up->addUpdate(forceVector);
      */
   }
}
