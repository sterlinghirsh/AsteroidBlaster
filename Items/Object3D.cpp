/**
 * Object3D
 * Sterling Hirsh / Taylor Arnicar
 * 1-16-11
 * CPE 476
 */

#include "Items/Object3D.h"
#include <math.h>
#include <algorithm>
#include "Utility/Matrix4.h"

Object3D::Object3D(double x, double y, double z, GLuint displayListIn) : Drawable(0, 0, 0, 0) {
   minX = minY = minZ = -0.5;
   maxX = maxY = maxZ =  0.5;
   updateBoundingBox();
   displayList = displayListIn;
   /* Initialize velocity and acceleration to 0. */
   velocity = new Vector3D(0, 0, 0);
   acceleration = new Vector3D(0, 0, 0);
   axis = NULL;
   angle = 0;
   shouldConstrain = true; // True when the bounding space should constrain it.
   // Remove up, right, forward yaw, pitch, roll maybe?
   up = new Vector3D(0, 1, 0);
   right = new Vector3D(1, 0, 0);
   forward = new Vector3D(0, 0, 1);
   lockUpVector = false;
   yawSpeed = pitchSpeed = rollSpeed = 0;
   radius = std::max(fabs(maxX), fabs(minX));
   radius = std::max(radius, fabs(maxY));
   radius = std::max(radius, fabs(minY));
   radius = std::max(radius, fabs(maxZ));
   radius = std::max(radius, fabs(minZ));
   targeted = false;
}

Object3D::~Object3D() {
   if (minPosition != NULL)
      delete minPosition;
   if (maxPosition != NULL)
      delete maxPosition;
   if (velocity != NULL)
      delete velocity;
   if (acceleration != NULL)
      delete acceleration;
   if (up != NULL)
      delete up;
   if (right != NULL)
      delete right;
   if (forward != NULL)
      delete forward;
   if (axis != NULL)
      delete axis;
}

void Object3D::update(double timeDifference) {
   // Do the parent's update.
   Drawable::update(timeDifference);

   updateAcceleration(timeDifference);
   if (acceleration != NULL && velocity != NULL)
      velocity->addUpdate(acceleration->scalarMultiply(timeDifference));

   if (velocity != NULL) {
      while (!instantAccelerations.empty()) {
         if (instantAccelerations.front() != NULL) {
            velocity->addUpdate(*instantAccelerations.front());
            delete instantAccelerations.front();
         }
         instantAccelerations.pop();
      }

   }

   if (velocity != NULL && position != NULL)
      velocity->scalarMultiply(timeDifference).movePoint(*position);
   yaw(yawSpeed * timeDifference);
   roll(rollSpeed * timeDifference);
   pitch(pitchSpeed * timeDifference);
   updateBoundingBox();
}

/**
 * Subclasses can extend this, but this by default does nothing.
 * This should draw anything that needs to glow in the color it should
 * glow, and everything else in black.
 */
void Object3D::drawGlow() {
  glPushMatrix();
  glPopMatrix();
}

/**
 * Subclasses can extend this, but this does some basic movement and will
 * draw a displaylist if one exists.
 */
void Object3D::draw() {
   /*
   glPushMatrix();
   if (position != NULL)
      position->glTranslate();
   if (axis != NULL)
      glRotatef(angle, axis->xMag, axis->yMag, axis->zMag);
   //glCallList(displayList);
   glPopMatrix();
   */
}



/**
 * These three are setters for pitch, roll, yaw.
 * They must be set back to 0 after the pitch is complete.
 */
void Object3D::setYawSpeed(double radiansPerSecond) {
   yawSpeed = radiansPerSecond;
}

void Object3D::setPitchSpeed(double radiansPerSecond) {
   pitchSpeed = radiansPerSecond;
}

void Object3D::setRollSpeed(double radiansPerSecond) {
   rollSpeed = radiansPerSecond;
}

/**
 * Yaw the object.
 * Angle is in radians.
 * We rotate around forward.
 */
void Object3D::yaw(double angle) {
   forward->rotate(angle, *up);
   right->rotate(angle, *up);
}

/**
 * Roll the object.
 * Angle is in radians.
 * We rotate around forward.
 */
void Object3D::roll(double angle) {
   if (!lockUpVector)
      up->rotate(angle, *forward);
   right->rotate(angle, *forward);
}

/**
 * Pitch the object.
 * Angle is in radians.
 * We rotate around right.
 */
void Object3D::pitch(double angle) {
   forward->rotate(-angle, *right);
   if (!lockUpVector)
      up->rotate(-angle, *right);
}


