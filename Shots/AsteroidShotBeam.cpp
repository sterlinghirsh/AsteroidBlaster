/**
 * AsteroidShotBeam.cpp
 * Sterling Hirsh
 * Shot type 2
 */

#include "AsteroidShotBeam.h"
#include <math.h>

materialStruct beamMaterial = {
   {1, 1, 0, .5},
   {1, 1, 0, .5},
   {1, 1, 0, .5},
   {0}
};

materialStruct hitBeamMaterial = {
   {1, .5, 1, .5},
   {1, .5, 1, .5},
   {1, .5, 1, .5},
   {0}
};

materialStruct ballMaterial = {
   {0.3, 0.3, 0.3, 0.5},
   {0.8, 0.8, 0.8, 0.5},
   {0, 0,   0.8, 0.5},
   {8}
};

double AsteroidShotBeam::frequency = .5;

AsteroidShotBeam::AsteroidShotBeam(Point3D& posIn, Vector3D dirIn, AsteroidShip* const ownerIn) :
 AsteroidShot(posIn, dirIn, ownerIn) {
   lifetime = 0.5;
   // In this context, velocity means direction.
   velocity->normalize();
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
   Vector3D axis = zVector.cross(*velocity);
   glPushMatrix();
   glColor3f(1, 0, 0);
   materials(hitYet ? hitBeamMaterial : beamMaterial);
   //glTranslatef(position.x, position.y, position.z);
   position->glTranslate();
   glPushMatrix();
      glRotatef(180 + zVector.getAngleInDegrees(*velocity), 
         axis.xMag, axis.yMag, axis.zMag);
      // It shrinks, probably should fade out.
      drawCylinder(timeLeft * 0.04 , length);
   glPopMatrix();

   // Dots
   Vector3D normal(velocity->getNormalVector());
   materials(ballMaterial);
   for (double distance = 0; distance < length; 
    distance += distanceDifference) {
      glPushMatrix();
      velocity->glTranslate(distance);
      glRotatef(fmod(curTime, 4) * 90 + (distance*angleDiff), velocity->xMag, 
       velocity->yMag, velocity->zMag);
      normal.glTranslate((1 - timeLeft) + ballOffset);
      //glutSolidSphere(0.05 * (1 - timeLeft), 10, 10);
      gluSphere(quadric, 0.05 * (1 - timeLeft), 10,10);
      glPopMatrix();
   }
   glPopMatrix();
}

bool AsteroidShotBeam::checkHit(Object3D* other) {
   if (hitYet && curFrame != lastHitFrame)
      return false;
   Vector3D positionVector(*position);
   Vector3D otherVector(*other->position);

   // otherVector now is how far from the ship it is.
   otherVector.subtractUpdate(positionVector);
   
   double distance = velocity->dot(otherVector);

   // Is it behind me?
   if (distance < 0)
      return false;

   // Is it too far to one side?
   Vector3D normalToDirection(velocity->getNormalVector());
   distance = normalToDirection.dot(otherVector);
   if (distance > other->radius || distance < -other->radius)
      return false;

   /* x1 is ship
      x2 is direction
      x0 is other 
      */

   distance = (velocity->cross(otherVector).getLength() /
         velocity->getLength());
   if(distance < other->radius) {
      hitYet = true;
      lifetime = 0.4;
      timeFired = doubleTime();
      lastHitFrame = curFrame;
      return true;
   }
   // Never return true.
   return false;
}
