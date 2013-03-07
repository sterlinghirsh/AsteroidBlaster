#include "Network/NetBlasterShot.h"
#include "Utility/GameState.h"

BOOST_CLASS_EXPORT_IMPLEMENT(NetBlasterShot);

bool NetBlasterShot::toObject(GameState* gameState, Object3D*& item) {
   item = gameState->custodian[id];
   BlasterShot* blastShot;
   bool itemCreated = false;

   if (item == NULL) {
      // Add new shard.
      AsteroidShip* tempShip = dynamic_cast<AsteroidShip*>(gameState->custodian[owner]);
      if (tempShip == NULL) {
         std::cout << "This owner ID should be an AsteroidShip, but the dynamic cast failed!" << std::endl;
         return false;
      }
      blastShot = new BlasterShot(position, velocity, weaponIndex, tempShip, gameState);
      itemCreated = true;
   } else {
      blastShot = dynamic_cast<BlasterShot*>(item);
      if (blastShot == NULL) {
         std::cout << "Item " << id << " is not an blastShot, but should be." << std::endl;
         return false;
      }
   }

   // Actuall add the members from Object3D.
   item = blastShot;
   NetShot::toObject(gameState, item);
   return itemCreated;
}

void NetBlasterShot::fromObject(BlasterShot* in) {
   NetShot::fromObject(in);
}


