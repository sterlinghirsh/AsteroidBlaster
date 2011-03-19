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
   shakeAmount = 0;
   zoomFactor = 1.0;
}

Camera::Camera(Object3D* object) {
   lockUpVector = false;
   shakeAmount = 0;
   offset = new Vector3D(0, 0, 0);
   viewFrom(object);
   zoomFactor = 1.0;
}

Camera::~Camera() {
   // Position, up, right, forward deleted by the Object3D's destructor
   delete offset;
}

void Camera::setCamera(bool setPosition) {
   Vector3D random;
   random.randomMagnitude();
   offset->scalarMultiplyUpdate(zoomFactor);
   random.setLength(shakeAmount / 50);
   if (setPosition) {
      gluLookAt(
       position->x + offset->xMag, 
       position->y + offset->yMag, 
       position->z + offset->zMag,
       position->x + offset->xMag + forward->xMag + random.xMag, 
       position->y + offset->yMag + forward->yMag + random.yMag, 
       position->z + offset->zMag + forward->zMag + random.zMag,
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

/**
 * Set offset.
 */
void Camera::setOffset(double x, double y, double z) {
   offset->updateMagnitude(x, y, z);
   //offset->scalarMultiplyUpdate(zoomFactor);
}

void Camera::setOffset(Vector3D& newOffset) {
   offset->updateMagnitude(newOffset);
   //offset->scalarMultiplyUpdate(zoomFactor);
}

void Camera::setViewVector(Vector3D* newView) {
   forward = newView;
}

Point3D Camera::getEyePoint(){
   Point3D eyePoint(*position);
   offset->movePoint(eyePoint);
   //offset->scalarMultiplyUpdate(zoomFactor);
   return eyePoint;
}

void Camera::shake(float newShakeAmount) {
   shakeAmount = (float) clamp(newShakeAmount, 0, 1);
}

void Camera::zoom() {
   zoomFactor = (float) ((((int) (zoomFactor / 2)) + 1) % 2) + 1.0f;
}
