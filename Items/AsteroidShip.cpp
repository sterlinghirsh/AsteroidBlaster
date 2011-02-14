/**
 * AsteroidShip.cpp
 * Sterling Hirsh
 * This is the ship that a player plays from or can see.
 */

#include "AsteroidShip.h"
#include "Shots/BeamShot.h"
#include "Shots/ProjectileShot.h"
#include <math.h>
#include <time.h>
#include "Utility/Quaternion.h"

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
      //shipRadius = 1; NOT USED
      maxX = maxY = maxZ = 0.3;
      minX = minY = minZ = -0.3;
      updateBoundingBox();

      // Orientation vectors.
      upstart = new Vector3D(0, 1, 0);
      upstart->updateMagnitude(0, 1, 0);
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

      // TODO: create all of the shooters that this ship will have.
      shooter = new ShootingAI(this);

      // Create our Radar
      radar = new Radar(this);
      
      quadratic = gluNewQuadric();
   }

/**
 * Set the currently selected weapon to the weapon type specified.
 */
void AsteroidShip::selectWeapon(int weaponType) {
   currentWeapon = weaponType;
}

/**
 * Set the AI to take over firing
 *
 void AsteroidShip::setShootingAI( bool ai )
 {
 }
 */

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

void AsteroidShip::addNewParticle(Point3D& emitter, Vector3D& baseDirection,
 Vector3D& offsetDirectionX, Vector3D& offsetDirectionY) {
   static Vector3D particleVariation;
   static Point3D curPoint;
   static Vector3D initialOffset;
   static Vector3D randomOffset;

   curPoint = emitter;

   // Translate the point in 2D
   randomOffset.add(offsetDirectionX.scalarMultiply(randdouble() - 0.5));
   randomOffset.add(offsetDirectionY.scalarMultiply(randdouble() - 0.5));
   randomOffset.add(baseDirection.scalarMultiply(randdouble() -0.5));
   randomOffset.scalarMultiplyUpdate(0.01);

   particleVariation.updateMagnitude(baseDirection.scalarMultiply(randdouble() * 2));
   particleVariation.addUpdate(offsetDirectionX.scalarMultiply(randdouble() * 8 - 4));
   particleVariation.addUpdate(offsetDirectionY.scalarMultiply(randdouble() * 8 - 4));
   particleVariation.scalarMultiplyUpdate(0.05);
   //curPoint = position->add(randomPoint);
   initialOffset.movePoint(curPoint);
   randomOffset.movePoint(curPoint);
   Particle::Add(new Point3D(curPoint), 
    new Vector3D(baseDirection.add(particleVariation)));
}

void AsteroidShip::createEngineParticles(double timeDiff) {
   //add particles in the opposite direction of the acceration
   
   const float increment = 0.01f;
   
   //const float length = acceleration->getLength;
   const int newParticlesPerSecond = 10000;
   static Vector3D baseParticleAcceleration;
   static Point3D emitter;

   // First do up upAcceleration.
   if (curUpAccel != 0) {
      baseParticleAcceleration = velocity->add(up->scalarMultiply(-curUpAccel * 0.2));
      emitter = *position;
      forward->movePoint(emitter, -0.5);
      for (int i = 0; i <= newParticlesPerSecond * timeDiff; ++i){
         addNewParticle(emitter, baseParticleAcceleration, *forward, *right);
      }
   }

   // Next do right upAcceleration.
   if (curRightAccel != 0) {
      baseParticleAcceleration = velocity->add(right->scalarMultiply(-curRightAccel * 0.2));
      emitter = *position;
      forward->movePoint(emitter, -0.5);
      for (int i = 0; i <= newParticlesPerSecond * timeDiff; ++i){
         addNewParticle(emitter, baseParticleAcceleration, *forward, *up);
      }
   }
   
   // Next do forward upAcceleration.
   if (curForwardAccel != 0) {
      // We want to do two streams.
      baseParticleAcceleration = velocity->add(forward->scalarMultiply(-curForwardAccel * 0.2));
      Point3D initialPoint(*position);
      forward->movePoint(initialPoint, -0.7 - (curForwardAccel * 0.02));

      // First do the right side.
      right->movePoint(initialPoint, 0.1);
      baseParticleAcceleration.addUpdate(right->scalarMultiply(0.5));
      for (int i = 0; i <= newParticlesPerSecond * timeDiff; ++i){
         addNewParticle(initialPoint, baseParticleAcceleration, *forward, *up);
      }

      // Next do the left side.
      right->movePoint(initialPoint, -0.2);
      baseParticleAcceleration.addUpdate(right->scalarMultiply(-1));
      for (int i = 0; i <= newParticlesPerSecond * timeDiff; ++i){
         addNewParticle(initialPoint, baseParticleAcceleration, *forward, *up);
      }
   }

}

void AsteroidShip::update(double timeDiff) {
   //TODO: Make the shooting and AIs think.

   if (shooter->isEnabled()) {
      shooter->think(timeDiff);
   } else {
      updateShotDirectionVector();
   }
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

   createEngineParticles(timeDiff);
}

