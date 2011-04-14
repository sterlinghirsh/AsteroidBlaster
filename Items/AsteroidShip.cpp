/**
 * AsteroidShip.cpp
 * Sterling Hirsh
 * This is the ship that a player plays from or can see.
 */

#include "AsteroidShip.h"
#include <math.h>
#include <time.h>
#include "Utility/Quaternion.h"
#include "Utility/SoundEffect.h"
#include "Particles/EngineParticle.h"

#ifndef M_PI
#define M_PI           3.14159265358979323846
#endif

#define BOOST_SCALE 2.0

using namespace std;
const double rotationFactor = 3;
const float shipScale = 5;

AsteroidShip::AsteroidShip(const GameState* _gameState) :
   Object3D(_gameState),     // Initialize superclass
   shotDirection(0, 0, 1) {  // Initialize shot direction to forward
      /* Currently not braking or acceleration. */
      isBraking = false;
      isBoosting = false;
      shakeAmount = 0;
      brakeFactor = 2;
      /* We store acceleration as scalars to multiply forward, right, and up by each tick. */
      curForwardAccel = curRightAccel = curUpAccel = 0;
      
      cullRadius = 12;
      
      yawSpeed = rollSpeed = pitchSpeed = 0;
      maxSpeed = 5; // Units/s, probably will be changed with an upgrade.
      maxBoostSpeed = maxSpeed * 1.5; // Units/s, probably will be changed with an upgrade.

      // Bounding box stuff.
      //shipRadius = 1; NOT USED
      maxX = maxY = maxZ = 4;
      minX = minY = minZ = -4;
      updateBoundingBox();
      
      spin = 90;
      flashiness = 0;
      tracker = 0;
      rando = 1;
      
      zMove = 2;
      lineMove = zMove / 4;
      frontX = 0;
      frontY = 0;
      frontZ = 0;
      cornerX = .2;
      cornerY = .2;
      cornerZ = 1.3;
      middleXY = .15;
      middleZ = 1;
      backX = 0;
      backY = 0;
      backZ = 1.6;
      
      //skew must be set to 0 til I figure out a better way to do things
      skew = 0;
      
      x2Change = middleXY / 2;
      y2Change = middleXY / 2;
      z2Change = (backZ - middleZ) / 2;
      xChange = 0;
      yChange = 0;
      zChange = 0;
      backChange = 0;
      
      shotOriginScale = 4;
      shotOrigin = *position;
      forward->movePoint(shotOrigin, shotOriginScale);
      

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
      // The number of shard collected. This number is displayed to the screen.
      nShards = 0;
      // The ship's max motion parameters.
      maxForwardAccel = 10;
      maxRightAccel = 5;
      maxUpAccel = 5;
      maxYawSpeed = maxPitchSpeed = maxRollSpeed = 3;
      //engine upgrade, terminal speed is raised when leved up
      engineUpgrade = 0;


      // TODO: create all of the shooters that this ship will have.
      shooter = new ShootingAI(this);
      flyingAI = new FlyingAI(this);

      // Create our Radar
      radar = new Radar(this);

      // Add weapons to the list!
      weapons.push_back(new Blaster(this));
      weapons.push_back(new RailGun(this));
      /* IF YOU CHANGE THE ORDER OF THIS LIST, CHANGE THE MAGIC NUMBER 2 for the TractorBeam IN THE FUNCTION drawShotDirectionIndicators below.
       */
      weapons.push_back(new TractorBeam(this));
      weapons.push_back(new Electricity(this));
      weapons.push_back(new LawnMower(this));
      weapons.push_back(new Ram(this));
      weapons.push_back(new AntiInertia(this));
      weapons.push_back(new Bomber(this));
      weapons.push_back(new Missile(this));

      // The ship's currently selected weapon.
      currentWeapon = 0;
      soundHandle = -1;

      cameraOffset = new Vector3D(0, 2, 5);
      currentView = VIEW_THIRDPERSON_SHIP;
      zoomFactor = 1.0;
      zoomSpeed = 0.0;

      shouldDrawInMinimap = true;
      accelerationStartTime = doubleTime();
      particlesEmitted = 0;
   }

/**
 * Set the currently selected weapon to the weapon type specified.
 * The currentWeapon can also be set from nextWeapon() and prevWeapon().
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
 * Retrieve a pointer to the ship's shot direction vector
 */
Vector3D* AsteroidShip::getShotDirection() {
   return &shotDirection;
}

/**
 * Retrieve the ship's score.
 */
int AsteroidShip::getScore() {
   return score;
}

/**
 * Retrieve the number of shards collected.
 */
int AsteroidShip::getShards() {
   return nShards;
}

void AsteroidShip::setYawSpeed(double yawAmountIn) {
   yawSpeed = rotationFactor * yawAmountIn;
}

