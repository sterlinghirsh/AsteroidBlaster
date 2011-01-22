/**
 * Object3D
 * Sterling Hirsh / Taylor Arnicar
 * 1-16-22
 * CPE 476
 */

#include "Utility/Object3D.h"

Object3D::Object3D(double x, double y, double z, GLuint displayListIn) {
   minX = minY = minZ = -0.5;
   maxX = maxY = maxZ =  0.5;
   displayList = displayListIn;
   position = new Point3D(x, y, z);
   velocity = NULL;
   acceleration = NULL;
   axis = NULL;
   angle = 0;
   // Remove up, right, forward yaw, pitch, roll maybe?
   up = new Vector3D(0, 1, 0);
   right = new Vector3D(1, 0, 0);
   forward = new Vector3D(0, 0, 1);
   lockUpVector = false;
   yawSpeed = pitchSpeed = rollSpeed = 0;
   setCorners(false);
}

Object3D::~Object3D() {
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
   for (int i = 0; i < 8; ++i)
      delete corners[i];
}

void Object3D::setCorners(bool deleteFirst) {
   if (deleteFirst) {
      for (int i = 0; i < 8; ++i)
         delete corners[i];
   }
   corners[0] = new Point3D(minX, minY, minZ);
   corners[1] = new Point3D(minX, minY, maxZ);
   corners[2] = new Point3D(minX, maxY, minZ);
   corners[3] = new Point3D(minX, maxY, maxZ);
   corners[4] = new Point3D(maxX, minY, minZ);
   corners[5] = new Point3D(maxX, minY, maxZ);
   corners[6] = new Point3D(maxX, maxY, minZ);
   corners[7] = new Point3D(maxX, maxY, maxZ);
}

void Object3D::update(double timeDifference) {
   if (acceleration != NULL && velocity != NULL)
      velocity->addUpdate(acceleration->scalarMultiply(timeDifference));
   if (velocity != NULL && position != NULL)
      velocity->scalarMultiply(timeDifference).movePoint(*position);
   yaw(yawSpeed * timeDifference);
   roll(rollSpeed * timeDifference);
   pitch(pitchSpeed * timeDifference);
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
   // Add position to min/max.
   double tempMinX = minX + position->x;
   double tempMaxX = maxX + position->x;
   double tempMinY = minY + position->y;
   double tempMaxY = maxY + position->y;
   double tempMinZ = minZ + position->z;
   double tempMaxZ = maxZ + position->z;
   // The other object's bounding box boundries
   double otherTempMinX = other->minX + other->position->x;
   double otherTempMaxX = other->maxX + other->position->x;
   double otherTempMinY = other->minY + other->position->y;
   double otherTempMaxY = other->maxY + other->position->y;
   double otherTempMinZ = other->minZ + other->position->z;
   double otherTempMaxZ = other->maxZ + other->position->z;

   // Whether or not there's collisions in each of the dimensions
   bool xIntersect = (tempMinX >= otherTempMinX && tempMinX <= otherTempMaxX) ||
                     (tempMaxX >= otherTempMinX && tempMaxX <= otherTempMaxX);
   bool yIntersect = (tempMinY >= otherTempMinY && tempMinY <= otherTempMaxY) ||
                     (tempMaxY >= otherTempMinY && tempMaxY <= otherTempMaxY);
   bool zIntersect = (tempMinZ >= otherTempMinZ && tempMinZ <= otherTempMaxZ) ||
                     (tempMaxZ >= otherTempMinZ && tempMaxZ <= otherTempMaxZ);
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
   position->glTranslate();
   glBegin(GL_LINES);
   glVertex3f(minX, minY, minZ); glVertex3f(minX, minY, maxZ);
   glVertex3f(minX, maxY, minZ); glVertex3f(minX, maxY, maxZ);
   glVertex3f(maxX, minY, minZ); glVertex3f(maxX, minY, maxZ);
   glVertex3f(maxX, maxY, minZ); glVertex3f(maxX, maxY, maxZ);
   
   glVertex3f(minX, minY, minZ); glVertex3f(minX, maxY, minZ);
   glVertex3f(minX, minY, maxZ); glVertex3f(minX, maxY, maxZ);
   glVertex3f(maxX, minY, minZ); glVertex3f(maxX, maxY, minZ);
   glVertex3f(maxX, minY, maxZ); glVertex3f(maxX, maxY, maxZ);
   
   glVertex3f(minX, minY, minZ); glVertex3f(maxX, minY, minZ);
   glVertex3f(minX, minY, maxZ); glVertex3f(maxX, minY, maxZ);
   glVertex3f(minX, maxY, minZ); glVertex3f(maxX, maxY, minZ);
   glVertex3f(minX, maxY, maxZ); glVertex3f(maxX, maxY, maxZ);
   glEnd();
   glPopMatrix();
   glEnable(GL_LIGHTING);
}
