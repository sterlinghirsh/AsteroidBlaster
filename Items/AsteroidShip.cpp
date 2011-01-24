/**
 * AsteroidShip.cpp
 * Sterling Hirsh
 * This is the ship that a player plays from or can see.
 */

#include "AsteroidShip.h"
#include <math.h>
#include <time.h>

#ifndef M_PI
#define M_PI           3.14159265358979323846
#endif

using namespace std;

GLfloat headlight_pos[4] = {0, 0, -1, 0};
GLfloat headlight_amb[4] = {1, 1, 1, 1};
GLfloat headlight_diff[4] = {0.8, 0.8, 0.8, 1.0};
GLfloat headlight_spec[4] = {0.8, 0.8, 0.8, 1.0};


AsteroidShip::AsteroidShip(int headlightIn, double worldSizeIn) : 
 Object3D(0, 0, 0, 0),
 shotDirection(0, 0, 1), 
 headlight(headlightIn), 
 worldSize(worldSizeIn) {
   velocity = new Vector3D(0, 0, 0);
   acceleration = new Vector3D(0, 0, 0);
   brakeFactor = 0;
   forwardAccel = rightAccel = upAccel = 0;
   yawFactor = 1;
   rollFactor = pitchFactor = 2;
   yawAmount = 0;
   maxSpeed = 20;
   shotSpeed = 40;
   timeOfLastShot = 0;
   timeOfLastBeam = 0;
   lastGunFired = 0;
   shipRadius = 1;
   maxX = maxY = maxZ = 1;
   minX = minY = minZ = -1;
   forward->updateMagnitude(0, 0, 1);
   up->updateMagnitude(0, 1, 0);
   right->updateMagnitude(-1, 0, 0);
   fireShots = fireBeams = false;
   shotPhi = shotTheta = 0;
   // The ship's score. This number is displayed to the screen.
   score = 0;
   // The ship's health. This number is displayed to the screen.
   health = 100;
}

/**
 * Retrieve the ship's health.
 */
int AsteroidShip::getHealth() {
   return health;
}

/**
 * Retrieve the ship's score.
 */
int AsteroidShip::getScore() {
   return score;
}

void AsteroidShip::startYaw(double yawAmountIn) {
   yawAmount = yawAmountIn;
}

void AsteroidShip::updateAcceleration() {
   acceleration->updateMagnitude(
         forward->scalarMultiply(forwardAccel).add(
         right->scalarMultiply(rightAccel).add(
         up->scalarMultiply(upAccel))));
}

void AsteroidShip::brake(double brakeFactorIn) {
   brakeFactor = brakeFactorIn;
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

   velocity->xMag -= velocity->xMag * timeDiff * brakeFactor;
   velocity->yMag -= velocity->yMag * timeDiff * brakeFactor;
   velocity->zMag -= velocity->zMag * timeDiff * brakeFactor;
   
   double speed = velocity->getLength();
   if (speed > maxSpeed)
      velocity->setLength(maxSpeed);

   Object3D::update(timeDiff);
   
   roll(timeDiff * rollAmount * pitchFactor);
   pitch(timeDiff * pitchAmount * pitchFactor);
   yaw(timeDiff * yawAmount * yawFactor);

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
   Point3D start = *position;
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
      distance = position->distanceFrom(*(*asteroid)->position);
      if (distance < shipRadius + (*asteroid)->collisionRadius) {
         // TEMP. We should damage the ship.
         asteroid = asteroids.erase(asteroid);
         score += (*asteroid)->collisionRadius * (rand()%3 + 1);
         health -= (*asteroid)->collisionRadius/2;
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
   shotDirection.updateMagnitude(forward);
   shotDirection.rotate(shotPhi, *right);
   shotDirection.rotate(shotTheta, *up);
}
