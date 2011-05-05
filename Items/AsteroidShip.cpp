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
#include "Particles/ElectricityImpactParticle.h"
#include "Text/GameMessage.h"

#ifndef M_PI
#define M_PI           3.14159265358979323846
#endif

#define BOOST_SCALE 2.0

using namespace std;
const double rotationFactor = 2.6;
const float shipScale = 5;
const double rotationalAcceleration = 10; // rad/sec^2
const double spawnRate = .5;

/**
 * Constructor
 */
AsteroidShip::AsteroidShip(const GameState* _gameState, int _id) : 
   Object3D(_gameState) {

   cullRadius = 12;
   id = _id;
   health = 100;
   healthMax = 100;

   // Bounding box stuff.
   maxX = maxY = maxZ = 3;
   minX = minY = minZ = -3;
   radius = 3;
   updateBoundingBox();

   // Todo: comment these.
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

   hitX = 0;
   hitY = 0;
   hitZ = 0;

   justGotHit = 0;

   //skew must be set to 0 til I figure out a better way to do things
   skew = 0;

   // TODO: Comment these.
   x2Change = middleXY / 2;
   y2Change = middleXY / 2;
   z2Change = (backZ - middleZ) / 2;
   xChange = 0;
   yChange = 0;
   zChange = 0;
   backChange = 0;

   shotOriginScale = 4;

   // The ship's score. This number is displayed to the screen.
   score = 0;

   // The number of shard collected. This number is displayed to the screen.
   nShards = 0;

   // The ship's max motion parameters.
   maxForwardAccel = 10;
   maxRightAccel = 5;
   maxUpAccel = 5;
   maxYawSpeed = maxPitchSpeed = maxRollSpeed = 3;

   // Create our Radar
   radar = new Radar(this);

   // Add weapons to the list!
   /* IF YOU CHANGE THE ORDER OF THIS LIST, CHANGE THE MAGIC NUMBER 2 for the TractorBeam IN THE FUNCTION drawShotDirectionIndicators below.
   */
   weapons.push_back(new TractorBeam(this));
   weapons.push_back(new Blaster(this));
   weapons.push_back(new RailGun(this));
   weapons.push_back(new Electricity(this));
   //weapons.push_back(new LawnMower(this));
   //weapons.push_back(new Ram(this));
   //weapons.push_back(new AntiInertia(this));
   weapons.push_back(new TimedBomber(this));
   weapons.push_back(new RemoteBomber(this));
   weapons.push_back(new Energy(this));

   soundHandle = -1;

   cameraOffset = new Vector3D(0, 2, 5);
   currentView = VIEW_THIRDPERSON_SHIP;
   zoomFactor = 1.0;
   zoomSpeed = 0.0;

   shouldDrawInMinimap = true;

   collisionType = collisionSphere = new CollisionSphere(4, *position);

   reInitialize();

   /* These must be created last b/c they need the ship / weapons to be
    * initialized first.
    */
   shooter = new ShootingAI(this);
   flyingAI = new FlyingAI(this);
}

/**
 * Destructor
 */
AsteroidShip::~AsteroidShip() {
   for (unsigned int i = 0; i < weapons.size(); ++i) {
      delete (weapons[i]);
   }
   delete shooter;
   delete flyingAI;
   delete radar;
   delete cameraOffset;
}

/**
 * Reset the ship as if it just spawned.
 */
void AsteroidShip::reInitialize() {
   shotDirection.updateMagnitude(0, 0, 1);
   /* Currently not braking or acceleration. */
   isBraking = false;
   isBoosting = false;
   shakeAmount = 0;
   brakeFactor = 2;
   /* We store acceleration as scalars to multiply forward, right, and up by each tick. */
   curForwardAccel = curRightAccel = curUpAccel = 0;
   yawSpeed = rollSpeed = pitchSpeed = 0;
   targetYawSpeed = targetRollSpeed = targetPitchSpeed = 0;
   maxSpeed = 5; // Units/s, probably will be changed with an upgrade.
   maxBoostSpeed = maxSpeed * 1.5; // Units/s, probably will be changed with an upgrade.
   shotOrigin = *position;
   forward->movePoint(shotOrigin, shotOriginScale);

   forward->updateMagnitude(0, 0, 1);
   up->updateMagnitude(0, 1, 0);
   right->updateMagnitude(-1, 0, 0);

   // Is the ship firing? Not when it's instantiated.
   isFiring = false;
   shotPhi = shotTheta = 0;

   // The ship's health. This number is displayed to the screen.
   health = 100;
   healthMax = 100;
   healthUpgradeAmount = 5;
   healthUpgradePrice = 5;
   healthPrice = 2;
   healthAmount = 10;

   regenHealthLevel = 0;
   regenHealthUpgradePrice = 10;
   regenHealthLevelMax = 5;

   //engine upgrade, terminal speed is raised when leved up
   engineUpgrade = 0;
   engineMax = 5;
   enginePrice = 10;

   // The ship's currently selected weapon.
   currentWeapon = 1; // Blaster

   accelerationStartTime = doubleTime();
   particlesEmitted = 0;

   velocity->updateMagnitude(0, 0, 0);
   position->updateMagnitude(0, 0, 0);
   respawnTimer.reset();
   aliveTimer.countUp();
}

