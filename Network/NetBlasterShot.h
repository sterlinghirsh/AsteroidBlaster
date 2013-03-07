
#ifndef __NETBLASTERSHOT_H__
#define __NETBLASTERSHOT_H__

#include "Shots/BlasterShot.h"
#include "Network/NetShot.h"
#include "boost/serialization/base_object.hpp"
#include "boost/serialization/extended_type_info.hpp"
#include "boost/serialization/export.hpp"

struct NetBlasterShot : public NetShot {
   template <class Archive>
    void serialize(Archive & ar, const unsigned int version) {
      ar & boost::serialization::base_object<NetShot>(*this);
   }

   virtual bool toObject(GameState* gameState, Object3D*& item);
   virtual void fromObject(BlasterShot* in);
};

BOOST_CLASS_EXPORT_KEY(NetBlasterShot);

#endif
