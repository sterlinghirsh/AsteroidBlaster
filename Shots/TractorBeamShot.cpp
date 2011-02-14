/**
 * TractorBeamShot.cpp
 * Incredibly attractive shots.
 * Vwahhhh
 * @author Sterling Hirsh (shirsh@calpoly.edu)
 * @date Valentine's Day
 * <3
 */

#include "Shots/TractorBeamShot.h"
#include <math.h>

TractorBeamShot::TractorBeamShot(Point3D& posIn, Vector3D dirIn, 
 AsteroidShip* const ownerIn) : Shot(posIn, dirIn, ownerIn) {
   persist = true;
   angle = M_PI / 10; // Radians from the center
   length = 20;
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
   timeFired = doubleTime();
   shouldConstrain = false;
   updateBoundingBox();
}

/**
 * Kill this after a certain number of frames.
 */
void TractorBeamShot::update(double timeDiff) {
}

/**
 * We want to remove this after drawing it once.
 */
void TractorBeamShot::draw() {
   shouldRemove = true;
   glPushMatrix();
      Point3D start(*position);
      velocity->movePoint(start);
      start.glTranslate();


      glRotate();
      materials(RedFlat);
      gluCylinder(quadric, farRadius, 0, length, 8, 8);
   glPopMatrix();
}
