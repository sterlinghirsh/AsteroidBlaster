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
#include "boost/serialization/base_object.hpp"
#include "boost/serialization/extended_type_info.hpp"
#include "boost/serialization/export.hpp"

struct NetShard : public NetObject3D {
   template <class Archive>
    void serialize(Archive & ar, const unsigned int version) {
      ar & boost::serialization::base_object<NetObject3D>(*this);
   }

   virtual void toObject(GameState* gameState, Object3D*& item);
};

BOOST_CLASS_EXPORT_KEY(NetShard);

#endif
