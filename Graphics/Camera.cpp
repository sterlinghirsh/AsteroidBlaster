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

#define STEREO_OFFSET_AMOUNT 0.2
#include "Graphics/Camera.h"

Camera::Camera(bool lockUp) {
   position = new Point3D(0, 0, 0);
   velocity = new Vector3D(0, 0, 0);
   up = new Vector3D(0, 1, 0);
   right = new Vector3D(1, 0, 0);
   forward = new Vector3D(0, 0, 1);
   offset = new Vector3D(0, 0, 0);
   lockUpVector = lockUp;
   shakeAmount = 0;
   // Stereo stuff
   converganceDistance = 20.0;
   stereoOffsetAmount = STEREO_OFFSET_AMOUNT;
}

Camera::Camera(Object3D* object) {
   position = new Point3D(0, 0, 0);
   velocity = new Vector3D(0, 0, 0);
   lockUpVector = false;
   shakeAmount = 0;
   offset = new Vector3D(0, 0, 0);
   up = new Vector3D(0, 1, 0);
   viewFrom(object);
   converganceDistance = 80.0;
   stereoOffsetAmount = STEREO_OFFSET_AMOUNT;
}

Camera::~Camera() {
   // Position, up, right, forward deleted by the Object3D's destructor
   delete offset;
}

void Camera::setCamera(bool setPosition) {
   Vector3D random;
   random.randomMagnitude();
   random.setLength(shakeAmount / 50);
   
   Vector3D stereoOffset;
   if (drawStereo_enabled) {
      stereoOffset = forward->cross(*up);
      if (stereo_eye_left)
         stereoOffset.scalarMultiplyUpdate(-stereoOffsetAmount);
      else
         stereoOffset.scalarMultiplyUpdate(stereoOffsetAmount);
   }

   if (setPosition) {
      gluLookAt(
            position->x + offset->x + stereoOffset.x,
            position->y + offset->y + stereoOffset.y,
            position->z + offset->z + stereoOffset.z,
            position->x + offset->x + forward->x * converganceDistance + 
             random.x + stereoOffset.x,
            position->y + offset->y + forward->y * converganceDistance + 
             random.y + stereoOffset.y,
            position->z + offset->z + forward->z * converganceDistance + 
             random.z + stereoOffset.z,
            up->x, up->y, up->z);
   } else {
      gluLookAt(0, 0, 0, forward->x, forward->y, forward->z,
            up->x, up->y, up->z);
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
   if (!cameraFollow) {
      position = object->position;
      velocity->clone(object->velocity);
      up = object->up;
   }
   forward = object->forward;
   right = object->right;
}

/**
 * Set offset.
 */
void Camera::setOffset(double x, double y, double z) {
   offset->updateMagnitude(x, y, z);
}

void Camera::setOffset(Vector3D& newOffset) {
   offset->updateMagnitude(newOffset);
}

void Camera::setViewVector(Vector3D* newView) {
   forward = newView;
}

Point3D Camera::getEyePoint(){
   Point3D eyePoint(*position);
   offset->movePoint(eyePoint);
   return eyePoint;
}

void Camera::shake(float newShakeAmount) {
   shakeAmount = (float) clamp(newShakeAmount, 0, 1);
}

void Camera::lookAt(double x, double y, double z) {
   forward->x = x - position->x;
   forward->y = y - position->y;
   forward->z = z - position->z;
   forward->scalarMultiply(-1);
   forward->normalize();
   up->x = 0;
   up->y = 1;
   up->z = 0;
   right->x = 1;
   right->y = 0;
   right->z = 0;
}




