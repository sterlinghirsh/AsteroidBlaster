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
class Object3D;
class Drawable;

class Radar {
   public:
      // Takes in a reference to the ship that constructed it.
      Radar(AsteroidShip* const ship);
      // Deconstructor: tear everything apart.
      virtual ~Radar();
      
      /**
       * Provides a filtered reading of the environment based on what's near 
       * the owner AsteroidShip.
       * The distance which objects must be within to be returned by getMinimapReading()
       * This reading does not include Particles of any type!
       */
      virtual std::list<Drawable*>* getMinimapReading(float radius, int& totalItems);
      
      /* Provides a filtered reading of the environment based on what's within the camera's viewFrustum right now.
       */
      virtual std::list<Drawable*>* getViewFrustumReading();
      /* Same as getViewFrustumReading, but only returns targetable objects for the AI. No particles are included.
       */
      virtual std::list<Object3D*>* getTargetableViewFrustumReading();

   private:
      // The ship that owns this Radar
      AsteroidShip* owner;
      Custodian* custodian;
};

// Thrown in here for good measure.
std::list<Drawable*>* getGenericViewFrustumReading(Custodian* custodian, Camera* camera);

#endif
