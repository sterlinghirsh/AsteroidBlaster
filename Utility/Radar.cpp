/**
 * Radar
 * Taylor Arnicar
 * 2-1-2011
 * Every ship will have a radar which it can query to get information about its surroundings.
 */

#include "Radar.h"
#include "Utility/GameState.h"
//#include "Utility/Object3D.h"
//#include "Utility/Vector3D.h"


Radar :: Radar(AsteroidShip* const ship) {
   owner = ship;
}

// Deconstructor shouldn't need to do anything
Radar :: ~Radar() {
   // Do nothing
}

// Provides a complete, unfiltered reading of the whole environment.
std::vector<Object3D*>* Radar :: getFullReading() {
   // Tell c++ that gameState was declared elsewhere (in main.cpp)
   extern GameState* gameState;
   
   /* Get the custodian out of gameState, and get its vector of Objects.
    * Use this vector to construct a new copied vector (to be safe), and return that one.
    */
   std::vector<Object3D*>* tmp (gameState->custodian.getListOfObjects());
   return tmp;
}

// Provides a filtered reading of the environment based on what's near the owner AsteroidShip.
std::vector<Object3D*>* Radar :: getNearbyReading() {
   // Tell c++ that gameState was declared elsewhere (in main.cpp)
   extern GameState* gameState;
   
   /* Get the custodian out of gameState, and get its vector of Objects.
    * Use this vector to construct a new copied vector (to be safe), and return that one.
    */
   std::vector<Object3D*>* tmp (gameState->custodian.getListOfObjects());
   return tmp;
}
