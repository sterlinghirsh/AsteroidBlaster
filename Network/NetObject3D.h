/**
 * Base class for network objects.
 * Sterling Hirsh
 * 5-17-11
 * CPE 476
 *
 * Here's how this works.
 * This knows about Object3D.h, Object3D.cpp knows about this.
 * This includes all the data from Drawable that we need to send.
 * 
 */

#ifndef __NETOBJECT3D_H__
#define __NETOBJECT3D_H__

#include "Items/Object3D.h"

struct NetObject3D {
   // Object3D stuff
   unsigned id;
   double angle;
   double rotationSpeed;
   double yawSpeed;
   double pitchSpeed;
   double rollSpeed;
   double radius;
   bool shouldRemove; // Maybe...

   Vector3D acceleration;

   // Maybe only include these on a case by case basis, since they're not always used.
   Vector3D axis;
   Vector3D up;
   Vector3D right;
   Vector3D forward;

   // Drawable stuff
   Vector3D velocity;
   Point3D position;
   // How do we handle collisions? Set on client side?
   // Oh wait, we don't need collisions on client side!
   
   NetObject3D() :
    id(0), angle(0), rotationSpeed(0), yawSpeed(0), pitchSpeed(0), rollSpeed(0),
    radius(0), shouldRemove(false), 
    // Vector3Ds
    acceleration(0, 0, 0), axis(0, 0, 0),
    up(0, 1, 0), right(1, 0, 0), forward(0, 0, -1), velocity(0, 0, 0),
    position(0, 0, 0) {}
   
   /**
    * Load values from an object into this.
    */
   void fromObject(Object3D* in) {
      id = in->id;

      angle = in->angle;
      rotationSpeed = in->rotationSpeed;
      yawSpeed = in->yawSpeed;
      pitchSpeed = in->pitchSpeed;
      rollSpeed = in->rollSpeed;
      radius = in->radius;
      shouldRemove = in->shouldRemove;

      if (in->acceleration != NULL) {
         acceleration = *in->acceleration;
      }

      if (in->axis != NULL) {
         axis = *in->axis;
      }

      if (in->up != NULL) {
         up = *in->up;
      }
      
      if (in->right != NULL) {
         right = *in->right;
      }
      
      if (in->forward != NULL) {
         forward = *in->forward;
      }
      
      position = *in->position;
      velocity = *in->velocity;
   }

   virtual bool toObject(GameState* gameState, Object3D*& item) {
      if (item == NULL) {
         std::cout << "Item " << id << " doesn't exist yet. It should have been created earlier." << std::endl;
         return false;
      }

      item->id = id;
      item->radius = radius;
      item->shouldRemove = shouldRemove;
      item->angle = angle;
      item->rotationSpeed = rotationSpeed;
      

      if (item->acceleration != NULL) {
         if (acceleration.getAngleInDegrees(*item->acceleration) >= 1.0) {
            *item->acceleration = acceleration;
         }
      }

      if (item->axis != NULL) {
         if (axis.getAngleInDegrees(*item->axis) >= 1.0) {
            *item->axis = axis;
         }
      }

      if (item->up != NULL) {
         if (up.getAngleInDegrees(*item->up) >= 1.0) {
            *item->up = up;
         }
      }
      
      if (item->right != NULL) {
         if (right.getAngleInDegrees(*item->right) >= 1.0) {
            *item->right = right;
         }
      }
      
      if (item->forward != NULL) {
         if (forward.getAngleInDegrees(*item->forward) >= 1.0) {
            *item->forward = forward;
         }
      }

      if (position.distanceFrom(*item->position) >= 1.0) {
         *item->position = position;
      }

      if (velocity.getAngleInDegrees(*item->velocity) >= 1.0) {
         *item->velocity = velocity;
      }

      return false;
   }

   template <class Archive>
    void serialize(Archive & ar, const unsigned int version) {
      ar & id;

      ar & acceleration;
      ar & axis;
      ar & up;
      ar & right;
      ar & forward;

      ar & angle;
      ar & rotationSpeed;

      ar & yawSpeed;
      ar & pitchSpeed;
      ar & rollSpeed;

      ar & position;
      ar & velocity;
      
      ar & radius;
      ar & shouldRemove;
   }
};

#endif
