/**
 * AsteroidShot.cpp
 * Sterling Hirsh
 * A shot in 3D Asteroids.
 */

#include "AsteroidShot.h"
#include <iostream>

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
   //velocity->setLength(40.0);
   timeFired = doubleTime();
   lifetime = 2;
   persist = false;
   minX = minY = minZ = -0.1;
   maxX = maxY = maxZ = 0.1;
   updateBoundingBox();
}

void AsteroidShot::draw() {
   Vector3D zVector(0, 0, -1);
   Vector3D axis = zVector.cross(*velocity);
   glPushMatrix();
   glDisable(GL_LIGHTING);
   glColor3f(1, 0, 0);
   materials(ShotMaterial);
   position->glTranslate();
   glRotatef(zVector.getAngleInDegrees(*velocity), 
      axis.xMag, axis.yMag, axis.zMag);
  // glutSolidSphere(0.05, 30, 10);
   //drawCylinder(0.02, 1);
   //glutSolidCone(0.02, 0.2, 8, 8);
   //gluCylinder(quadric,0.04f,0.0f,0.2f,8,8);
   gluCylinder(quadric,0.02f,0.0f,0.2f,8,8);
   glEnable(GL_LIGHTING);

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
   if (dynamic_cast<Asteroid3D*>(other) != NULL) {
      owner->score += (int)other->radius * 10;
   }
}

/**
 * Don't draw shots in the map.
 * For now :)
 */
void AsteroidShot::drawInMinimap() {
   return;
}

void AsteroidShot::debug() {
   printf("AsteroidShot: (min, max, position, velocity)\n");
   minPosition->print();
   maxPosition->print();
   position->print();
   velocity->print();
   printf("--\n");
}