void AsteroidShip::setPitchSpeed(double pitchAmountIn) {
   pitchSpeed = rotationFactor * pitchAmountIn;
}

void AsteroidShip::setRollSpeed(double rollAmountIn) {
   rollSpeed = rotationFactor * rollAmountIn;
}

void AsteroidShip::updatePlayerAcceleration() {
   addAcceleration(new Vector3D(
         forward->scalarMultiply(curForwardAccel).add(
            right->scalarMultiply(curRightAccel).add(
               up->scalarMultiply(curUpAccel)))));
}

void AsteroidShip::setBrake(bool doBrake) {
   isBraking = doBrake;
}

void AsteroidShip::setBoost(bool doBoost) {
   isBoosting = doBoost;
}

/**
 * Set the engine's acceleration.
 */
void AsteroidShip::accelerateForward(int dir) {
   curForwardAccel = dir * maxForwardAccel;
   updatePlayerAcceleration();
   accelerationStartTime = doubleTime();
   particlesEmitted = 0;
}

void AsteroidShip::accelerateUp(int dir) {
   curUpAccel = dir * maxUpAccel;
   updatePlayerAcceleration();
   accelerationStartTime = doubleTime();
   particlesEmitted = 0;
}

void AsteroidShip::accelerateRight(int dir) {
   curRightAccel = dir * maxRightAccel;
   updatePlayerAcceleration();
   accelerationStartTime = doubleTime();
   particlesEmitted = 0;
}

void AsteroidShip::addNewParticle(Point3D& emitter, Vector3D& baseDirection,
      Vector3D& offsetDirectionX, Vector3D& offsetDirectionY, int color) {
   static Vector3D particleVariation;
   static Point3D curPoint;
   static Vector3D initialOffset;
   static Vector3D randomOffset;
   const float randomAmount = 0.1f;
   curPoint = emitter;

   // Translate the point in 2D
   randomOffset.add(offsetDirectionX.scalarMultiply(randomAmount * (randdouble() - 0.5)));
   randomOffset.add(offsetDirectionY.scalarMultiply(randomAmount * (randdouble() - 0.5)));
   randomOffset.add(baseDirection.scalarMultiply(randomAmount * (randdouble() -0.5)));
   randomOffset.scalarMultiplyUpdate(0.01);

   particleVariation.updateMagnitude(baseDirection.scalarMultiply(randdouble() * 2));
   particleVariation.addUpdate(offsetDirectionX.scalarMultiply(randdouble() * 8 - 4));
   particleVariation.addUpdate(offsetDirectionY.scalarMultiply(randdouble() * 8 - 4));
   particleVariation.scalarMultiplyUpdate(0.05);
   //curPoint = position->add(randomPoint);
   initialOffset.movePoint(curPoint);
   randomOffset.movePoint(curPoint);
   EngineParticle::Add(new Point3D(curPoint),
         new Vector3D(baseDirection.add(particleVariation)), color, gameState);
}

void AsteroidShip::createEngineParticles(double timeDiff) {
   //add particles in the opposite direction of the acceration

   const float increment = 0.01f;

   //const float length = acceleration->getLength;
   const int newParticlesPerSecond = 30;
   static Vector3D baseParticleAcceleration;
   static Point3D emitter;

   double accelerationTime = doubleTime() - accelerationStartTime;
   while ((double) particlesEmitted / accelerationTime < newParticlesPerSecond) {
      // First do up Acceleration.
      if (curUpAccel != 0) {
         baseParticleAcceleration = up->scalarMultiply(-curUpAccel * 0.2);
         emitter = *position;
         forward->movePoint(emitter, -0.5);
         addNewParticle(emitter, baseParticleAcceleration, *forward, *right);
      }

      // Next do right Acceleration.
      if (curRightAccel != 0) {
         baseParticleAcceleration = right->scalarMultiply(-curRightAccel * 0.2);
         emitter = *position;
         forward->movePoint(emitter, -0.7);
         addNewParticle(emitter, baseParticleAcceleration, *forward, *up);
      }

      // Next do forward Acceleration.
      if (curForwardAccel != 0) {
         // We want to do two streams.
         baseParticleAcceleration = forward->scalarMultiply(-curForwardAccel * 0.05);
         Point3D initialPoint(*position);
         forward->movePoint(initialPoint, -0.7 - (curForwardAccel * 0.02));

         // First do the right side.
         right->movePoint(initialPoint, 1);
         addNewParticle(initialPoint, baseParticleAcceleration, *right, *up, 2);

         // Next do the left side.
         right->movePoint(initialPoint, -2);
         addNewParticle(initialPoint, baseParticleAcceleration, *right, *up, 3);
      }

      ++particlesEmitted;
   }
}

