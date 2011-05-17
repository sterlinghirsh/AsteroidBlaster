/**
 * Network object for Shard
 * Sterling Hirsh
 * 5-17-11
 * CPE 476
 */

#ifndef __NETSHARD_H__
#define __NETSHARD_H__

#include "Items/Shard.h"
#include "Network/NetObject3D.h"
#include <boost/serialization/export.hpp>
#include <boost/serialization/base_object.hpp>

struct NetShard : public NetObject3D {
   template <class Archive>
    void serialize(Archive & ar, const unsigned int version) {
      ar & boost::serialization::base_object<NetObject3D>(*this);
   }

   virtual void toObject(GameState* gameState, Object3D*& item) {
      item = gameState->custodian[id];
      Shard* shard;
      
      if (item == NULL) {
         // Add new shard.
         shard = new Shard(0.5, gameState->worldSize, gameState);
      } else {
         shard = dynamic_cast<Shard*>(item);
         if (shard == NULL) {
            std::cout << "Item " << id << " is not a shard, but should be." << std::endl;
            return;
         }
         // Nothing to add for shards.
      }

      // Actuall add the members from Object3D.
      item = shard;
      NetObject3D::toObject(gameState, item);
   }
};

BOOST_CLASS_EXPORT(NetShard);

#endif
