
#ifndef __NETBEAMSHOT_H__
#define __NETBEAMSHOT_H__

#include "Shots/BeamShot.h"
#include "Network/NetShot.h"
#include "boost/serialization/base_object.hpp"
#include "boost/serialization/extended_type_info.hpp"
#include "boost/serialization/export.hpp"

struct NetBeamShot : public NetShot {
   bool hitYet;
   unsigned long firstFrame;
   unsigned long lastHitFrame;
   unsigned hitItemID;

   template <class Archive>
    void serialize(Archive & ar, const unsigned int version) {
      ar & boost::serialization::base_object<NetShot>(*this);
      ar & hitYet;
      ar & firstFrame;
      ar & lastHitFrame;
      ar & hitItemID;
   }

   virtual bool toObject(GameState* gameState, Object3D*& item);
   virtual void fromObject(BeamShot* in);
};

BOOST_CLASS_EXPORT_KEY(NetBeamShot);

#endif
