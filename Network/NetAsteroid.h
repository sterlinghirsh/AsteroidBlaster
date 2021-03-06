
#ifndef __NETASTEROID_H__
#define __NETASTEROID_H__

#include "Items/Asteroid3D.h"
#include "Network/NetObject3D.h"
#include "boost/serialization/base_object.hpp"
#include "boost/serialization/extended_type_info.hpp"
#include "boost/serialization/export.hpp"

struct NetAsteroid : public NetObject3D {
   double health;
   float cyan;
   float magenta;
   float yellow;

   template <class Archive>
    void serialize(Archive & ar, const unsigned int version) {
      ar & boost::serialization::base_object<NetObject3D>(*this);
      ar & health;
      ar & cyan;
      ar & magenta;
      ar & yellow;
   }

   virtual bool toObject(GameState* gameState, Object3D*& item);
   virtual void fromObject(Asteroid3D* in);
};

BOOST_CLASS_EXPORT_KEY(NetAsteroid);

#endif
