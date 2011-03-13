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
#include "Particles/Particle.h"
#include <list>

// Incomplete declaration so we can use the pointer.
class AsteroidShip;

class Radar {
   public:
      // Takes in a reference to the ship that constructed it.
      Radar(AsteroidShip* const ship);
      // Deconstructor: tear everything apart.
      virtual ~Radar();
      
      /**
       * Provides a complete, unfiltered reading of the whole environment.
       * This reading does not include Particles of any type!
       */
      std::list<Drawable*>* getFullReading();
      
      /**
       * Provides a filtered reading of the environment based on what's near 
       * the owner AsteroidShip.
       * The distance which objects must be within to be returned by getNearbyReading()
       * This reading does not include Particles of any type!
       */
      virtual std::list<Drawable*>* getNearbyReading(float radius);
      
      // Provides a filtered reading of the environment based on what's within the camera's viewFrustum right now.
      virtual std::list<Drawable*>* getViewFrustumReading();
      
   private:
      // The ship that owns this Radar
      AsteroidShip* owner;
      ViewFrustum* curFrustum;
      // Set up a list iterator to go over a list.
      std::list<Drawable*> :: iterator listIter;
      // Set up a particle list iterator.
      std::list<Particle*> :: iterator particleIter;
      // Set up a vector iterator to go over a vector.
      std::vector<Drawable*> :: iterator vectorIter;

};

#endif