void AsteroidShip::update(double timeDiff) {
   if (shooter->isEnabled()) {
      shooter->think(timeDiff);
   }
   
   if(flyingAI->isEnabled()) {
      flyingAI->think(timeDiff);
   } else {
      updatePlayerAcceleration();
   }
   
   if (isBraking) {
      velocity->x -= velocity->x * timeDiff * brakeFactor;
      velocity->y -= velocity->y * timeDiff * brakeFactor;
      velocity->z -= velocity->z * timeDiff * brakeFactor;
   }
   if (isBoosting) {
      velocity->x += velocity->x * timeDiff * brakeFactor;
      velocity->y += velocity->y * timeDiff * brakeFactor;
      velocity->z += velocity->z * timeDiff * brakeFactor;
   }

   double speed = velocity->getLength();
   if (!isBoosting && speed > maxSpeed + (engineUpgrade*2)) {
      velocity->setLength(maxSpeed+ (engineUpgrade*2));
   } else if (isBoosting && speed > (maxBoostSpeed + (engineUpgrade*2))) {
      velocity->setLength(maxBoostSpeed + (engineUpgrade*2));
   }

   Object3D::update(timeDiff);

   roll(timeDiff * rollSpeed);
   pitch(timeDiff * pitchSpeed);
   yaw(timeDiff * yawSpeed);
   
   if (!shooter->isEnabled()) {
      updateShotDirectionVector();
   }

   // Update weapons.
   for (std::vector<Weapon*>::iterator iter = weapons.begin();
         iter != weapons.end(); ++iter) {
      (*iter)->update(timeDiff);
   }

   if (!gameState->inMenu &&
    (curForwardAccel != 0 || curUpAccel != 0 || curRightAccel != 0)) {
      if (soundHandle == -1)
         soundHandle = SoundEffect::playSoundEffect("ShipEngine.wav", true);
   } else {
      if (soundHandle != -1) {
         SoundEffect::stopSoundEffect(soundHandle);
         soundHandle = -1;
      }
   }

   shotOrigin = *position;
   forward->movePoint(shotOrigin, shotOriginScale);
   
   // Actually fire the weapons.
   keepFiring();

   if (shakeAmount != 0) {
      shakeAmount -= (float) (5 * shakeAmount * timeDiff);
      if (shakeAmount < 0.01) {
         shakeAmount = 0;
      }
   }

   if (curForwardAccel == 10) {
      backChange += (zMove * timeDiff);
      if (backChange > (backZ - middleZ)) {
         backChange = (backZ - middleZ);
      }
      
      if (backChange == (backZ - middleZ)) {
         xChange += lineMove * timeDiff;
         yChange += lineMove * timeDiff;
         zChange += zMove * timeDiff;
         x2Change += lineMove * timeDiff;
         y2Change += lineMove * timeDiff;
         z2Change += zMove * timeDiff;
      }
      
      if (x2Change > middleXY) {
         xChange = 0;
         yChange = 0;
         zChange = 0;
         x2Change = middleXY / 2;
         y2Change = middleXY / 2;
         z2Change = (backZ - middleZ) / 2;
      }
      
   } else {
      backChange -= zMove * timeDiff;
      if (backChange < 0) {
         backChange = 0;
      }
   }
   

   createEngineParticles(timeDiff);

   // Update the zoom level.
   const float minZoom = 1;
   const float maxZoom = 3;
   zoomFactor = (float) clamp(zoomFactor + (timeDiff * zoomSpeed)
    , minZoom, maxZoom);
}

/**
 * This translates an xOffset and yOffset in world coords to
 * a phi and a theta from the ship.
 */
void AsteroidShip::updateShotDirection(double xOffset, double yOffset) {
   aimX = xOffset;
   aimY = yOffset;
   shotPhi = (M_PI/180) * yOffset * VERT_FOV / 2;
   shotTheta = (M_PI/180) * xOffset * VERT_FOV / (/*((double) GW/GH) / */ -2);
   updateShotDirectionVector();
}

void AsteroidShip::fire(bool startFiring) {
   isFiring = startFiring;
}

void AsteroidShip::keepFiring() {
   if (!isFiring) return;
   weapons[currentWeapon]->fire();
}

