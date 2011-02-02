/**
 * Radar
 * Taylor Arnicar
 * 2-1-2011
 * Every ship will have a radar which it can query to get information about its surroundings.
 */

#include "Radar.h"
//#include "Utility/Object3D.h"
//#include "Utility/Vector3D.h"

Radar :: Radar(AsteroidShip* const ship) {
   owner = ship;
}

Radar :: ~Radar() {
   // Do nothing
}

std::vector<Object3D*>* Radar :: getFullReading() {
   std::vector<Object3D*>* tmp = new std::vector<Object3D*>;
   return tmp;
}