void Object3D::handleCollision(Drawable* other) {
   // Handle stuff in subclasses.
}

void Object3D::drawBoundingBox() {
   glDisable(GL_LIGHTING);
   glPushMatrix();
   glBegin(GL_LINES);
   glVertex3d(minPosition->x, minPosition->y, minPosition->z); glVertex3d(minPosition->x, minPosition->y, maxPosition->z);
   glVertex3d(minPosition->x, maxPosition->y, minPosition->z); glVertex3d(minPosition->x, maxPosition->y, maxPosition->z);
   glVertex3d(maxPosition->x, minPosition->y, minPosition->z); glVertex3d(maxPosition->x, minPosition->y, maxPosition->z);
   glVertex3d(maxPosition->x, maxPosition->y, minPosition->z); glVertex3d(maxPosition->x, maxPosition->y, maxPosition->z);
   
   glVertex3d(minPosition->x, minPosition->y, minPosition->z); glVertex3d(minPosition->x, maxPosition->y, minPosition->z);
   glVertex3d(minPosition->x, minPosition->y, maxPosition->z); glVertex3d(minPosition->x, maxPosition->y, maxPosition->z);
   glVertex3d(maxPosition->x, minPosition->y, minPosition->z); glVertex3d(maxPosition->x, maxPosition->y, minPosition->z);
   glVertex3d(maxPosition->x, minPosition->y, maxPosition->z); glVertex3d(maxPosition->x, maxPosition->y, maxPosition->z);
   
   glVertex3d(minPosition->x, minPosition->y, minPosition->z); glVertex3d(maxPosition->x, minPosition->y, minPosition->z);
   glVertex3d(minPosition->x, minPosition->y, maxPosition->z); glVertex3d(maxPosition->x, minPosition->y, maxPosition->z);
   glVertex3d(minPosition->x, maxPosition->y, minPosition->z); glVertex3d(maxPosition->x, maxPosition->y, minPosition->z);
   glVertex3d(minPosition->x, maxPosition->y, maxPosition->z); glVertex3d(maxPosition->x, maxPosition->y, maxPosition->z);
   glEnd();
   glPopMatrix();
   glEnable(GL_LIGHTING);
}

void Object3D::drawBoundingSphere() {
   glDisable(GL_LIGHTING);
   glPushMatrix();
   GLUquadricObj *q;
   q = gluNewQuadric();
   gluSphere(q, radius, 12, 12);
   glPopMatrix();
   glEnable(GL_LIGHTING);
}

void Object3D::updateBoundingBox() {
   minPosition->clone(position);
   maxPosition->clone(position);
   minPosition->offsetBy(minX, minY, minZ);
   maxPosition->offsetBy(maxX, maxY, maxZ);
}

/**
 * This debug function can be overridden with more specific debug info.
 * This one does not print velocity, since Object3D may have null velocity.
 */
void Object3D::debug() {
   printf("Object3D::debug(): (min/max)\n");
   minPosition->print();
   maxPosition->print();
}

/**
 * This will apply a rotation to move the current up/forward/right vectors
 * so that the item can be drawn with the right orientation.
 * doTranspose defaults to true.
 */
void Object3D::glRotate(bool doTranspose) {
   static Matrix4 modelViewMatrix;
   // Switch to ModelView mode.
   glMatrixMode(GL_MODELVIEW);
   glPushMatrix();
      // Create a new matrix.
      glLoadIdentity();
      // Make the modelview matrix be the rotation vector from the ship's forward and up vectors to the new ones.
      gluLookAt(0, 0, 0, forward->xMag, forward->yMag, forward->zMag,
       up->xMag, up->yMag, up->zMag);
      // Save a copy of the inverse model view matrix.
      modelViewMatrix.loadModelviewMatrix();
      if (doTranspose)
         modelViewMatrix = modelViewMatrix.toTranspose();
   glPopMatrix();
   modelViewMatrix.glMultMatrix();
}

void Object3D::setTargeted(bool a) {
   targeted = a;
}

bool Object3D::isTargeted() {
   return targeted;
}


void Object3D::addAcceleration(Vector3D* newAccel) {
   accelerations.push(newAccel);
}

void Object3D::addInstantAcceleration(Vector3D* newAccel) {
   instantAccelerations.push(newAccel);
}

void Object3D::updateAcceleration(double timeDiff) {
   acceleration->updateMagnitude(0, 0, 0);
   Vector3D* tempAccel;
   while (!accelerations.empty()) {
      tempAccel = accelerations.front();
      acceleration->addUpdate(*tempAccel);
      delete tempAccel;
      accelerations.pop();
   }
}
