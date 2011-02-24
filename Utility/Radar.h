/**
 * Radar
 * Taylor Arnicar
 * 2-1-2011
 * Every ship will have a radar which it can query to get information about its surroundings.
 */

#ifndef __RADAR_H__
#define __RADAR_H__

#include "Items/AsteroidShip.h"
#include "Utility/ViewFrustum.h"
#include "Utility/GameState.h"
#include <list>

// Incomplete declaration so we can use the pointer.
class AsteroidShip;

class Radar {
   public:
      // Takes in a reference to the ship that constructed it.
      Radar(AsteroidShip* const ship);
      // Deconstructor: tear everything apart.
      virtual ~Radar();
      
      // Provides a complete, unfiltered reading of the whole environment.
      std::list<Object3D*>* getFullReading();
      
      // Provides a filtered reading of the environment based on what's near the owner AsteroidShip.
      virtual std::list<Object3D*>* getNearbyReading(float radius);
      
      // Provides a filtered reading of the environment based on what's within the camera's viewFrustum right now.
      virtual std::list<Object3D*>* getViewFrustumReading();
      
   private:
      // The ship that owns this Radar
      AsteroidShip* owner;
      ViewFrustum* curFrustum;
      // Set up a list iterator to go over a list.
      std::list<Object3D*> :: iterator listIter;
      // Set up a vector iterator to go over a vector.
      std::vector<Object3D*> :: iterator vectorIter;

};

#endif