/**
 * Set the currently selected weapon to the weapon type specified.
 * The currentWeapon can also be set from nextWeapon() and prevWeapon().
 */
void AsteroidShip::selectWeapon(int weaponType) {
   currentWeapon = weaponType;
   weapons[currentWeapon]->activationTimer.setCountDown(0.5);
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
double AsteroidShip::getHealth() {
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
   targetYawSpeed = rotationFactor * yawAmountIn;
}

void AsteroidShip::setPitchSpeed(double pitchAmountIn) {
   targetPitchSpeed = rotationFactor * pitchAmountIn;
}

void AsteroidShip::setRollSpeed(double rollAmountIn) {
   targetRollSpeed = rotationFactor * rollAmountIn;
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
   if (health <= 0) {
      const double respawnTime = 4;
      shakeAmount = 0;
      // Handle respawning.
      if (!respawnTimer.isRunning) {
         respawnTimer.setCountDown(respawnTime);
         // Make some sparkles when you die!~~~
         for (int i = 0; i < 500; ++i) {
            Point3D* particleStartPoint = new Point3D(*position);
            Vector3D* particleDirection = new Vector3D();
            particleDirection->randomMagnitude();
            particleDirection->setLength(3);
            ElectricityImpactParticle::Add(particleStartPoint, particleDirection, gameState);
            // These may crash everything terribly when a ship explodes.
            //delete particleStartPoint;
            //delete particleDirection;
         }
      }

      double timeLeftToRespawn = respawnTimer.getTimeLeft();

      if (this == gameState->ship) {
         std::ostringstream gameMsg;
         gameMsg << "Respawning in " << (int)(timeLeftToRespawn);

         GameMessage::Add(gameMsg.str(), 30, 0);
      }

      if (gameState->gameIsRunning && respawnTimer.isRunning &&
            timeLeftToRespawn <= 0) {
         reInitialize();
      } else {
         fire(false);
         setRollSpeed(0);
         accelerateForward(0);
         accelerateUp(0);
         accelerateRight(0);
         setYawSpeed(0.0);
         setPitchSpeed(0.0);
         setRollSpeed(0.0);
         return;
      }
   }

   //health regen
   static double healthRegenTimer = 1.0;
   if (healthRegenTimer <= 0) {
      health += regenHealthLevel;
      healthRegenTimer = 1.0;
   }
   healthRegenTimer -= timeDiff;

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

   if (rollSpeed > targetRollSpeed) {
      rollSpeed = clamp(rollSpeed - (timeDiff * rotationalAcceleration),
            targetRollSpeed, rollSpeed);
   } else if (rollSpeed < targetRollSpeed) {
      rollSpeed = clamp(rollSpeed + (timeDiff * rotationalAcceleration),
            rollSpeed, targetRollSpeed);
   }

   if (pitchSpeed > targetPitchSpeed) {
      pitchSpeed = clamp(pitchSpeed - (timeDiff * rotationalAcceleration),
            targetPitchSpeed, pitchSpeed);
   } else if (pitchSpeed < targetPitchSpeed) {
      pitchSpeed = clamp(pitchSpeed + (timeDiff * rotationalAcceleration),
            pitchSpeed, targetPitchSpeed);
   }

   if (yawSpeed > targetYawSpeed) {
      yawSpeed = clamp(yawSpeed - (timeDiff * rotationalAcceleration),
            targetYawSpeed, yawSpeed);
   } else if (yawSpeed < targetYawSpeed) {
      yawSpeed = clamp(yawSpeed + (timeDiff * rotationalAcceleration),
            yawSpeed, targetYawSpeed);
   }

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
   // I don't think anything does fireBackwards.
   if (getCurrentWeapon()->fireBackwards) {
      shotOriginScale = -4;
   } else {
      shotOriginScale = 4;
   }
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

   if (doubleTime() - justGotHit < 2) {
      //justGotHit -= timeDiff;
      drawHit = true;
   } else {
      drawHit = false;
   }

   //printf("Just got hit variable: %d\n", drawHit);

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
   glUseProgram(shipYShader);
   glPushMatrix();
   glBegin(GL_TRIANGLES);
   glVertex3d(frontX, frontY, frontZ);
   glVertex3d(cornerX, cornerY, cornerZ);
   glVertex3d(middleXY, skew, middleZ);

   glVertex3d(frontX, frontY, frontZ);
   glVertex3d(-middleXY, skew, middleZ);
   glVertex3d(-cornerX, cornerY, cornerZ);

   glVertex3d(frontX, frontY, frontZ);
   glVertex3d(-cornerX, -cornerY, cornerZ);
   glVertex3d(-middleXY, skew, middleZ);

   glVertex3d(frontX, frontY, frontZ);
   glVertex3d(middleXY, skew, middleZ);
   glVertex3d(cornerX, -cornerY, cornerZ);
   glEnd();
   glUseProgram(0);

   glUseProgram(shipXShader);
   glBegin(GL_TRIANGLES);
   glVertex3d(frontX, frontY, frontZ);
   glVertex3d(skew, middleXY, middleZ);
   glVertex3d(cornerX, cornerY, cornerZ);

   glVertex3d(frontX, frontY, frontZ);
   glVertex3d(-cornerX, cornerY, cornerZ);
   glVertex3d(skew, middleXY, middleZ);

   glVertex3d(frontX, frontY, frontZ);
   glVertex3d(skew, -middleXY, middleZ);
   glVertex3d(-cornerX, -cornerY, cornerZ);

   glVertex3d(frontX, frontY, frontZ);
   glVertex3d(cornerX, -cornerY, cornerZ);
   glVertex3d(skew, -middleXY, middleZ);
   glEnd();
   glUseProgram(0);

   glPopMatrix();
}

void AsteroidShip::draw_backpanels() {
   glUseProgram(backShader);
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
   glUseProgram(0);
}

void AsteroidShip::draw_spaceboner() {
   glBegin(GL_TRIANGLES);
   if (curForwardAccel == 10.0) {
      glColor4d(1, .4, 0, 1);

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
      glVertex3d(skew, middleXY, middleZ);
      glVertex3d(backX, backY, middleZ + backChange);

   } else if (backChange == 0) {
      glColor4d(0, 0, 0, 1);

      glVertex3d(-middleXY, skew, middleZ);
      glVertex3d(middleXY, skew, middleZ);
      glVertex3d(skew, middleXY, middleZ);

      glVertex3d(-middleXY, skew, middleZ);
      glVertex3d(middleXY, skew, middleZ);
      glVertex3d(skew, -middleXY, middleZ);
   } else {
      glColor4d(1, .4, 0, 1);

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
      glLineWidth(3.0);
      if (backChange == (backZ - middleZ)) {
         glBegin(GL_LINE_LOOP);
         glColor3d(1, 0, 0);
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
      glLineWidth(3.0);
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
   glLineWidth(1.0);
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

   // Make the ship start off flat and expand it to its normal size.
   if (aliveTimer.getTimeRunning() < spawnRate) {
      glScaled(.05, .05, (aliveTimer.getTimeRunning()) / spawnRate);
   } else if (aliveTimer.getTimeRunning() < 2 * spawnRate) {
      glScaled((aliveTimer.getTimeRunning() - spawnRate) / spawnRate, .05, 1);
   } else if (aliveTimer.getTimeRunning() < 3 * spawnRate) {
      glScaled(1, (aliveTimer.getTimeRunning() - 2 * spawnRate) / spawnRate, 1);
   }
   glTranslated(0, 0, -4);
   //printf("Respawn Timer: %f\n", respawnTimer.getTimeRunning());
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

   if(drawHit) {
      //glPushMatrix();
      draw_hitEffect();
      //glPopMatrix();
   }

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
   //glScalef(shipScale, shipScale, shipScale);
   //glTranslatef(0, 0, -3);
   glColor4d(0, 0, 0, 0.2);
   draw_ship();
   glPopMatrix();
}

void AsteroidShip::draw_hitEffect() {
   hitX += (double) (rand() % 10);
   hitY += (double) (rand() % 20);
   hitZ += (double) (rand() % 30);

   double sx, sy, sz;
   sx = 5 / (1.5 * 5);
   sy = 5 / (.5 * 5);
   sz = 5 / (.8 * 5);

   if (hitX > 100) {
      hitX = 0;
   }
   if (hitY > 100) {
      hitY = 0;
   }
   if (hitZ > 100) {
      hitZ = 0;
   }
   glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
   glUseProgram(hitShader);
   glPushMatrix();
   glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
   glDisable(GL_LIGHTING);
   //printf("I got into draw hit effect\n");
   glScaled(sx, sy, sz);

   glTranslated(0, 0, .6);
   glRotated(spin, hitX, hitY, hitZ);
   gluSphere(quadric, .6, 20, 20);
   glEnable(GL_LIGHTING);
   glPopMatrix();
   glUseProgram(0);
}


void AsteroidShip::draw() {
   if (getHealth() <= 0)
      return;

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
   //glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
   // Translate to the position.
   position->glTranslate();
   //shotOrigin->glTranslate();
   // Rotate to the current up/right/forward vectors.

   glRotate();
   spin+= 2;
   glColor4d(0, 0, 0, .8);
   //glRotated(90, 1, 0, 0);
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
   if (getHealth() <= 0)
      return;

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
   if (getHealth() <= 0)
      return;

   // Don't draw this while firing the tractorbeam.
   // TODO: Make 2 not a magic number.
   if (currentWeapon == TRACTOR_WEAPON_INDEX && isFiring || (gameState->godMode && isFiring)) {
      return;
   }
   // The coords of the boxes.
   Point3D drawPoint = shotOrigin;
   double boxSize = shotOriginScale > 0 ? 1.0 : 0.6;
   const double boxDecrement = 0.2;
   const double distanceIncrement = shotOriginScale > 0 ? 5 : 1;

   glPushMatrix();
   shotDirection.movePoint(drawPoint, distanceIncrement);
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
   right->movePoint(drawPoint, boxDecrement / 2.0);

   // Move again
   shotDirection.movePoint(drawPoint, distanceIncrement);
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
   right->movePoint(drawPoint, boxDecrement / 2.0);

   // Move again
   shotDirection.movePoint(drawPoint, distanceIncrement);
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
   //return weapons[(currentWeapon + 1) %weapons.size()];
   //int tmpIndex = std::min(currentWeapon + 1, (int)weapons.size() - 1);
   printf("getNext\n");
   int tmpIndex = std::max(currentWeapon, (currentWeapon + 1) % (int) weapons.size());
   return weapons[tmpIndex];
}


/*
 * Just tell us what the previous weapon would be, but don't switch to it.
 */
Weapon* AsteroidShip :: getPreviousWeapon() {
   //return weapons[(currentWeapon - 1) %weapons.size()];
   //int tmpIndex = std::max(currentWeapon - 1, 0);
   //int tmpIndex = std::min(currentWeapon - 1, (currentWeapon + (int) weapons.size() - 1) % (int) weapons.size());
   printf("getPrev\n");
   int tmpIndex = std::max(currentWeapon - 1, 0);
   return weapons[tmpIndex];
}

/**
 * Find the next available weapon.
 */
void AsteroidShip::nextWeapon() {
   int thisWeapon = currentWeapon;
   while (thisWeapon < weapons.size() - 1) {
      if (weapons[thisWeapon + 1]->purchased) {
         selectWeapon(thisWeapon + 1);
         return;
      } else {
         thisWeapon++;
      }
   }
   if (!weapons[thisWeapon]->purchased) {
      thisWeapon = currentWeapon;
   }
   selectWeapon(thisWeapon);
}

/**
 * Find the previous available weapon.
 */
void AsteroidShip::prevWeapon() {
   int thisWeapon = currentWeapon;
   while (thisWeapon > 0) {
      if (weapons[thisWeapon - 1]->purchased) {
         selectWeapon(thisWeapon - 1);
         return;
      } else {
         thisWeapon--;
      }
   }
   if (!weapons[thisWeapon]->purchased) {
      thisWeapon = currentWeapon;
   }
   selectWeapon(thisWeapon);
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

/**
 * How long has the ship been alive for?
 */
double AsteroidShip::getAliveTime() {
   return aliveTimer.getTimeRunning();
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
   //const double thirdPersonForwardOffset = -12;
   const double thirdPersonForwardOffset = -6;
   const double thirdPersonUpOffset = 1;
   const double firstPersonForwardOffset = 0.5 * shotOriginScale;
   const double firstPersonUpOffset = 0.5;
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

bool AsteroidShip::isRespawning() {
   return respawnTimer.isRunning && respawnTimer.getTimeLeft() > 0;
}

// serialize
std::string AsteroidShip::serialize() {
   return "";
}