void AsteroidShip::draw_frontpanels() {
   glBegin(GL_TRIANGLES);

   glVertex3d(frontX, frontY, frontZ);
   glVertex3d(cornerX, cornerY, cornerZ);
   glVertex3d(middleXY, skew, middleZ);

   glVertex3d(frontX, frontY, frontZ);
   glVertex3d(skew, middleXY, middleZ);
   glVertex3d(cornerX, cornerY, cornerZ);

   glVertex3d(frontX, frontY, frontZ);
   glVertex3d(-cornerX, cornerY, cornerZ);
   glVertex3d(skew, middleXY, middleZ);

   glVertex3d(frontX, frontY, frontZ);
   glVertex3d(-middleXY, skew, middleZ);
   glVertex3d(-cornerX, cornerY, cornerZ);

   glVertex3d(frontX, frontY, frontZ);
   glVertex3d(-cornerX, -cornerY, cornerZ);
   glVertex3d(-middleXY, skew, middleZ);

   glVertex3d(frontX, frontY, frontZ);
   glVertex3d(skew, -middleXY, middleZ);
   glVertex3d(-cornerX, -cornerY, cornerZ);

   glVertex3d(frontX, frontY, frontZ);
   glVertex3d(cornerX, -cornerY, cornerZ);
   glVertex3d(skew, -middleXY, middleZ);

   glVertex3d(frontX, frontY, frontZ);
   glVertex3d(middleXY, skew, middleZ);
   glVertex3d(cornerX, -cornerY, cornerZ);
   glEnd();
}

void AsteroidShip::draw_backpanels() {
   glBegin(GL_TRIANGLES);
   
   glVertex3d(middleXY, skew, middleZ);
   glVertex3d(cornerX, cornerY, cornerZ);
   glVertex3d(skew, middleXY, middleZ);

   glVertex3d(middleXY, skew, middleZ);
   glVertex3d(skew, -middleXY, middleZ);
   glVertex3d(cornerX, -cornerY, cornerZ);

   glVertex3d(-middleXY, skew, middleZ);
   glVertex3d(-cornerX, -cornerY, cornerZ);
   glVertex3d(skew, -middleXY, middleZ);

   glVertex3d(-middleXY, skew, middleZ);
   glVertex3d(skew, middleXY, middleZ);
   glVertex3d(-cornerX, cornerY, cornerZ);
   glEnd();
}

void AsteroidShip::draw_spaceboner() {
   glBegin(GL_TRIANGLES);
   if (curForwardAccel == 10.0) {
      glColor4d(1, .4, 0, .5);
      
      glVertex3d(middleXY, skew, middleZ);
      glVertex3d(skew, middleXY, middleZ);
      glVertex3d(backX, backY, middleZ + backChange);

      glVertex3d(middleXY, skew, middleZ);
      glVertex3d(backX, backY, middleZ + backChange);
      glVertex3d(skew, -middleXY, middleZ);

      glVertex3d(-middleXY, skew, middleZ);
      glVertex3d(skew, -middleXY, middleZ);
      glVertex3d(backX, backY, middleZ + backChange);

      glVertex3d(-middleXY, skew, middleZ);
      glVertex3d(backX, backY, middleZ + backChange);
      glVertex3d(skew, middleXY, middleZ);
   } else if (backChange == 0) {
      glColor4d(0, 0, 0, .4);

      glVertex3d(-middleXY, skew, middleZ);
      glVertex3d(middleXY, skew, middleZ);
      glVertex3d(skew, middleXY, middleZ);
      
      glVertex3d(-middleXY, skew, middleZ);
      glVertex3d(middleXY, skew, middleZ);
      glVertex3d(skew, -middleXY, middleZ);
   } else {
      glColor4d(1, .4, 0, .5);

      glVertex3d(middleXY, skew, middleZ);
      glVertex3d(skew, middleXY, middleZ);
      glVertex3d(backX, backY, middleZ + backChange);

      glVertex3d(middleXY, skew, middleZ);
      glVertex3d(backX, backY, middleZ + backChange);
      glVertex3d(skew, -middleXY, middleZ);

      glVertex3d(-middleXY, skew, middleZ);
      glVertex3d(skew, -middleXY, middleZ);
      glVertex3d(backX, backY, middleZ + backChange);

      glVertex3d(-middleXY, skew, middleZ);
      glVertex3d(backX, backY, middleZ + backChange);
      glVertex3d(skew, middleXY, middleZ);
   }   
   glEnd();
}

