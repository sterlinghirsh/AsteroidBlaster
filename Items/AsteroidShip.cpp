/**
 * AsteroidShip.cpp
 * Sterling Hirsh
 * This is the ship that a player plays from or can see.
 */

#include "AsteroidShip.h"
#include <math.h>

#ifndef M_PI
#define M_PI           3.14159265358979323846
#endif

using namespace std;

GLfloat headlight_pos[4] = {0, 0, -1, 0};
GLfloat headlight_amb[4] = {1, 1, 1, 1};
GLfloat headlight_diff[4] = {0.8, 0.8, 0.8, 1.0};
GLfloat headlight_spec[4] = {0.8, 0.8, 0.8, 1.0};

AsteroidShip::AsteroidShip(int headlightIn, double worldSizeIn) : 
 headlight(headlightIn), worldSize(worldSizeIn) {
   position.x = 0;
   position.y = 0;
   position.z = 0;
   brakeFactor = 0;
   forwardAccel = 0;
   yawFactor = 1;
   rollFactor = pitchFactor = 2;
   yawAmount = 0;
   maxSpeed = 20;
   shotSpeed = 40;
   timeOfLastShot = 0;
   timeOfLastBeam = 0;
   lastGunFired = 0;
   shipRadius = 1;
   forward.updateMagnitude(0, 0, 1);
   up.updateMagnitude(0, 1, 0);
   right.updateMagnitude(-1, 0, 0);
   fireShots = fireBeams = false;
}
      
void AsteroidShip::setCamera(bool setPosition) {
   /*
   headlight_pos[0] = -forward.xMag;
   headlight_pos[1] = -forward.yMag;
   headlight_pos[2] = -forward.zMag;
   */
   glPushMatrix();
   glLoadIdentity();
   glLightfv(headlight, GL_POSITION, headlight_pos);
   glPopMatrix();
   if (setPosition) {
      gluLookAt(
       position.x, position.y, position.z,
       position.x + forward.xMag, position.y + forward.yMag, 
         position.z + forward.zMag,
       up.xMag, up.yMag, up.zMag);
   } else {
      gluLookAt(0, 0, 0, forward.xMag, forward.yMag, forward.zMag,
            up.xMag, up.yMag, up.zMag);
   }
}

void AsteroidShip::startYaw(double yawAmountIn) {
   yawAmount = yawAmountIn;
}

void AsteroidShip::noYaw() {
   yawAmount = 0;
}
      
void AsteroidShip::updateAcceleration() {
   acceleration = forward.scalarMultiply(forwardAccel).add(
      right.scalarMultiply(rightAccel).add(
         up.scalarMultiply(upAccel)));
}

void AsteroidShip::noAccel() {
   forwardAccel = 0;
   rightAccel = 0;
   upAccel = 0;
   updateAcceleration();
}
      
void AsteroidShip::brake(double brakeFactorIn) {
   brakeFactor = brakeFactorIn;
}

void AsteroidShip::noBrake() {
   brakeFactor = 0;
}

void AsteroidShip::forwardAcceleration(double newAcc) {
   forwardAccel = newAcc;
   updateAcceleration();
}

void AsteroidShip::rightAcceleration(double newAcc) {
   rightAccel = newAcc;
   updateAcceleration();
}

void AsteroidShip::upAcceleration(double newAcc) {
   upAccel = newAcc;
   updateAcceleration();
}


void AsteroidShip::updatePosition(double timeDiff, 
 double rollAmount, double pitchAmount) {
   updateAcceleration();
   velocity.xMag += timeDiff * acceleration.xMag;
   velocity.yMag += timeDiff * acceleration.yMag;
   velocity.zMag += timeDiff * acceleration.zMag;
   
   velocity.xMag -= velocity.xMag * timeDiff * brakeFactor;
   velocity.yMag -= velocity.yMag * timeDiff * brakeFactor;
   velocity.zMag -= velocity.zMag * timeDiff * brakeFactor;

   double speed = velocity.getLength();
   if (speed > maxSpeed)
      velocity.setLength(maxSpeed);

   position.x += timeDiff * velocity.xMag;
   position.y += timeDiff * velocity.yMag;
   position.z += timeDiff * velocity.zMag;
   // reflect
   if (position.x + shipRadius > worldSize / 2)  velocity.xMag = -fabs(velocity.xMag);
   else if (position.x - shipRadius < -worldSize / 2) velocity.xMag =  fabs(velocity.xMag);
   if (position.y + shipRadius > worldSize / 2)  velocity.yMag = -fabs(velocity.yMag);
   else if (position.y - shipRadius < -worldSize / 2) velocity.yMag =  fabs(velocity.yMag);
   if (position.z + shipRadius > worldSize / 2)  velocity.zMag = -fabs(velocity.zMag);
   else if (position.z - shipRadius < -worldSize / 2) velocity.zMag =  fabs(velocity.zMag);

   roll(timeDiff * rollAmount * pitchFactor);
   pitch(timeDiff * pitchAmount * pitchFactor);
   yaw(timeDiff * yawAmount * yawFactor);

   Vector3D tempShotDir;
   // iterate throught shots
   double curTime = doubleTime();
   for (shotIter = shots.begin(); shotIter != shots.end(); ++shotIter) {
      if ((*shotIter)->timeFired < curTime - (*shotIter)->lifetime) {
         delete *shotIter;
         shotIter = shots.erase(shotIter);
         continue;
      }
      (*shotIter)->updatePosition(timeDiff);
   }
   
   updateShotDirectionVector();
}

