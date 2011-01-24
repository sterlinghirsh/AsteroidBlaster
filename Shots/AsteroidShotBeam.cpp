/**
 * AsteroidShotBeam.cpp
 * Sterling Hirsh
 * Shot type 2
 */

#include "AsteroidShotBeam.h"
#include <math.h>

materialStruct beamMaterial = {
   {1, 1, 0, .5},
   {0, 0, 0, .5},
   {0, 0, 0, .5},
   {0}
};

materialStruct hitBeamMaterial = {
   {1, .5, 1, .5},
   {0, 0, 0, .5},
   {0, 0, 0, .5},
   {0}
};

materialStruct ballMaterial = {
   {0.3, 0.3, 0.3, 0.5},
   {0.8, 0.8, 0.8, 0.5},
   {0, 0,   0.8, 0.5},
   {8}
};

double AsteroidShotBeam::frequency = .5;

AsteroidShotBeam::AsteroidShotBeam(Point3D posIn, Vector3D dirIn) :
 AsteroidShot(posIn, dirIn) {
   lifetime = 0.5;
   direction.normalize();
   hitYet = false;
   persist = true;
   lastHitFrame = 0;
}

void AsteroidShotBeam::draw() {
   const int length = 100;
   const double distanceDifference = 0.3; // :)
   const double angleDiff = 40; // Degrees per unit of ball helix.
   const double ballOffset = 0.2;
   double curTime =  doubleTime();
   double timeLeft = lifetime - (curTime - timeFired);
   Vector3D zVector(0, 0, -1);
   Vector3D axis = zVector.cross(direction);
   glPushMatrix();
   glColor3f(1, 0, 0);
   materials(hitYet ? hitBeamMaterial : beamMaterial);
   //glTranslatef(position.x, position.y, position.z);
   position.glTranslate();
   glPushMatrix();
      glRotatef(180 + zVector.getAngleInDegrees(direction), 
         axis.xMag, axis.yMag, axis.zMag);
      // It shrinks, probably should fade out.
      drawCylinder(timeLeft * 0.04 , length);
   glPopMatrix();

   // Dots
   Vector3D normal(direction.getNormalVector());
   materials(ballMaterial);
   for (double distance = 0; distance < length; 
    distance += distanceDifference) {
      glPushMatrix();
      direction.glTranslate(distance);
      glRotatef(fmod(curTime, 4) * 90 + (distance*angleDiff), direction.xMag, 
       direction.yMag, direction.zMag);
      normal.glTranslate((1 - timeLeft) + ballOffset);
      //glutSolidSphere(0.05 * (1 - timeLeft), 10, 10);
      gluSphere(quadric, 0.05 * (1 - timeLeft), 10,10);
      glPopMatrix();
   }
   glPopMatrix();
}

void AsteroidShotBeam::updatePosition(double timeDiff) {
   // Do nothing.
}

bool AsteroidShotBeam::checkHit(Asteroid3D* asteroid) {
   if (hitYet && curFrame != lastHitFrame)
      return false;
   Vector3D positionVector(position);
   Vector3D asteroidVector(*asteroid->position);

   // asteroidVector now is how far from the ship it is.
   asteroidVector.subtractUpdate(positionVector);
   
   double distance = direction.dot(asteroidVector);

   // Is it behind me?
   if (distance < 0)
      return false;

   // Is it too far to one side?
   Vector3D normalToDirection(direction.getNormalVector());
   distance = normalToDirection.dot(asteroidVector);
   if (distance > asteroid->radius || distance < -asteroid->radius)
      return false;

   /* x1 is ship
      x2 is direction
      x0 is asteroid 
      */

   distance = (direction.cross(asteroidVector).getLength() /
         direction.getLength());
   if(distance < asteroid->radius) {
      hitYet = true;
      lifetime = 0.4;
      timeFired = doubleTime();
      lastHitFrame = curFrame;
      return true;
   }
   // Never return true.
   return false;
}