void AsteroidShip::draw_bonerlines() {
   if (curForwardAccel == 10.0) {
      if (backChange == (backZ - middleZ)) {
         glBegin(GL_LINE_LOOP);
         glColor3d(1, .4, 0);
         glVertex3d(middleXY - xChange, skew, middleZ + zChange);
         glVertex3d(skew, middleXY - yChange, middleZ + zChange);
         glVertex3d(-middleXY + xChange, skew, middleZ + zChange);
         glVertex3d(skew, -middleXY + yChange, middleZ + zChange);
         glVertex3d(middleXY - xChange, skew, middleZ + zChange);  
         glEnd();
         
         glBegin(GL_LINE_LOOP);
         glVertex3d(middleXY - x2Change, skew, middleZ + z2Change);
         glVertex3d(skew, middleXY - y2Change, middleZ + z2Change);
         glVertex3d(-middleXY + x2Change, skew, middleZ + z2Change);
         glVertex3d(skew, -middleXY + y2Change, middleZ + z2Change);
         glVertex3d(middleXY - x2Change, skew, middleZ + z2Change);
         glEnd();

      }
      
      glBegin(GL_LINE_LOOP);
      glColor3d(1, .4, 0);
      
      glVertex3d(middleXY, skew, middleZ);
      glVertex3d(cornerX, cornerY, cornerZ);
      glVertex3d(skew, middleXY, middleZ);

      glVertex3d(middleXY, skew, middleZ);
      glVertex3d(backX, backY, middleZ + backChange);
      glVertex3d(skew, middleXY, middleZ);

      glVertex3d(middleXY, skew, middleZ);
      glVertex3d(cornerX, -cornerY, cornerZ);
      glVertex3d(skew, -middleXY, middleZ);

      glVertex3d(middleXY, skew, middleZ);
      glVertex3d(backX, backY, middleZ + backChange);
      glVertex3d(skew, -middleXY, middleZ);

      glVertex3d(-middleXY, skew, middleZ);
      glVertex3d(-cornerX, -cornerY, cornerZ);
      glVertex3d(skew, -middleXY, middleZ);

      glVertex3d(-middleXY, skew, middleZ);
      glVertex3d(backX, backY, middleZ + backChange);
      glVertex3d(skew, -middleXY, middleZ);

      glVertex3d(-middleXY, skew, middleZ);
      glVertex3d(-cornerX, cornerY, cornerZ);
      glVertex3d(skew, middleXY, middleZ);

      glVertex3d(-middleXY, skew, middleZ);
      glVertex3d(backX, backY, middleZ + backChange);
      glVertex3d(skew, middleXY, middleZ);

      glEnd();
   } else if (backChange == 0) {
      glBegin(GL_LINE_LOOP);
      glColor3d(1, .4, 0);
      glVertex3d(middleXY, skew, middleZ);
      glVertex3d(skew, middleXY, middleZ);
      glVertex3d(-middleXY, skew, middleZ);
      glVertex3d(skew, -middleXY, middleZ);
      glVertex3d(middleXY, skew, middleZ);
      glEnd();
   } else {
      glBegin(GL_LINE_LOOP);
      glColor3d(1, .4, 0);
      glVertex3d(middleXY, skew, middleZ);
      glVertex3d(backX, backY, middleZ + backChange);
      glVertex3d(skew, middleXY, middleZ);
      
      glVertex3d(middleXY, skew, middleZ);
      glVertex3d(skew, -middleXY, middleZ);
      glVertex3d(backX, backY, middleZ + backChange);
      
      
      glVertex3d(-middleXY, skew, middleZ);
      glVertex3d(skew, -middleXY, middleZ);
      glVertex3d(backX, backY, middleZ + backChange);
      
      
      glVertex3d(-middleXY, skew, middleZ);
      glVertex3d(backX, backY, middleZ + backChange);
      glVertex3d(skew, middleXY, middleZ);
      glEnd();
      
      glBegin(GL_LINE_LOOP);
      glColor3d(1, .4, 0);
      glVertex3d(middleXY, skew, middleZ);
      glVertex3d(skew, middleXY, middleZ);
      glVertex3d(-middleXY, skew, middleZ);
      glVertex3d(skew, -middleXY, middleZ);
      glVertex3d(middleXY, skew, middleZ);
      glEnd();
   }
}

void AsteroidShip::draw_frontlines() {
   glBegin(GL_LINES);
   glColor3d(0, 1, 1);
   glVertex3d(frontX, frontY, frontZ);
   glVertex3d(cornerX, cornerY, cornerZ);
   
   glVertex3d(frontX, frontY, frontZ);
   glVertex3d(-cornerX, cornerY, cornerZ);
   
   glVertex3d(frontX, frontY, frontZ);
   glVertex3d(-cornerX, -cornerY, cornerZ);
   
   glVertex3d(frontX, frontY, frontZ);
   glVertex3d(cornerX, -cornerY, cornerZ);
   glEnd(); 
}

void AsteroidShip::draw_backlines() {
   glBegin(GL_LINE_LOOP);
   glVertex3d(middleXY, skew, middleZ);
   glVertex3d(cornerX, cornerY, cornerZ);
   glVertex3d(skew, middleXY, middleZ);
   glVertex3d(-cornerX, cornerY, cornerZ);
   glVertex3d(-middleXY, skew, middleZ);
   glVertex3d(-cornerX, -cornerY, cornerZ);
   glVertex3d(skew, -middleXY, middleZ);
   glVertex3d(cornerX, -cornerY, cornerZ);
   glVertex3d(middleXY, skew, middleZ);
   glEnd();
}

