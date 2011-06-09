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
      printf("Adding ship with id %d.\n", id);
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

   ship->bankedShards = bankedShards;
   ship->unbankedShards = unbankedShards;

   ship->engineLevel = engineLevel;
   ship->health = health;
   ship->healthMax = healthMax;
   ship->regenHealthLevel = regenHealthLevel;

   ship->color1 = color1;
   ship->color2 = color2;

   ship->isBarrelRollingLeft = isBarrelRollingLeft;
   ship->isBarrelRollingRight = isBarrelRollingRight;

   // Actuall add the members from Object3D.
   item = ship;
   NetObject3D::toObject(gameState, item);
   return itemCreated;
}

void NetShip::fromObject(AsteroidShip* in) {
   NetObject3D::fromObject(in);

   score = in->score;
   kills = in->kills;
   deaths = in->deaths;

   bankedShards = in->bankedShards;
   unbankedShards = in->unbankedShards;

   engineLevel = in->engineLevel;
   health = in->health;
   healthMax = in->healthMax;
   regenHealthLevel = in->regenHealthLevel;

   color1 = in->color1;
   color2 = in->color2;

   isBarrelRollingLeft = in->isBarrelRollingLeft;
   isBarrelRollingRight = in->isBarrelRollingRight;
}


