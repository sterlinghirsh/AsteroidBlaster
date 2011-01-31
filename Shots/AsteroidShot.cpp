/**
 * AsteroidShot.cpp
 * Sterling Hirsh
 * A shot in 3d asteroids.
 */

#include "AsteroidShot.h"

materialStruct ShotMaterial = {
  {1, 0, 0, 1},
  {1, 0, 0, 1},
  {1, 0, 0, 1},
  {8.0}
};

double AsteroidShot::frequency = 10;
      
AsteroidShot::AsteroidShot(Point3D& posIn, Vector3D dirIn,
 AsteroidShip* const ownerIn ) : owner(ownerIn), Object3D(0, 0, 0, 0) {
   *position = posIn;
   velocity = new Vector3D(dirIn);
   timeFired = doubleTime();
   lifetime = 2;
   persist = false;
}

void AsteroidShot::draw() {
   Vector3D zVector(0, 0, -1);
   Vector3D axis = zVector.cross(*velocity);
   glPushMatrix();
   glColor3f(1, 0, 0);
   materials(ShotMaterial);
   position->glTranslate();
   glRotatef(zVector.getAngleInDegrees(*velocity), 
      axis.xMag, axis.yMag, axis.zMag);
  // glutSolidSphere(0.05, 30, 10);
   //drawCylinder(0.02, 1);
   //glutSolidCone(0.02, 0.2, 8, 8);
   gluCylinder(quadric,0.02f,0.0f,0.2f,8,8);

   glPopMatrix();
}

void AsteroidShot::update(double timeDiff) {
   Object3D::update(timeDiff);
   if (doubleTime() - timeFired > lifetime) {
      shouldRemove = true;
   }
}

void AsteroidShot::handleCollision(Object3D* other) {
   if (other == owner)
      return;
   shouldRemove = true;
}
