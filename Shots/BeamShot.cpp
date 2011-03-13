/**
 * BeamShot.cpp
 * Sterling Hirsh
 * Shot type 2
 */

#include "BeamShot.h"
#include <math.h>
#include <algorithm>

materialStruct beamMaterial = {
   {1, 1, 0, .8},
   {1, 1, 0, .8},
   {1, 1, 0, .8},
   {0}
};

materialStruct hitBeamMaterial = {
   {1, .5, 1, .8},
   {1, .5, 1, .8},
   {1, .5, 1, .8},
   {0}
};

materialStruct ballMaterial = {
   {0.3, 0.3, 0.3, 0.8},
   {0.8, 0.8, 0.8, 0.8},
   {0, 0,   0.8, 0.8},
   {8}
};

BeamShot::BeamShot(Point3D& posIn, Vector3D dirIn, AsteroidShip* const ownerIn) :
   Shot(posIn, dirIn, ownerIn) {
      lifetime = 0.5;
      // In this context, velocity means direction.
      hitYet = false;
      hitItem = NULL; // We use this to make sure it hits only one thing.
      persist = true;
      lastHitFrame = 0;
      firstFrame = curFrame;
      Point3D endPoint1(*position);
      Point3D endPoint2(*position);
      // Set endPoint2 100 units away.
      velocity->setLength(100);
      velocity->movePoint(endPoint2);
      velocity->normalize();
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
      /* Make sure beam shots aren't culled from the view frustum (necessary to make them appear)
      */
      shouldBeCulled = false;
      updateBoundingBox();
   }

/**
 * Expire after a certain amount of time.
 */
void BeamShot::update(double timeDiff) {
   if (!gameState->godMode) {

      if (doubleTime() - timeFired > lifetime) {
         shouldRemove = true;
      }
   } else {
      if (doubleTime() - timeFired > 0.1) {
         shouldRemove = true;
      }
   }
}

void BeamShot::drawBeam(bool drawDots) {
   const double length = 100;
   const double distanceDifference = 0.3; // :)
   const double ballOffset = 0.2;
   const double angleDiff = 40; // Degrees per unit of ball helix.
   double curTime = doubleTime();
   double timeLeft;
   if (!gameState->godMode) {
      timeLeft = lifetime - (curTime - timeFired);
   } else {
      timeLeft = 1.4 - (curTime - timeFired);
   }
   Vector3D zVector(0, 0, -1);
   Vector3D axis = zVector.cross(*velocity);
   glPushMatrix();
   glColor3f(1, 0, 0);
   setMaterial(hitYet ? hitBeamMaterial : beamMaterial);
   position->glTranslate();
   glPushMatrix();
   glRotatef(180 + zVector.getAngleInDegrees(*velocity),
         axis.xMag, axis.yMag, axis.zMag);
   // It shrinks, probably should fade out.
   if (!gameState->godMode) {
      drawCylinder(timeLeft * 0.04 , length);
   } else {
      drawCylinder(0.1 , length);
   }
   glPopMatrix();
   if (drawDots) {
      if (!gameState->godMode) {
         // Dots
         Vector3D normal(velocity->getNormalVector());
         setMaterial(ballMaterial);
         for (double distance = 0; distance < length;
               distance += distanceDifference) {
            glPushMatrix();
            velocity->glTranslate(distance);
            glRotatef(fmod(curTime, 4) * 90 + (distance*angleDiff), velocity->xMag,
                  velocity->yMag, velocity->zMag);
            normal.glTranslate((1 - timeLeft) + ballOffset);
            //glutSolidSphere(0.05 * (1 - timeLeft), 10, 10);
            gluSphere(quadric, 0.05 * (1 - timeLeft), 5,5);
            glPopMatrix();
         }
      }
   }
   glPopMatrix();
}

void BeamShot::draw() {
   drawBeam(true);
}

void BeamShot::drawGlow() {
   drawBeam(false);
}

/**
 * This does the actual beam-weapon hit detection.
 * This ignores checkOther, since the beam will be the final say on what gets hit.
 */
bool BeamShot::detectCollision(Drawable* other, bool checkOther) {
   if (hitYet || curFrame != firstFrame)
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
   if (fabs(distance) > other->radius)
      return false;

   /* x1 is ship
      x2 is direction
      x0 is other
      */

   distance = (velocity->cross(otherVector).getLength() /
         velocity->getLength());
   return fabs(distance) < other->radius;
}

void BeamShot::handleCollision(Drawable* other) {
   //printf("Distance: %f\n", position->distanceFrom(*other->position));
   if (other == owner || hitYet || (curFrame - 1) > firstFrame)
      return;
   // Only count hits on Asteroids and Shards.
   // TODO: Add ship.
   if (dynamic_cast<Asteroid3D*>(other) == NULL && 
    dynamic_cast<Shard*>(other) == NULL &&
    dynamic_cast<AsteroidShip*>(other) == NULL)
      return;
   hitYet = true;
   hitItem = other;
   lifetime = 0.4;
   timeFired = doubleTime();
   lastHitFrame = curFrame;
   if (dynamic_cast<Asteroid3D*>(other) != NULL) {
      owner->score += (int)other->radius * 10;
   }
}

void BeamShot::debug() {
   printf("BeamShot::debug(): (min/max/position/direction)\n");
   minPosition->print();
   maxPosition->print();
   position->print();
   velocity->print();
}

/**
 * Don't draw shots in minimap (for now)
 */
void BeamShot::drawInMinimap() {
   return;
}
