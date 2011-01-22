/**
 * Camera!
 * Helps you look at stuff.
 * "Why don't you take a picture, it'll last longer."
 *   - Peewee's Big Adventure (And probably a lot of things)
 *
 * Sterling Hirsh / Taylor Arnicar
 * 1-17-11
 * CPE 476
 */

#include "Graphics/Camera.h"

Camera::Camera(bool lockUp) {
   position = new Point3D(0, 0, 0);
   up = new Vector3D(0, 1, 0);
   right = new Vector3D(1, 0, 0);
   forward = new Vector3D(0, 0, 1);
   offset = new Vector3D(0, 0, 0);
   lockUpVector = lockUp;
}

Camera::Camera(Object3D* object) {
   lockUpVector = false;
   offset = new Vector3D(0, 0, 0);
   viewFrom(object);
}

Camera::~Camera() {
   delete position;
   delete up;
   delete right;
   delete forward;
   delete offset;
}

void Camera::setCamera(bool setPosition) {
   if (setPosition) {
      gluLookAt(
       position->x + offset->xMag, 
       position->y + offset->yMag, 
       position->z + offset->zMag,
       position->x + offset->xMag + forward->xMag, 
       position->y + offset->yMag + forward->yMag, 
       position->z + offset->zMag + forward->zMag,
       up->xMag, up->yMag, up->zMag);
   } else {
      gluLookAt(0, 0, 0, forward->xMag, forward->yMag, forward->zMag,
            up->xMag, up->yMag, up->zMag);
   }
}

void Camera::setCamera(Point3D* newPosition, Vector3D* upIn, 
 Vector3D* rightIn, Vector3D* forwardIn) {
   position->x = newPosition->x;
   position->y = newPosition->y;
   position->z = newPosition->z;
   if (!lockUpVector)
      up->updateMagnitude(upIn);
   right->updateMagnitude(rightIn);
   forward->updateMagnitude(forwardIn);
   setCamera(true);
}

/**
 * Bind the camera to some Object3D.
 */
void Camera::viewFrom(Object3D* object) {
   position = object->position;
   up = object->up;
   forward = object->forward;
   right = object->right;
}
