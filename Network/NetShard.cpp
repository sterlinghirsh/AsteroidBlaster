#include "Network/NetShard.h"
#include "Utility/GameState.h"

BOOST_CLASS_EXPORT_IMPLEMENT(NetShard);

bool NetShard::toObject(GameState* gameState, Object3D*& item) {
   item = gameState->custodian[id];
   Shard* shard;
   bool itemCreated = false;

   if (item == NULL) {
      // Add new shard.
      shard = new Shard(0.5, gameState->worldSize, gameState);
      itemCreated = true;
   } else {
      shard = dynamic_cast<Shard*>(item);
      if (shard == NULL) {
         std::cout << "Item " << id << " is not a shard, but should be." << std::endl;
         return false;
      }
      // Nothing to add for shards.
   }

   // Actuall add the members from Object3D.
   item = shard;
   NetObject3D::toObject(gameState, item);
   return itemCreated;
}
