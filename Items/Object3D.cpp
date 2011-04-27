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

Object3D::Object3D(const GameState* _gameState) : Drawable(_gameState) {
   minX = minY = minZ = -0.5;
   maxX = maxY = maxZ =  0.5;
   updateBoundingBox();
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
   rotationSpeed = 0;
}

Object3D::~Object3D() {
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
   angle += rotationSpeed * timeDifference;

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
      gluLookAt(0, 0, 0, forward->x, forward->y, forward->z,
       up->x, up->y, up->z);
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

void Object3D::draw() {
   // Do nothing;
}

void Object3D::drawGlow() {
   // Also do nothing;
}

/**
 * Treats the current object3D as a sphere. 
 * Calculates collision with some ray (like a railgun shot or electricity shot).
 * If you pass in a pointer to a double, this sets that to the distance from
 *  origin to collision point.
 */
Point3D* Object3D::sphereCollideWithRay(const Point3D& origin, Vector3D direction, double* hitDistance) {
   direction.normalize();
   Point3D* collisionPoint = NULL;
   
   // Use the quadratic formula with magic.
   Vector3D rayOriginToObjectLocation(*position, origin);
   double quadratic_a = direction.dot(direction);
   double quadratic_b = 2 * direction.dot(rayOriginToObjectLocation);
   double quadratic_c = rayOriginToObjectLocation.dot(rayOriginToObjectLocation) - (radius * radius);

   double discriminant = (quadratic_b * quadratic_b) - (4 * quadratic_a * quadratic_c);
   
   // We don't want to continue if we're going to deal with imaginary numbers.
   if (discriminant < 0) {
      return NULL;
   }

   double root1 = ((-quadratic_b) + sqrt(discriminant)) / (2 * quadratic_a);
   double root2 = ((-quadratic_b) - sqrt(discriminant)) / (2 * quadratic_a);

   // We don't want any points that are behind the origin.
   if (root1 < 0) {
      if (root2 < 0) {
         return NULL;
      }
      root1 = root2;
   } else if (root2 < 0) {
      root2 = root1;
   }

   double minRoot = root1 < root2 ? root1 : root2;

   collisionPoint = new Point3D(origin);
   direction.movePoint(*collisionPoint, minRoot);

   if (hitDistance != NULL) {
      *hitDistance = minRoot;
   }

   return collisionPoint;
}

void Object3D::nullPointers() {
   Drawable::nullPointers();
   acceleration = NULL;
   up = NULL;
   right = NULL;
   forward = NULL;
   axis = NULL;
}
