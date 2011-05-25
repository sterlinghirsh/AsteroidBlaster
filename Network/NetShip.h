
#ifndef __NETSHIP_H__
#define __NETSHIP_H__

#include "Items/AsteroidShip.h"
#include "Network/NetObject3D.h"
#include "boost/serialization/base_object.hpp"
#include "boost/serialization/extended_type_info.hpp"
#include "boost/serialization/export.hpp"

struct NetShip : public NetObject3D {
   int score;
   int kills;
   int deaths;

   int nShards;
   int bankedShards;
   int unbankedShards;

   int engineUpgrade;
   int engineMax;
   double health;
   double healthMax;
   int healthUpgradePrice;
   int healthPrice;
   int healthAmount;
   int healthUpgradeAmount;
   int regenHealthLevel;
   int regenHealthUpgradePrice;
   int regenHealthLevelMax;

   float color1;
   float color2;

   template <class Archive>
    void serialize(Archive & ar, const unsigned int version) {
      ar & boost::serialization::base_object<NetObject3D>(*this);
      ar & score;
      ar & kills;
      ar & deaths;

      ar & nShards;
      ar & bankedShards;
      ar & unbankedShards;

      ar & engineUpgrade;
      ar & engineMax;
      ar & health;
      ar & healthMax;
      ar & healthUpgradePrice;
      ar & healthPrice;
      ar & healthAmount;
      ar & healthUpgradeAmount;
      ar & regenHealthLevel;
      ar & regenHealthUpgradePrice;
      ar & regenHealthLevelMax;

      ar & color1;
      ar & color2;

      ar & yawSpeed;
      ar & pitchSpeed;
      ar & rollSpeed;
   }

   virtual bool toObject(GameState* gameState, Object3D*& item);
   virtual void fromObject(AsteroidShip* in);
};

BOOST_CLASS_EXPORT_KEY(NetShip);

#endif
