/**
 * TractorBeamShot.cpp
 * Incredibly attractive shots.
 * Vwahhhh
 * @author Sterling Hirsh (shirsh@calpoly.edu)
 * @date Valentine's Day
 * <3
 */

#include "Items/Shard.h"
#include "Shots/TractorBeamShot.h"
#include <math.h>

static float spin = 0;

TractorBeamShot::TractorBeamShot(Point3D& posIn, Vector3D dirIn, 
 AsteroidShip* const ownerIn) : Shot(posIn, dirIn, ownerIn) {
   persist = true;
   angle = M_PI / 20; // Radians from the center
   length = 40;
   farRadius = length * tan(angle);
   framesAlive = 0;
   forward = new Vector3D(*velocity); // Goofy.
   up = new Vector3D(0, 1, 0);
   Point3D endPoint1(*position);
   Point3D endPoint2(*position);
   // Set endPoint2 100 units away.
   velocity->setLength(length);
   velocity->movePoint(endPoint2);
   // Correct for position when calculating endpoint1 and 2.
   Vector3D positionVector(*position);
   positionVector = positionVector.scalarMultiply(-1);
   positionVector.movePoint(endPoint1);
   positionVector.movePoint(endPoint2);
   // Now set min/max xyz
   minX = std::min(endPoint1.x, endPoint2.x);
   maxX = std::max(endPoint1.x, endPoint2.x);
   minY = std::min(endPoint1.y, endPoint2.y);
   maxY = std::max(endPoint1.y, endPoint2.y);
   minZ = std::min(endPoint1.z, endPoint2.z);
   maxZ = std::max(endPoint1.z, endPoint2.z);
   
   // Expand the bounding box to make sure it contains the whole cone.
   minX -= farRadius;
   minY -= farRadius;
   minZ -= farRadius;
   maxX += farRadius;
   maxY += farRadius;
   maxZ += farRadius;

   timeFired = doubleTime();
   shouldConstrain = false;
   /* Make sure tractor beam shots aren't culled from the view frustum (necessary to make them appear)
    */
   shouldBeCulled = false;
   updateBoundingBox();
}

/**
 * Kill this after a certain number of frames.
 */
void TractorBeamShot::update(double timeDiff) {
   if (++framesAlive >= 2)
      shouldRemove = true;
}

/**
 * We want to remove this after drawing it once.
 */
void TractorBeamShot::draw() {
   
   glUseProgram(shader1);
   glPushMatrix();
      
      float lpos[4] = {1.0, 0.5, 1.0, 0.0};	// light postion
      //glLightfv(GL_LIGHT0, GL_POSITION, lpos);
      Point3D start(*position);
      velocity->movePoint(start);
      start.glTranslate();
      
      glRotate();
      glRotatef(spin++,0,0,1);
      if (spin >= 360 ) {
         spin = 0;
      }
      //materials(RedFlat);
      //glutWireTeapot(1.0);
      //gluSphere(quadric,1.0, 10 , 10);
      //gluCylinder(quadric, farRadius, 0, length, 8, 8);
      glutWireCone(farRadius, length,20,20);
   glPopMatrix();
   glUseProgram(0);
}

/**
 * Is the object in the cone?
 */
bool TractorBeamShot::detectCollision(Object3D* other, bool checkOther) {
   if (other == owner)
      return false;
   
   // This is how far it is for position to other->position.
   Vector3D shotToTarget(*position, *other->position);
   
   Vector3D tempDirection(*velocity);
   tempDirection.normalize();

   double forwardDistance = tempDirection.dot(shotToTarget);
   // Is it behind me?
   if (forwardDistance < 0) {
      return false;
   }

   if (forwardDistance > length) {
      return false;
   }

   double requiredDistance = forwardDistance * tan(angle);
   Point3D closestPoint(*position);
   tempDirection.movePoint(closestPoint, forwardDistance);
   return (other->position->distanceFrom(closestPoint) <=
    (other->radius + requiredDistance));
}

void TractorBeamShot::handleCollision(Object3D* other) {
   if (other == owner)
      return;
}