/**
 * This translates an xOffset and yOffset in world coords to
 * a phi and a theta from the ship.
 */
void AsteroidShip::updateShotDirection(double xOffset, double yOffset) {
   xOff = xOffset;
   yOff = yOffset;
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
   //shotDirection.movePoint(start);
   if (fireShots && (timeOfLastShot < curTime - (1 / ProjectileShot::frequency) ||
            timeOfLastShot == 0)) {
      custodian->add(new ProjectileShot(start,
               shotDirection.scalarMultiply(shotSpeed), this));
      timeOfLastShot = curTime;
   }
   if (fireBeams && (timeOfLastBeam < curTime - (1 / BeamShot::frequency) ||
            timeOfLastBeam == 0)) {
      custodian->add(new BeamShot(start, shotDirection, this));
      timeOfLastBeam = curTime;
   }
}

void draw_ship(){
   glEnable(GL_LIGHTING);
   glColor4f(1, 1, 1, 1);
   glScalef(1.5, .5, .8);

   glBegin(GL_TRIANGLES);

   materials(black);
   glVertex3f(0, 0, 0);
   glVertex3f(.2, .2, 1.3);
   glVertex3f(.15, 0, 1);

   glVertex3f(0, 0, 0);
   glVertex3f(0, .15, 1);
   glVertex3f(.2, .2, 1.3);

   glVertex3f(0, 0, 0);
   glVertex3f(-.2, .2, 1.3);
   glVertex3f(0, .15, 1);

   glVertex3f(0, 0, 0);
   glVertex3f(-.15, 0, 1);
   glVertex3f(-.2, .2, 1.3);

   glVertex3f(0, 0, 0);
   glVertex3f(-.2, -.2, 1.3);
   glVertex3f(-.15, 0, 1);

   glVertex3f(0, 0, 0);
   glVertex3f(0, -.15, 1);
   glVertex3f(-.2, -.2, 1.3);

   glVertex3f(0, 0, 0);
   glVertex3f(.2, -.2, 1.3);
   glVertex3f(0, -.15, 1);

   glVertex3f(0, 0, 0);
   glVertex3f(.15, 0, 1);
   glVertex3f(.2, -.2, 1.3);
   glPopMatrix();

   /* Back of Ship */
   materials(black);

   glVertex3f(.15, 0, 1);
   glVertex3f(.2, .2, 1.3);
   glVertex3f(0, .15, 1);

   glVertex3f(.15, 0, 1);
   glVertex3f(0, -.15, 1);
   glVertex3f(.2, -.2, 1.3);

   glVertex3f(-.15, 0, 1);
   glVertex3f(-.2, -.2, 1.3);
   glVertex3f(0, -.15, 1);

   glVertex3f(-.15, 0, 1);
   glVertex3f(0, .15, 1);
   glVertex3f(-.2, .2, 1.3);

   glVertex3f(.15, 0, 1);
   glVertex3f(0, .15, 1);
   glVertex3f(0, 0, 1.6);

   glVertex3f(.15, 0, 1);
   glVertex3f(0, 0, 1.6);
   glVertex3f(0, -.15, 1);

   glVertex3f(-.15, 0, 1);
   glVertex3f(0, -.15, 1);
   glVertex3f(0, 0, 1.6);

   glVertex3f(-.15, 0, 1);
   glVertex3f(-0, 0, 1.6);
   glVertex3f(0, .15, 1);
   glEnd();

   /* Outline of Ship */
   glLineWidth(5.0);
   glBegin(GL_LINE_LOOP);
   materials(Orange);
   glVertex3f(.15, 0, 1);
   glVertex3f(.2, .2, 1.3);
   glVertex3f(0, .15, 1);

   glVertex3f(.15, 0, 1);
   glVertex3f(0, 0, 1.6);
   glVertex3f(0, .15, 1);

   glVertex3f(.15, 0, 1);
   glVertex3f(.2, -.2, 1.3);
   glVertex3f(0, -.15, 1);

   glVertex3f(.15, 0, 1);
   glVertex3f(0, 0, 1.6);
   glVertex3f(0, -.15, 1);

   glVertex3f(-.15, 0, 1);
   glVertex3f(-.2, -.2, 1.3);
   glVertex3f(0, -.15, 1);

   glVertex3f(-.15, 0, 1);
   glVertex3f(0, 0, 1.6);
   glVertex3f(0, -.15, 1);

   glVertex3f(-.15, 0, 1);
   glVertex3f(-.2, .2, 1.3);
   glVertex3f(0, .15, 1);

   glVertex3f(-.15, 0, 1);
   glVertex3f(-0, 0, 1.6);
   glVertex3f(0, .15, 1);

   glEnd();

   glLineWidth(5.0);
   glBegin(GL_LINE_LOOP);
   materials(Orange);
   glVertex3f(.15, 0, 1.01);
   glVertex3f(0, -.15, 1.01);
   glVertex3f(-.15, 0, 1.01);
   glVertex3f(0, .15, 1.01);
   glEnd();


   glLineWidth(4.0);
   glBegin(GL_LINE_LOOP);
   glColor3f(0, 0, 0);
   materials(Cyan);
   glVertex3f(0, 0, 0);
   glVertex3f(.2, .2, 1.3);
   glVertex3f(.15, 0, 1);

   materials(Cyan);
   glVertex3f(0, 0, 0);
   glVertex3f(.2, .2, 1.3);
   glVertex3f(0, .15, 1);

   glVertex3f(0, 0, 0);
   glVertex3f(-.2, .2, 1.3);
   glVertex3f(0, .15, 1);

   glVertex3f(0, 0, 0);
   glVertex3f(-.2, .2, 1.3);
   glVertex3f(-.15, 0, 1);

   glVertex3f(0, 0, 0);
   glVertex3f(-.2, -.2, 1.3);
   glVertex3f(-.15, 0, 1);

   glVertex3f(0, 0, 0);
   glVertex3f(-.2, -.2, 1.3);
   glVertex3f(0, -.15, 1);

   glVertex3f(0, 0, 0);
   glVertex3f(.2, -.2, 1.3);
   glVertex3f(0, -.15, 1);

   glVertex3f(0, 0, 0);
   glVertex3f(.2, -.2, 1.3);
   glVertex3f(.15, 0, 1);

   glEnd();
   glLineWidth(1.0);
}

