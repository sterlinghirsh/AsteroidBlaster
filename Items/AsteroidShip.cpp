/**
 * AsteroidShip.cpp
 * Sterling Hirsh
 * This is the ship that a player plays from or can see.
 */

#include "AsteroidShip.h"
#include "Shots/AsteroidShotBeam.h"
#include <math.h>
#include <time.h>

#ifndef M_PI
#define M_PI           3.14159265358979323846
#endif

using namespace std;

materialStruct brown = {
  {0.3, .1, 0.1, 1.0},
  {0.64, 0.16, 0.16, 1.0},
  {0.64, 0.16, 0.16, 1.0},
  {30.0}
};

materialStruct black = {
  {0.0, .0, 0.0, 1.0},
  {0.0, 0.0, 0.0, 1.0},
  {0.0, 0.0, 0.0, 1.0},
  {0.0}
};

materialStruct something = {
  {0.33, 0.22, 0.03, 1.0},
  {0.78, 0.57, 0.11, 1.0},
  {0.99, 0.91, 0.81, 1.0},
  {0.0}
};

materialStruct Cyan = {
  {0.0, 1, 1, 1.0},
  {0.0, 1, 1, 1.0},
  {0.2, 1.0, 1.0, 1.0},
  {50.0}
};

materialStruct BrightYellow = {
  {1, 1, 0.0, 1.0},
  {1, 1, 0.0, 1.0},
  {1.0, 1.0, 0.2, 1.0},
  {50.0}
};

materialStruct Orange = {
  {1, .27, 0.0, 1.0},
  {1, .27, 0.0, 1.0},
  {1.0, .27, 0.0, 1.0},
  {50.0}
};

materialStruct blue = {
  {0.3, .0, 0.0, 1.0},
  {0.6, .0, 0.0, 1.0},
  {0.8, .6, .6, 1.0},
  {32.0}
};

materialStruct white = {
  {.5, .5, .5, 1.0},
  {1, 1, 1, 1.0},
  {1, 1, 1, 1.0},
  {100}
};


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
   maxX = maxY = maxZ = 2;
   minX = minY = minZ = -2;
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
   
   // Create our Radar
   //radar = new Radar();
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


void AsteroidShip::update(double timeDiff) {
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

   updateShotDirectionVector();
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
   shotDirection.movePoint(start);
   if (fireShots && (timeOfLastShot < curTime - (1 / AsteroidShot::frequency) ||
    timeOfLastShot == 0)) {
      custodian->add(new AsteroidShot(start,
       shotDirection.scalarMultiply(shotSpeed), this));
      timeOfLastShot = curTime;
   }
   if (fireBeams && (timeOfLastBeam < curTime - (1 / AsteroidShotBeam::frequency) ||
    timeOfLastBeam == 0)) {
      custodian->add(new AsteroidShotBeam(start, shotDirection, this));
      timeOfLastBeam = curTime;
   }
}

void draw_ship(){

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
   Vector3D axis;
   Vector3D upStart(0, 1, 0);
   Vector3D forwardStart(0, 0, 1);

   double angle;
   double forwardAngle;
   axis = upStart.cross(*up);
   angle = upStart.getAngleInDegrees(*up);
   // rotate forward about up
   forwardStart.rotateByDegrees(angle, axis);
   forwardAngle = forwardStart.getAngleInDegrees(*forward);
   //up->print();
   forwardStart.print();
   printf("%f\n", forwardAngle);
   
   for (shotIter = shots.begin(); shotIter != shots.end(); shotIter++) {
      (*shotIter)->draw();
   }
   glMatrixMode(GL_MODELVIEW);
   glPushMatrix();
	getStuff();
	//printf("upstart?: %f\n", upstart->xMag);
	//printf("up?: %f\n", up->xMag);
	//upstart = up;
	//rotate();
        glTranslatef(position->x + 3*forward->xMag, position->y + 3*forward->yMag, position->z + 3*forward->zMag);
	//rotate();
	//glRotatef(forward->xMag * 90, 0, 1, 0);
	//glRotatef(up->zMag * 90, 1, 0, 0);
	//glRotatef((1 - up->yMag) * 90 - (1 - forward->zMag) * 90, 0, 0, 1);
   //glRotatef(-angle, x, y, z);
   forwardStart.draw();
   //glRotatef(forwardAngle, up->xMag, up->yMag, up->zMag);
   /*if(forwardStart.xMag > 0){
   glRotatef(forwardAngle, up->xMag, up->yMag, up->zMag);}
   else{glRotatef(360 - forwardAngle, up->xMag, up->yMag, up->zMag);}*/
   glRotatef(angle, axis.xMag, axis.yMag, axis.zMag);
   //glRotatef(180, 0 , 1, 0);
   glScalef(1.5, .5, .8);
   
   //draw_ship();
    glPushMatrix();
    glTranslatef(0, 0, 1);
    draw_vectors();
    glPopMatrix();
   glPopMatrix();
	//printf("upstart?: %f\n", upstart->yMag);
	//printf("upstart?: %f\n", up->xMag);
   //i/f(change % 2 > 0){
   //upstart = up;
   //printf("Angle?: %f\n", up->zMag);

   //}
}

/**
 * This handles our collisions. Duh.
 * We use a series of dynamic_casts to figure out what we've hit.
 */
void AsteroidShip::handleCollision(Object3D* other) {
   Asteroid3D* asteroid;
   
   // Try converting other into an Asteroid3D
   if ((asteroid = dynamic_cast<Asteroid3D*>(other)) != NULL) {
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
