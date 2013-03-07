

#ifndef __NETSHOT_H__
#define __NETSHOT_H__

#include "Shots/Shot.h"
#include "Network/NetObject3D.h"
#include "boost/serialization/base_object.hpp"
#include "boost/serialization/extended_type_info.hpp"
#include "boost/serialization/export.hpp"

struct NetShot : public NetObject3D {
   unsigned owner;
   double timeFired;
   double lifetime;
   double damage;
   int weaponIndex;
   
   NetShot() :
    owner(0), timeFired(0), lifetime(0), damage(0), weaponIndex(0) {}
   
   /**
    * Load values from an object into this.
    */
   void fromObject(Shot* in) {
      NetObject3D::fromObject(in);
      owner = in->owner->id;
      timeFired = in->timeFired;
      lifetime = in->lifetime;
      damage = in->damage;
      weaponIndex = in->weaponIndex;
   }

   virtual bool toObject(GameState* gameState, Object3D*& item) {
      Shot* tempShot;
      if (item == NULL) {
         std::cout << "Item " << id << " doesn't exist yet. It should have been created earlier." << std::endl;
         return false;
      } else {
         tempShot = dynamic_cast<Shot*>(item);
         if (tempShot == NULL) {
            std::cout << "Item " << id << " is not a Shot, but should be." << std::endl;
            return false;
         }
      }

      AsteroidShip* tempShip = dynamic_cast<AsteroidShip*>(gameState->custodian[owner]);
      if (tempShip == NULL) {
         std::cout << "This owner ID should be an AsteroidShip, but the dynamic cast failed!" << std::endl;
         return false;
      }

      tempShot->owner = tempShip;
      tempShot->timeFired = timeFired;
      tempShot->lifetime = lifetime;
      tempShot->damage = damage;
      tempShot->weaponIndex = weaponIndex;

      return false;
   }

   template <class Archive>
    void serialize(Archive & ar, const unsigned int version) {
      ar & boost::serialization::base_object<NetObject3D>(*this);
      ar & owner;
      ar & timeFired;
      ar & lifetime;
      ar & damage;
      ar & weaponIndex;
   }
};

BOOST_CLASS_EXPORT_KEY(NetShot);

#endif
