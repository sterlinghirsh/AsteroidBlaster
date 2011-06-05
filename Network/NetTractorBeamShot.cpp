#include "Network/NetTractorBeamShot.h"
#include "Utility/GameState.h"

BOOST_CLASS_EXPORT_IMPLEMENT(NetTractorBeamShot);

bool NetTractorBeamShot::toObject(GameState* gameState, Object3D*& item) {
   item = gameState->custodian[id];
   TractorBeamShot* tractBeamShot;
   bool itemCreated = false;

   if (item == NULL) {
      // Add new shard.
      AsteroidShip* tempShip = dynamic_cast<AsteroidShip*>(gameState->custodian[owner]);
      if (tempShip == NULL) {
         std::cout << "This owner ID should be an AsteroidShip, but the dynamic cast failed!" << std::endl;
         return false;
      }
      tractBeamShot = new TractorBeamShot(position, velocity, weaponIndex, tempShip, gameState);
      itemCreated = true;
   } else {
      tractBeamShot = dynamic_cast<TractorBeamShot*>(item);
      if (tractBeamShot == NULL) {
         std::cout << "Item " << id << " is not an beamShot, but should be." << std::endl;
         return false;
      }
   }

   // Actuall add the members from Object3D.
   item = tractBeamShot;
   NetShot::toObject(gameState, item);
   return itemCreated;
}

void NetTractorBeamShot::fromObject(TractorBeamShot* in) {
   NetShot::fromObject(in);
}


