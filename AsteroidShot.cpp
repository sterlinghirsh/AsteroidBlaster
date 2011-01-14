/**
 * AsteroidShot.cpp
 * Sterling Hirsh
 * A shot in 3d asteroids.
 */

#include "AsteroidShot.h"

materialStruct ShotMaterial = {
  {1, 0, 0, .8},
  {0, 0, 0, .8},
  {0, 0, 0, .8},
  {8.0}
};

double AsteroidShot::frequency = 10;
      
AsteroidShot::AsteroidShot(Point3D posIn, Vector3D dirIn) :
 position(posIn), direction(dirIn) {
   timeFired = doubleTime();
   lifetime = 2;
   persist = false;
}

void AsteroidShot::draw() {
   Vector3D zVector(0, 0, -1);
   Vector3D axis = zVector.cross(direction);
   glPushMatrix();
   glColor3f(1, 0, 0);
   materials(ShotMaterial);
   glTranslatef(position.x, position.y, position.z);
   glRotatef(zVector.getAngleInDegrees(direction), 
      axis.xMag, axis.yMag, axis.zMag);
  // glutSolidSphere(0.05, 30, 10);
   //drawCylinder(0.02, 1);
   glutSolidCone(0.02, 0.2, 8, 8);

   glPopMatrix();
}

void AsteroidShot::updatePosition(double timeDiff) {
   position.x += direction.xMag * timeDiff;
   position.y += direction.yMag * timeDiff;
   position.z += direction.zMag * timeDiff;
}

bool AsteroidShot::checkHit(Asteroid3D* asteroid) {
   double distance = position.distanceFrom(asteroid->position);
   return distance < asteroid->collisionRadius;
}