/**
 * Yaw the ship.
 * Angle is in radians.
 * We rotate around up.
 */
void AsteroidShip::yaw(double angle) {
   forward.rotate(angle, up);
   right.rotate(angle, up);
}

/**
 * Roll the ship.
 * Angle is in radians.
 * We rotate around forward.
 */
void AsteroidShip::roll(double angle) {
   up.rotate(angle, forward);
   right.rotate(angle, forward);
}

/**
 * Pitch the ship.
 * Angle is in radians.
 * We rotate around right.
 */
void AsteroidShip::pitch(double angle) {
   forward.rotate(-angle, right);
   up.rotate(-angle, right);
}

/**
 * This translates an xOffset and yOffset in world coords to
 * a phi and a theta from the ship.
 */
void AsteroidShip::updateShotDirection(double xOffset, double yOffset) {
   shotPhi = (M_PI/180) * yOffset * VERT_FOV / 2;
   shotTheta = (M_PI/180) * xOffset * VERT_FOV / (/*((double) GW/GH) / */ -2);
   updateShotDirectionVector();
}

void AsteroidShip::keepFiring() {
   if (!(fireShots || fireBeams)) return;
   double curTime = doubleTime();
   Point3D start = position;
   shotDirection.movePoint(start);
   if (fireShots && (timeOfLastShot < curTime - (1 / AsteroidShot::frequency) ||
    timeOfLastShot == 0)) {
      shots.push_back(new AsteroidShot(start,
       shotDirection.scalarMultiply(shotSpeed)));
      timeOfLastShot = curTime;
   }
   if (fireBeams && (timeOfLastBeam < curTime - (1 / AsteroidShotBeam::frequency) ||
    timeOfLastBeam == 0)) {
      shots.push_back(new AsteroidShotBeam(start, shotDirection));
      timeOfLastBeam = curTime;
   }
}

void AsteroidShip::fireLasers(double xOffset, double yOffset, int weapon) {
   switch (weapon) {
      case 0: fireShots = true; break;
      case 1: fireBeams = true; break;
   }
   updateShotDirection(xOffset, yOffset);
}

void AsteroidShip::stopLasers(int weapon) {
   switch (weapon) {
      case 0: fireShots = false; break;
      case 1: fireBeams = false; break;
   }
}


void AsteroidShip::draw() {
   for (shotIter = shots.begin(); shotIter != shots.end(); shotIter++) {
      (*shotIter)->draw();
   }
}

void AsteroidShip::checkAsteroidCollisions(list<Asteroid3D*>& asteroids) {
   double distance;
   list<Asteroid3D*>::iterator asteroid = asteroids.begin();
   for (asteroid = asteroids.begin(); asteroid != asteroids.end(); ++asteroid) {
      // Distance to ship.
      distance = position.distanceFrom((*asteroid)->position);
      if (distance < shipRadius + (*asteroid)->collisionRadius) {
         // TEMP. We should kill the ship.
         asteroid = asteroids.erase(asteroid);
         continue;
      }
      for (shotIter = shots.begin(); shotIter != shots.end(); shotIter++) {
         if ((*shotIter)->checkHit(*asteroid)) {
            if ((*asteroid)->handleHit(asteroids)) {
               delete *asteroid;
               asteroid = asteroids.erase(asteroid);
            }
            if ((*shotIter)->persist)
               continue;
            delete *shotIter;
            shotIter = shots.erase(shotIter);
            break;
         }
      }
   }
}

void AsteroidShip::updateShotDirectionVector() {
   shotDirection = forward;
   shotDirection.rotate(shotPhi, right);
   shotDirection.rotate(shotTheta, up);
}
