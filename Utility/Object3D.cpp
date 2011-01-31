/**
 * Object3D
 * Sterling Hirsh / Taylor Arnicar
 * 1-16-22
 * CPE 476
 */

#include "Utility/Object3D.h"

Object3D::Object3D(double x, double y, double z, GLuint displayListIn) :
 position(new Point3D(x, y, z)) {
   minX = minY = minZ = -0.5;
   maxX = maxY = maxZ =  0.5;
   minPosition = new Point3D(0, 0, 0);
   maxPosition = new Point3D(0, 0, 0);
   minXRank = maxXRank = 0;
   updateBoundingBox();
   displayList = displayListIn;
   velocity = NULL;
   acceleration = NULL;
   axis = NULL;
   angle = 0;
   shouldRemove = false;
   // Remove up, right, forward yaw, pitch, roll maybe?
   up = new Vector3D(0, 1, 0);
   right = new Vector3D(1, 0, 0);
   forward = new Vector3D(0, 0, 1);
   lockUpVector = false;
   yawSpeed = pitchSpeed = rollSpeed = 0;
}

Object3D::~Object3D() {
   if (minPosition != NULL)
      delete minPosition;
   if (maxPosition != NULL)
      delete maxPosition;
   if (position != NULL)
      delete position;
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
   if (acceleration != NULL && velocity != NULL)
      velocity->addUpdate(acceleration->scalarMultiply(timeDifference));
   if (velocity != NULL && position != NULL)
      velocity->scalarMultiply(timeDifference).movePoint(*position);
   yaw(yawSpeed * timeDifference);
   roll(rollSpeed * timeDifference);
   pitch(pitchSpeed * timeDifference);
   updateBoundingBox();
}

void Object3D::draw() {
   glPushMatrix();
   if (position != NULL)
      glTranslatef(position->x, position->y, position->z);
   if (axis != NULL)
      glRotatef(angle, axis->xMag, axis->yMag, axis->zMag);
   glCallList(displayList);
   glPopMatrix();
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


/**
 * checkOther is set to true by default in Object3D.h.
 * If this doesn't detect a collision, then checkOther specifies if we should use other's 
 * collision detection function.
 */
bool Object3D::detectCollision(Object3D* other, bool checkOther) {
   // Do checks here.

   // Whether or not there's collisions in each of the dimensions
   bool xIntersect = (minPosition->x >= other->minPosition->x &&
                      minPosition->x <= other->maxPosition->x) ||
                     (maxPosition->x >= other->maxPosition->x &&
                      maxPosition->x <= other->maxPosition->x);
   bool yIntersect = (minPosition->y >= other->minPosition->y &&
                      minPosition->y <= other->maxPosition->y) ||
                     (maxPosition->y >= other->maxPosition->y &&
                      maxPosition->y <= other->maxPosition->y);
   bool zIntersect = (minPosition->z >= other->minPosition->z &&
                      minPosition->z <= other->maxPosition->z) ||
                     (maxPosition->z >= other->maxPosition->z &&
                      maxPosition->z <= other->maxPosition->z);

   /* If there was any sort of collision, handle it. Else if we should be checking the other's
      collision detection, do so and return that result. Otherwise, all collision detection failed.
    */
   if (xIntersect && yIntersect && zIntersect) {
      handleCollision(other);
      other->handleCollision(this);
      return true;
   } else if (checkOther) {
      return other->detectCollision(this, false);
   } else {
      return false;
   }
}

void Object3D::handleCollision(Object3D* other) {
   // Handle stuff in subclasses.
}

void Object3D::drawBoundingBox() {
   glDisable(GL_LIGHTING);
   glPushMatrix();
   glBegin(GL_LINES);
   glVertex3f(minPosition->x, minPosition->y, minPosition->z); glVertex3f(minPosition->x, minPosition->y, maxPosition->z);
   glVertex3f(minPosition->x, maxPosition->y, minPosition->z); glVertex3f(minPosition->x, maxPosition->y, maxPosition->z);
   glVertex3f(maxPosition->x, minPosition->y, minPosition->z); glVertex3f(maxPosition->x, minPosition->y, maxPosition->z);
   glVertex3f(maxPosition->x, maxPosition->y, minPosition->z); glVertex3f(maxPosition->x, maxPosition->y, maxPosition->z);
   
   glVertex3f(minPosition->x, minPosition->y, minPosition->z); glVertex3f(minPosition->x, maxPosition->y, minPosition->z);
   glVertex3f(minPosition->x, minPosition->y, maxPosition->z); glVertex3f(minPosition->x, maxPosition->y, maxPosition->z);
   glVertex3f(maxPosition->x, minPosition->y, minPosition->z); glVertex3f(maxPosition->x, maxPosition->y, minPosition->z);
   glVertex3f(maxPosition->x, minPosition->y, maxPosition->z); glVertex3f(maxPosition->x, maxPosition->y, maxPosition->z);
   
   glVertex3f(minPosition->x, minPosition->y, minPosition->z); glVertex3f(maxPosition->x, minPosition->y, minPosition->z);
   glVertex3f(minPosition->x, minPosition->y, maxPosition->z); glVertex3f(maxPosition->x, minPosition->y, maxPosition->z);
   glVertex3f(minPosition->x, maxPosition->y, minPosition->z); glVertex3f(maxPosition->x, maxPosition->y, minPosition->z);
   glVertex3f(minPosition->x, maxPosition->y, maxPosition->z); glVertex3f(maxPosition->x, maxPosition->y, maxPosition->z);
   glEnd();
   glPopMatrix();
   glEnable(GL_LIGHTING);
}

void Object3D::updateBoundingBox() {
   minPosition->clone(position);
   maxPosition->clone(position);
   minPosition->offsetBy(minX, minY, minZ);
   maxPosition->offsetBy(maxX, maxY, maxZ);
}
