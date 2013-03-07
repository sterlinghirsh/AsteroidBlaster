
#ifndef __NETTRACTORBEAMSHOT_H__
#define __NETTRACTORBEAMSHOT_H__

#include "Shots/TractorBeamShot.h"
#include "Network/NetShot.h"
#include "boost/serialization/base_object.hpp"
#include "boost/serialization/extended_type_info.hpp"
#include "boost/serialization/export.hpp"

struct NetTractorBeamShot : public NetShot {

   template <class Archive>
    void serialize(Archive & ar, const unsigned int version) {
      ar & boost::serialization::base_object<NetShot>(*this);
   }

   virtual bool toObject(GameState* gameState, Object3D*& item);
   virtual void fromObject(TractorBeamShot* in);
};

BOOST_CLASS_EXPORT_KEY(NetTractorBeamShot);

#endif
