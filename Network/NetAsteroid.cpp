#include "Network/NetAsteroid.h"
#include "Utility/GameState.h"

BOOST_CLASS_EXPORT_IMPLEMENT(NetAsteroid);

bool NetAsteroid::toObject(GameState* gameState, Object3D*& item) {
   item = gameState->custodian[id];
   Asteroid3D* asteroid;
   bool itemCreated = false;

   if (item == NULL) {
      // Add new shard.
      asteroid = new Asteroid3D(radius, gameState->worldSize, gameState, false);
      itemCreated = true;
   } else {
      asteroid = dynamic_cast<Asteroid3D*>(item);
      if (asteroid == NULL) {
         std::cout << "Item " << id << " is not an asteroid, but should be." << std::endl;
         return false;
      }
   }

   asteroid->health = health;
   asteroid->cyan = cyan;
   asteroid->magenta = magenta;
   asteroid->yellow = yellow;

   // Actuall add the members from Object3D.
   item = asteroid;
   NetObject3D::toObject(gameState, item);
   return itemCreated;
}
