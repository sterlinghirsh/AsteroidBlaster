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

AsteroidShip::AsteroidShip() : 
 Object3D(0, 0, 0, 0),     // Initialize superclass
 shotDirection(0, 0, 1) {  // Initialize shot direction to forward
   /* Initialize velocity and acceleration to 0. */
   velocity = new Vector3D(0, 0, 0);
   acceleration = new Vector3D(0, 0, 0);
   /* Currently not braking or acceleration. */
   isBraking = false;
   brakeFactor = 2;
   /* We store acceleration as scalars to multiply forward, right, and up by each tick. */
   curForwardAccel = curRightAccel = curUpAccel = 0;

   yawSpeed = rollSpeed = pitchSpeed = 0;
   maxSpeed = 5; // Units/s, probably will be changed with an upgrade.
   shotSpeed = 40; // Also probably will be changed with an upgrade.

   // Timing stuff
   timeOfLastShot = 0;
   timeOfLastBeam = 0;
   lastGunFired = 0;

   // Bounding box stuff.
   shipRadius = 1;
   maxX = maxY = maxZ = 1;
   minX = minY = minZ = -1;

   // Orientation vectors.
   forward->updateMagnitude(0, 0, 1);
   up->updateMagnitude(0, 1, 0);
   right->updateMagnitude(-1, 0, 0);

   // Is the ship firing? Not when it's instantiated.
   isFiring = false;
   shotPhi = shotTheta = 0;
   // The ship's score. This number is displayed to the screen.
   score = 0;
   // The ship's health. This number is displayed to the screen.
   health = 100;
   // The ship's currently selected weapon.
   currentWeapon = 0;
   // The ship's max motion parameters.
   maxForwardAccel = 10;
   maxRightAccel = 5;
   maxUpAccel = 5;
   maxYawSpeed = 2;
   maxPitchSpeed = 2;
   maxRollSpeed = 2;
}

/**
 * Set the currently selected weapon to the weapon type specified.
 */
void AsteroidShip::selectWeapon(int weaponType) {
   currentWeapon = weaponType;
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

void AsteroidShip::setYawSpeed(double yawAmountIn) {
   yawSpeed = yawAmountIn;
}

void AsteroidShip::setPitchSpeed(double pitchAmountIn) {
   pitchSpeed = pitchAmountIn;
}

void AsteroidShip::setRollSpeed(double rollAmountIn) {
   rollSpeed = rollAmountIn;
}

void AsteroidShip::updateAcceleration() {
   acceleration->updateMagnitude(
         forward->scalarMultiply(curForwardAccel).add(
         right->scalarMultiply(curRightAccel).add(
         up->scalarMultiply(curUpAccel))));
}

void AsteroidShip::setBrake(bool doBrake) {
   isBraking = doBrake;
}

/**
 * Set the engine's acceleration.
 */
void AsteroidShip::accelerateForward(int dir) {
   curForwardAccel = dir * maxForwardAccel;
   updateAcceleration();
}

void AsteroidShip::accelerateUp(int dir) {
   curUpAccel = dir * maxUpAccel;
   updateAcceleration();
}

void AsteroidShip::accelerateRight(int dir) {
   curRightAccel = dir * maxRightAccel;
   updateAcceleration();
}


void AsteroidShip::updatePosition(double timeDiff) {
   updateAcceleration();

   if (isBraking) {
      velocity->xMag -= velocity->xMag * timeDiff * brakeFactor;
      velocity->yMag -= velocity->yMag * timeDiff * brakeFactor;
      velocity->zMag -= velocity->zMag * timeDiff * brakeFactor;
   }
   
   double speed = velocity->getLength();
   if (speed > maxSpeed)
      velocity->setLength(maxSpeed);

   Object3D::update(timeDiff);
   
   roll(timeDiff * rollSpeed);
   pitch(timeDiff * pitchSpeed);
   yaw(timeDiff * yawSpeed);

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

void AsteroidShip::fire(bool startFiring) {
   isFiring = startFiring;
}

void AsteroidShip::keepFiring() {
   if (!isFiring) return;
   double curTime = doubleTime();
   Point3D start = *position;
   /* This is goofy as hell, so let's do it the OOP way. */
   bool fireShots = (currentWeapon == 0);
   bool fireBeams = (currentWeapon == 1);
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
         // Increase the player's score by an appropriate amount.
         //score += (*asteroid)->collisionRadius * (rand()%3 + 1);
         // Decrease the player's health by an appropriate amount.
         health -= (*asteroid)->collisionRadius * 2;
         // TEMP. Asteroids should not be simply erased.
         asteroid = asteroids.erase(asteroid);
         continue;
      }
      for (shotIter = shots.begin(); shotIter != shots.end(); shotIter++) {
         if ((*shotIter)->checkHit(*asteroid)) {
            if ((*asteroid)->handleHit(asteroids)) {
               score += (*asteroid)->collisionRadius * (rand()%3 + 1);
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