void draw_vectors(){
   materials(white);
   glBegin(GL_LINES);
   glVertex3f(0,0,0);
   glVertex3f(0,.5,0);

   glVertex3f(0,0,0);
   glVertex3f(0,0,-1);

   glVertex3f(.5,0,0);
   glVertex3f(-.5,0,0);

   glEnd();

   glLineWidth(1.0);


}

/**
 * Draw an adorable little ship in the minimap.
 */
void AsteroidShip::drawInMinimap() {
   glPushMatrix();
   position->glTranslate();
   // Counteract the rotation done in GameState::drawInMinimap();
   glRotate();
   glScalef(15, 15, 15);
   draw_ship();
   glPopMatrix();
}

void AsteroidShip::getStuff(){
   x = (upstart->yMag * up->zMag) - (upstart->zMag * up->yMag);
   y = (upstart->zMag * up->xMag) - (upstart->xMag * up->zMag);
   z = (upstart->xMag * up->yMag) - (upstart->yMag * up->xMag);

   angle = acos(((upstart->xMag * up->xMag) + (upstart->yMag * up->yMag) + (upstart->zMag * up->zMag)) / (upstart->getLength() * up->getLength())) * 57.2958;
}

void AsteroidShip::rotate(){
   glMatrixMode(GL_MODELVIEW);
   glPushMatrix();
   glLoadIdentity();
   //glRotatef(1 - up->zMag, 1, 0, 0);
   //glMultMatrixf(trackballM);
   //glGetFloatv(GL_MODELVIEW_MATRIX, trackballM);
   glPopMatrix();
}

void AsteroidShip::draw() {
   glPushMatrix();
      // Translate to the position.
      position->glTranslate();
      // Rotate to the current up/right/forward vectors.
      glRotate();
      draw_ship();
   glPopMatrix();
}

/**
 * This handles our collisions. Duh.
 * We use a series of dynamic_casts to figure out what we've hit.
 */
void AsteroidShip::handleCollision(Object3D* other) {
   Asteroid3D* asteroid;
   Shard* shard;

   // Try converting other into a Shard
   if ((shard = dynamic_cast<Shard*>(other)) != NULL) {
      health += 10;
      if (health > 100) {
         health = 100;
      }
      numShards++;
      score += 69;
      // Try converting other into an Asteroid3D
   } else if ((asteroid = dynamic_cast<Asteroid3D*>(other)) != NULL) {
      // Decrease the player's health by an appropriate amount.
      velocity->addUpdate(*(asteroid->velocity));
      health -= asteroid->radius;
   }
}

void AsteroidShip::updateShotDirectionVector() {
   shotDirection.updateMagnitude(forward);
   shotDirection.rotate(shotPhi, *right);
   shotDirection.rotate(shotTheta, *up);
}

double AsteroidShip::getAimX() {
   return xOff;
}
double AsteroidShip::getAimY() {
   return yOff;
}

void AsteroidShip::debug() {
   printf("AsteroidShip::debug(): \n");
   minPosition->print();
   maxPosition->print();
   velocity->print();
   printf("--\n");
}

void AsteroidShip::updateShotDirection(Vector3D dir) {
   shotDirection = dir;
}

Radar *AsteroidShip::getRadar() {
   return radar;
}

void AsteroidShip::updateShotDirection(Point3D dir) {
   shotDirection = Vector3D(dir.x, dir.y, dir.z);
}
