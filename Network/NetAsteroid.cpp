#include "Network/NetAsteroid.h"
#include "Utility/GameState.h"

BOOST_CLASS_EXPORT_IMPLEMENT(NetAsteroid);

void NetAsteroid::toObject(GameState* gameState, Object3D*& item) {
   item = gameState->custodian[id];
   Asteroid3D* asteroid;
   
   if (item == NULL) {
      // Add new shard.
      asteroid = new Asteroid3D(6.0, gameState->worldSize, gameState, false);
   } else {
      asteroid = dynamic_cast<Asteroid3D*>(item);
      if (asteroid == NULL) {
         std::cout << "Item " << id << " is not an asteroid, but should be." << std::endl;
         return;
      }
      // Nothing to add for shards.
   }

   // Actuall add the members from Object3D.
   item = asteroid;
   NetObject3D::toObject(gameState, item);
}
