#include "Network/NetShip.h"
#include "Utility/GameState.h"

BOOST_CLASS_EXPORT_IMPLEMENT(NetShip);

bool NetShip::toObject(GameState* gameState, Object3D*& item) {
   item = gameState->custodian[id];
   AsteroidShip* ship;
   bool itemCreated = false;

   if (item == NULL) {
      // Add new shard.
      ship = new AsteroidShip(gameState);
      itemCreated = true;
   } else {
      ship = dynamic_cast<AsteroidShip*>(item);
      if (ship == NULL) {
         std::cout << "Item " << id << " is not an ship, but should be." << std::endl;
         return false;
      }
   }

   ship->score = score;
   ship->kills = kills;
   ship->deaths = deaths;

   ship->nShards = nShards;
   ship->bankedShards = bankedShards;
   ship->curRoundShards = curRoundShards;

   ship->engineUpgrade = engineUpgrade;
   ship->engineMax = engineMax;
   ship->health = health;
   ship->healthMax = healthMax;
   ship->healthUpgradePrice = healthUpgradePrice;
   ship->healthPrice = healthPrice;
   ship->healthAmount = healthAmount;
   ship->healthUpgradeAmount = healthUpgradeAmount;
   ship->regenHealthLevel = regenHealthLevel;
   ship->regenHealthUpgradePrice = regenHealthUpgradePrice;
   ship->regenHealthLevelMax = regenHealthLevelMax;

   ship->color1 = color1;
   ship->color2 = color2;

   // Actuall add the members from Object3D.
   item = ship;
   NetObject3D::toObject(gameState, item);
   return itemCreated;
}

bool NetShip::fromObject(AsteroidShip* in) {
   NetObject3D::fromObject(in);

   score = in->score;
   kills = in->kills;
   deaths = in->deaths;

   nShards = in->nShards;
   bankedShards = in->bankedShards;
   curRoundShards = in->curRoundShards;

   engineUpgrade = in->engineUpgrade;
   engineMax = in->engineMax;
   health = in->health;
   healthMax = in->healthMax;
   healthUpgradePrice = in->healthUpgradePrice;
   healthPrice = in->healthPrice;
   healthAmount = in->healthAmount;
   healthUpgradeAmount = in->healthUpgradeAmount;
   regenHealthLevel = in->regenHealthLevel;
   regenHealthUpgradePrice = in->regenHealthUpgradePrice;
   regenHealthLevelMax = in->regenHealthLevelMax;

   color1 = in->color1;
   color2 = in->color2;
}