void AsteroidShip::draw_ship() {
   glPolygonOffset(1.0f, 1.0f);
   glEnable(GL_POLYGON_OFFSET_FILL);
   glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

   glDisable(GL_CULL_FACE);
   glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
   glEnable(GL_LIGHTING);
   glEnable(GL_COLOR_MATERIAL);
   glTranslated(0, 0, -4);
   glScaled(1.5, .5, .8);
   glScaled(shipScale, shipScale, shipScale);

   setMaterial(BlackSolid);

   glEnable(GL_NORMALIZE);

   draw_frontpanels();
   
   draw_backpanels();
   
   draw_spaceboner();

   /* Outline of Ship */
   glEnable(GL_POLYGON_OFFSET_LINE);
   glPolygonOffset(-1.0f, -1.0f);
   glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
   
   glLineWidth(1.5);
   glDisable(GL_LIGHTING);
   
   draw_bonerlines();
   
   draw_frontlines();
   
   draw_backlines();

   glEnable(GL_LIGHTING);
   glDisable(GL_COLOR_MATERIAL);
   glEnable(GL_CULL_FACE);
   glDisable(GL_POLYGON_OFFSET_LINE);
   glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void draw_vectors(){
   setMaterial(WhiteSolid);
   glBegin(GL_LINES);
   glVertex3d(0,0,0);
   glVertex3d(0,.5,0);

   glVertex3d(0,0,0);
   glVertex3d(0,0,-1);

   glVertex3d(.5,0,0);
   glVertex3d(-.5,0,0);

   glEnd();

   glLineWidth(1.0);
}

/**
 * Draw an adorable little ship in the minimap.
 */
void AsteroidShip::drawInMinimap() {
   glPushMatrix();
   position->glTranslate();
   //shotOrigin->glTranslate();
   // Counteract the rotation done in GameState::drawInMinimap();
   glRotate();
   glScalef(shipScale, shipScale, shipScale);
   //glTranslatef(0, 0, -3);
   glColor4d(0, 0, 0, 0.2);
   draw_ship();
   glPopMatrix();
}


void AsteroidShip::draw() {
   /*
   GLUquadricObj *quadratic;
   float ballx, bally, ballz;
   ballx = 0.0;
   bally = 0.0;
   ballz = 2.0;
   quadratic=gluNewQuadric();
   gluQuadricNormals(quadratic, GLU_SMOOTH);
   */
   glPushMatrix();
   glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
   // Translate to the position.
   position->glTranslate();
   //shotOrigin->glTranslate();
   // Rotate to the current up/right/forward vectors.
   
   glRotate();
   glColor4d(0, 0, 0, 0.4);

   draw_ship();
   /*
   glBegin(GL_POINTS);
   glVertex3f(0.0, 0.0, 0.0);
   glEnd();
   */
   glPopMatrix();
   /*
   Ball ball = Ball();
   ball.attach(this);
   ball.draw();
   */
}

/**
 * This handles our collisions. Duh.
 * We use a series of dynamic_casts to figure out what we've hit.
 */
void AsteroidShip::handleCollision(Drawable* other) {
   Asteroid3D* asteroid;
   Shard* shard;

   // Try converting other into a Shard
   if ((shard = dynamic_cast<Shard*>(other)) != NULL) {
      health += 3;
      if (health > 100) {
         health = 100;
      }
      nShards++;
      score += 69;
      // Try converting other into an Asteroid3D
   } else if ((asteroid = dynamic_cast<Asteroid3D*>(other)) != NULL) {
      // Decrease the player's health by an appropriate amount.
      addInstantAcceleration(new Vector3D(*(asteroid->velocity)));
      if (!gameState->godMode) {
         health -= (int) (4 * ceil(asteroid->radius));
      }
      if(health < 0) {
         health = 0;
      }
      shakeAmount = 8;
      SoundEffect::playSoundEffect("ShipHit.wav");
   }
}

void AsteroidShip::updateShotDirectionVector() {
   shotDirection.updateMagnitude(forward);
   shotDirection.rotate(shotPhi, *right);
   shotDirection.rotate(shotTheta, *up);
}

double AsteroidShip::getAimX() {
   return aimX;
}
double AsteroidShip::getAimY() {
   return aimY;
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

/**
 * This is the crosshair for first person.
 */
void AsteroidShip::drawCrosshair() {
   // If we should be drawing the boxes, do that instead.
   if (currentView != VIEW_FIRSTPERSON_SHIP &&
    currentView != VIEW_FIRSTPERSON_GUN) {
      return drawShotDirectionIndicators();
   }

   double crosshairSizeX = 0.05;
   double crosshairSizeY = 0.05;
   double thicknessX = 0.01;
   double thicknessY = 0.01;
   glPushMatrix();
   glLoadIdentity();

   // Make it white
   setMaterial(WhiteSolid);
   glColor3f(1, 1, 1);
   useOrtho();
   glDisable(GL_LIGHTING);
   /*
      glBegin(GL_QUADS);
      glVertex3f(gameState->ship->getAimX() + crosshairSizeX, gameState->ship->getAimY() + thicknessY, 0);
      glVertex3f(gameState->ship->getAimX() - crosshairSizeX, gameState->ship->getAimY() + thicknessY, 0);
      glVertex3f(gameState->ship->getAimX() - crosshairSizeX, gameState->ship->getAimY() - thicknessY, 0);
      glVertex3f(gameState->ship->getAimX() + crosshairSizeX, gameState->ship->getAimY() - thicknessY, 0);

      glVertex3f(gameState->ship->getAimX() + thicknessX, gameState->ship->getAimY() - crosshairSizeY, 0);
      glVertex3f(gameState->ship->getAimX() + thicknessX, gameState->ship->getAimY() + crosshairSizeY, 0);
      glVertex3f(gameState->ship->getAimX() - thicknessX, gameState->ship->getAimY() + crosshairSizeY, 0);
      glVertex3f(gameState->ship->getAimX() - thicknessX, gameState->ship->getAimY() - crosshairSizeY, 0);
      glEnd();
      */
   if (currentView == VIEW_FIRSTPERSON_SHIP) {
      glTranslatef((GLfloat)getAimX(),(GLfloat) getAimY(),(GLfloat)0.0f);
   }
   static GLUquadricObj *outer;
   static GLUquadricObj *inner;
   outer = gluNewQuadric();
   gluDisk(outer, crosshairSizeX / 2 - thicknessX / 1.5, crosshairSizeX / 2, 12, 1);

   inner = gluNewQuadric();
   gluDisk(inner, 0.0, thicknessX / 1.5, 8, 1);

   glEnable(GL_LIGHTING);
   usePerspective();
   glPopMatrix();
}


/**
 * Draw the double crosshair, starfox style.
 */
void AsteroidShip::drawShotDirectionIndicators() {
   // Don't draw this while firing the tractorbeam.
   // TODO: Make 2 not a magic number.
   if (currentWeapon == 2 && isFiring && weapons[2]->curAmmo > 0 || (gameState->godMode && isFiring)) {
      return;
   }
   // The coords of the boxes.
   Point3D drawPoint = shotOrigin;
   double boxSize = 1.0;
   const double boxDecrement = 0.2;

   glPushMatrix();
   shotDirection.movePoint(drawPoint, 5);
   // Start at top right.
   up->movePoint(drawPoint, boxSize / 2);
   right->movePoint(drawPoint, boxSize / 2);
   glDisable(GL_LIGHTING);
   glDisable(GL_CULL_FACE);
   glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
   setMaterial(WhiteTransparent);

   glLineWidth(3.0);
   glBegin(GL_QUADS);
   // top right
   glColor3d(0.2, 1, 0.0);
   drawPoint.draw();
   up->movePoint(drawPoint, -boxSize);
   drawPoint.draw();
   right->movePoint(drawPoint, -boxSize);
   drawPoint.draw();
   up->movePoint(drawPoint, boxSize);
   drawPoint.draw();

   boxSize -= boxDecrement;

   // Move again
   shotDirection.movePoint(drawPoint, 5);
   // top right
   glColor3d(1, 0.2, 0.0);
   right->movePoint(drawPoint, boxSize);
   drawPoint.draw();
   up->movePoint(drawPoint, -boxSize);
   drawPoint.draw();
   right->movePoint(drawPoint, -boxSize);
   drawPoint.draw();
   up->movePoint(drawPoint, boxSize);
   drawPoint.draw();
   
   boxSize -= boxDecrement;

   // Move again
   shotDirection.movePoint(drawPoint, 5);
   // top right
   glColor3d(0, 0.2, 1);
   right->movePoint(drawPoint, boxSize);
   drawPoint.draw();
   up->movePoint(drawPoint, -boxSize);
   drawPoint.draw();
   right->movePoint(drawPoint, -boxSize);
   drawPoint.draw();
   up->movePoint(drawPoint, boxSize);
   drawPoint.draw();
   glEnd();
   glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
   glLineWidth(1.0);
   glEnable(GL_LIGHTING);
   glEnable(GL_CULL_FACE);
   glPopMatrix();
}

/**
 * Returns a pointer to the currently selected weapon.
 */
Weapon* AsteroidShip::getCurrentWeapon() {
   return weapons[currentWeapon];
}

/*
 * Just tell us what the next weapon up would be, but don't switch to it.
 */
Weapon* AsteroidShip :: getNextWeapon() {
   return weapons[(currentWeapon + 1) %weapons.size()];
}


/*
 * Just tell us what the previous weapon would be, but don't switch to it.
 */
Weapon* AsteroidShip :: getPreviousWeapon() {
   return weapons[(currentWeapon - 1) %weapons.size()];
}

/**
 * Increment the current weapon and mod by the number of weapons.
 */
void AsteroidShip::nextWeapon() {
   currentWeapon = (currentWeapon + 1) % (int) weapons.size();
}

/**
 * Decrement the current weapon and mod by the number of weapons.
 */
void AsteroidShip::prevWeapon() {
   // We add weaons.size() in the middle so that we don't do -1 % something.
   currentWeapon = (currentWeapon + (int) weapons.size() - 1) % (int) weapons.size();
}

// Return a reference to the list of weapons that the ship has.
std::vector<Weapon*> AsteroidShip :: getWeaponsList() {
   return weapons;
}

// Get the number of types of weapons the ship has. They're indexed 0 - (n-1)
int AsteroidShip :: getNumWeapons() {
   return (int) weapons.size();
}

/**
 * Gets the cooldownamount of the currently selected weapon.
 */
float AsteroidShip::getCurrentWeaponCoolDown() {
   return (float) weapons[currentWeapon]->getCoolDownAmount();
}

Weapon* AsteroidShip::getWeapon(int wep) {
   return weapons[wep];
}

float AsteroidShip::getShakeAmount() {
   return shakeAmount;
}

void AsteroidShip::setShakeAmount(float shakeIn) {
   shakeAmount = shakeIn;
}

Vector3D* AsteroidShip::getViewVector() {
   switch(currentView) {
      case VIEW_FIRSTPERSON_SHIP:
      case VIEW_THIRDPERSON_SHIP: return forward;
      case VIEW_FIRSTPERSON_GUN: 
      case VIEW_THIRDPERSON_GUN: return getShotDirection();
      default:
       fprintf(stderr, "getViewVector got currentView %d\n", currentView);
      return forward;
   }
}

Vector3D* AsteroidShip::getCameraOffset() {
   const double thirdPersonForwardOffset = -12;
   const double thirdPersonUpOffset = 2;
   const double firstPersonForwardOffset = 0.9 * shotOriginScale;
   const double firstPersonUpOffset = 0.15;
   if (currentView == VIEW_THIRDPERSON_SHIP || currentView == VIEW_THIRDPERSON_GUN) {
      cameraOffset->updateMagnitude(getViewVector()->scalarMultiply(thirdPersonForwardOffset));
      cameraOffset->addUpdate(up->scalarMultiply(thirdPersonUpOffset));
      cameraOffset->scalarMultiplyUpdate(zoomFactor);
   } else {
      cameraOffset->updateMagnitude(forward->scalarMultiply(firstPersonForwardOffset));
      cameraOffset->addUpdate(up->scalarMultiply(firstPersonUpOffset));
   }
   return cameraOffset;
}

void AsteroidShip::nextView() {
   setView((currentView + 1) % VIEW_COUNT);
}

void AsteroidShip::setView(int _view) {
   if (_view > VIEW_COUNT) {
      fprintf(stderr, "setView got currentView %d\n", currentView);
      _view %= VIEW_COUNT;
   }
   currentView = _view;
}

int AsteroidShip::getCurrentView() {
   return currentView;
}

void AsteroidShip::setCameraDirectly() {
   gluLookAt(position->x, position->y, position->z,
         position->x + forward->x,
         position->y + forward->y,
         position->z + forward->z,
         up->x, up->y, up->z);
}

void AsteroidShip::setZoomSpeed(float speed) {
   zoomSpeed = speed;
}

/**
 * This is used to set up ships initially.
 * Do not use this for moving ships in the game.
 * The vectors should come out as orthogonal. 
 * This sets right to be forward x up.
 * Then up is set to right x forward.
 */
void AsteroidShip::setOrientation(double forwardX, double forwardY, double forwardZ,
 double upX, double upY, double upZ) {
   // Set forward and 
   forward->updateMagnitude(forwardX, forwardY, forwardZ);
   up->updateMagnitude(upX, upY, upZ);

   forward->normalize();
   up->normalize();

   right->updateMagnitude(forward->cross(*up));
   up->updateMagnitude(right->cross(*forward));
}

/**
 * Same as the other setOrientation.
 */
void AsteroidShip::setOrientation(Vector3D& forward, Vector3D& up) {
   setOrientation(forward.x, forward.y, forward.z,
 up.x, up.y, up.z);
}

/**
 * Look at some point with an arbitrary up vector.
 * This is used by the menu, and should not be used in the game. 
 */
void AsteroidShip::lookAt(double lookAtX, double lookAtY, double lookAtZ,
 double upX, double upY, double upZ) {
   Vector3D _forward(lookAtX - position->x,
    lookAtY - position->y,
    lookAtZ - position->z);
   
   Vector3D _up(upX, upY, upZ);

   setOrientation(_forward, _up);
}
