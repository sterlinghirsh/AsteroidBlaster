#include "Network/NetBeamShot.h"
#include "Utility/GameState.h"

BOOST_CLASS_EXPORT_IMPLEMENT(NetBeamShot);

bool NetBeamShot::toObject(GameState* gameState, Object3D*& item) {
   item = gameState->custodian[id];
   BeamShot* beamShot;
   bool itemCreated = false;

   if (item == NULL) {
      // Add new shard.
      AsteroidShip* tempShip = dynamic_cast<AsteroidShip*>(gameState->custodian[owner]);
      if (tempShip == NULL) {
         std::cout << "This owner ID should be an AsteroidShip, but the dynamic cast failed!" << std::endl;
         return false;
      }
      beamShot = new BeamShot(position, velocity, weaponIndex, tempShip, gameState);
      itemCreated = true;
   } else {
      beamShot = dynamic_cast<BeamShot*>(item);
      if (beamShot == NULL) {
         std::cout << "Item " << id << " is not an beamShot, but should be." << std::endl;
         return false;
      }
   }

   beamShot->hitYet = hitYet;
   beamShot->firstFrame = firstFrame;
   beamShot->lastHitFrame = lastHitFrame;

   Object3D* tempObj = gameState->custodian[hitItemID];
   if (tempObj == NULL) {
      std::cout << "hitItem in this NetBeam object is does not have a valid id" << std::endl;
   }

   beamShot->hitItem = tempObj;

   // Actuall add the members from Object3D.
   item = beamShot;
   NetShot::toObject(gameState, item);
   return itemCreated;
}

void NetBeamShot::fromObject(BeamShot* in) {
   NetShot::fromObject(in);

   hitYet = in->hitYet;
   firstFrame = in->firstFrame;
   lastHitFrame = in->lastHitFrame;
   if (in->hitItem == NULL) {
      std::cout << "hmm it's kind of NULL..." << std::endl;
      hitItemID = 0;
   } else {
      hitItemID = in->hitItem->id;
   }

}


