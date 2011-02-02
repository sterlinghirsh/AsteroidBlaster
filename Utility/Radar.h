/**
 * Radar
 * Taylor Arnicar
 * 2-1-2011
 * Every ship will have a radar which it can query to get information about its surroundings.
 */

#ifndef __RADAR_H__
#define __RADAR_H__

#include "Items/AsteroidShip.h"

// Incomplete declaration so we can use the pointer.
class AsteroidShip;

class Radar {
   public:
      // Takes in a reference to the ship that constructed it.
      Radar(AsteroidShip* const ship);
      // Deconstructor: tear everything apart.
      virtual ~Radar();
      
      // Provides a full, unfiltered reading of the whole environment.
      std::vector<Object3D*>* getFullReading();
      
      // Provides a filtered reading of the environment based on what's near the owner AsteroidShip.
      //virtual std::vector<Object3D*> getNearbyReading();
      
   private:
      // The ship that owns this Radar
      AsteroidShip* owner;
      
   protected:

};

#endif
