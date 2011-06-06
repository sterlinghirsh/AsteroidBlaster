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
#include "Network/ClientCommand.h"
#include "Items/Spring.h"

#include <sstream>

#ifndef M_PI
#define M_PI           3.14159265358979323846
#endif

#define BOOST_SCALE 2.0

#define SHOT_ANGLE_FACTOR ((M_PI/180) * VERT_FOV / 2)

int TRACTOR_WEAPON_INDEX = 0;
int BLASTER_WEAPON_INDEX = 0;
int RAILGUN_WEAPON_INDEX = 0;
int ELECTRICITY_WEAPON_INDEX = 0;
int TIMEDBOMBER_WEAPON_INDEX = 0;
int REMOTEBOMBER_WEAPON_INDEX = 0;
int ENERGY_WEAPON_INDEX = 0;
int RAM_WEAPON_INDEX = 0;
int HOMINGMISSILE_WEAPON_INDEX = 0;

int NUMBER_OF_WEAPONS = 0;

using namespace std;
const double rotationFactor = 2.6;
const float shipScale = 5;
const double rotationalAcceleration = 10; // rad/sec^2
const double spawnRate = .5;

/**
 * Constructor
 */
AsteroidShip::AsteroidShip(const GameState* _gameState) :
   Object3D(_gameState) {

      cullRadius = 12;
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

      isBarrelRollingLeft = -1;
      isBarrelRollingRight = -1;

      hitX = 0;
      hitY = 0;
      hitZ = 0;

      spawnInvulnerable = true;
      invulnerableTime = .25;
      justGotHit = 0;
      timeLeftToRespawn = -1;
      isFirstSpawn = true;

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
      kills = 0;
      deaths = 0;
      life = 4;

      lastDamager = NULL;
      lastDamagerWeapon = DAMAGER_INDEX_ASTEROID;

      // The number of shard collected. This number is displayed to the screen.
      nShards = 0;
      bankedShards = 0;
      unbankedShards = 0;
      totalBankedShards = 0;
      bankPeriod = 10; // Default bank period is 10 seconds.
      bankTimer.reset();

      // The ship's max motion parameters.
      maxForwardAccel = 10;
      maxRightAccel = 20;
      maxUpAccel = 5;
      maxYawSpeed = maxPitchSpeed = maxRollSpeed = 3;

      // The ship's health. This number is displayed to the screen.
      health = 100;
      healthMax = 100;
      healthUpgradeAmount = 5;
      healthUpgradePrice = 5;
      healthPrice = 2;
      healthAmount = 10;

      regenHealthLevel = 0;
      regenHealthUpgradePrice = 10;
      regenHealthLevelMax = 10;

      //engine upgrade, terminal speed is raised when leved up
      engineUpgrade = 0;
      engineMax = 5;
      enginePrice = 10;

   // Add weapons to the list!
   /* IF YOU CHANGE THE ORDER OF THIS LIST, CHANGE THE CONSTANTS IN Utility/Constants.h
   */
   int tmpNumberOfWeapons = 0;
   
   weapons.push_back(new TractorBeam(this, tmpNumberOfWeapons++));
   weapons.push_back(new Blaster(this, tmpNumberOfWeapons++));
   weapons.push_back(new RailGun(this, tmpNumberOfWeapons++));
   weapons.push_back(new Electricity(this, tmpNumberOfWeapons++));
   //weapons.push_back(new LawnMower(this, tmpNumberOfWeapons++));
   //weapons.push_back(new AntiInertia(this, tmpNumberOfWeapons++));
   weapons.push_back(new TimedBomber(this, tmpNumberOfWeapons++));
   weapons.push_back(new RemoteBomber(this, tmpNumberOfWeapons++));
   weapons.push_back(new Energy(this, tmpNumberOfWeapons++));
   //weapons.push_back(new Ram(this, tmpNumberOfWeapons++));
   weapons.push_back(new HomingMissile(this, tmpNumberOfWeapons++));

      NUMBER_OF_WEAPONS = tmpNumberOfWeapons;

      // Create this ship's Radar
      radar = new Radar(this);

      soundHandle = -1;

      cameraOffset = new Vector3D(0, 2, 5);
      currentView = VIEW_THIRDPERSON_SHIP;
      zoomFactor = 1.0;
      zoomSpeed = 0.0;

      // Ships should be drawn in the minimap
      shouldDrawInMinimap = true;

      collisionType = collisionSphere = new CollisionSphere(4, *position);

      reInitialize();

      /* These must be created last b/c they need the ship / weapons to be
       * initialized first.
       */
      shooter = new ShootingAI(this);
      flyingAI = new FlyingAI(this);
      
      // The two colors of the ship
      color1 = (float) randdouble();
      color2 = (float) randdouble();
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
   /*shotOrigin = *position;
   forward->movePoint(shotOrigin, shotOriginScale);

   forward->updateMagnitude(0, 0, 1);
   up->updateMagnitude(0, 1, 0);
   right->updateMagnitude(-1, 0, 0);*/

   // Is the ship firing? Not when it's instantiated.
   isFiring = false;
   shotPhi = shotTheta = 0;

   // The ship's currently selected weapon.
   currentWeapon = 1; // Blaster

   health = healthMax;

   accelerationStartTime = doubleTime();
   particlesEmitted = 0;

   velocity->updateMagnitude(0, 0, 0);
   
   int posOrNeg = (rand()%2)*2 - 1;
   int littleBitLessThanHalfOfWorldSize = int(gameState->worldSize / 2) - 4;

   double randX = posOrNeg*((double)(rand() % littleBitLessThanHalfOfWorldSize));//*(gameState->worldSize / 2);
   double randY = posOrNeg*((double)(rand() % littleBitLessThanHalfOfWorldSize));//*(gameState->worldSize / 2);
   double randZ = posOrNeg*((double)(rand() % littleBitLessThanHalfOfWorldSize));//*(gameState->worldSize / 2);
   position->updateMagnitude(randX, randY, randZ);
   shotOrigin = *position;

   forward->updateMagnitude(-position->x, -position->y, -position->z);
   forward->normalize();
   up->updateMagnitude(forward->getNormalVector());
   up->normalize();
   right->updateMagnitude(forward->cross(*up));
   right->normalize();
   forward->movePoint(shotOrigin, shotOriginScale);
   respawnTimer.reset();
   aliveTimer.countUp();
}

/**
 * Set the currently selected weapon to the weapon type specified.
 * The currentWeapon can also be set from nextWeapon() and prevWeapon().
 */
void AsteroidShip::selectWeapon(int weaponType) {
   if (currentWeapon != weaponType) {
      currentWeapon = weaponType;
      weapons[currentWeapon]->activationTimer.setCountDown(0.5);
   }
}

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
   if (curForwardAccel == 0 && dir != 0) {
      accelerationStartTime = doubleTime();
      particlesEmitted = 0;
   }

   curForwardAccel = dir * maxForwardAccel;
   updatePlayerAcceleration();
}

void AsteroidShip::accelerateUp(int dir) {
   if (curUpAccel == 0 && dir != 0) {
      accelerationStartTime = doubleTime();
      particlesEmitted = 0;
   }

   curUpAccel = dir * maxUpAccel;
   updatePlayerAcceleration();
}

void AsteroidShip::accelerateRight(int dir) {
   if (curRightAccel == 0 && dir != 0) {
      accelerationStartTime = doubleTime();
      particlesEmitted = 0;
   }

   curRightAccel = dir * maxRightAccel;
   updatePlayerAcceleration();
}

void AsteroidShip::addNewParticle(Point3D& emitter, Vector3D& baseDirection,
      Vector3D& offsetDirectionX, Vector3D& offsetDirectionY, double color) {
   static Vector3D particleVariation;
   static Point3D curPoint;
   static Vector3D initialOffset;
   static Vector3D randomOffset;
   const float randomAmount = 0.5f;
   curPoint = emitter;

   // Translate the point in 2D
   randomOffset.add(offsetDirectionX.scalarMultiply(randomAmount * (randdouble() - 0.5)));
   randomOffset.add(offsetDirectionY.scalarMultiply(randomAmount * (randdouble() - 0.5)));
   randomOffset.add(baseDirection.scalarMultiply(randomAmount * (randdouble() -0.5)));
   //randomOffset.scalarMultiplyUpdate(0.01);

   particleVariation.updateMagnitude(baseDirection.scalarMultiply(randdouble() * 6));
   particleVariation.addUpdate(offsetDirectionX.scalarMultiply(randdouble() * 8 - 4));
   particleVariation.addUpdate(offsetDirectionY.scalarMultiply(randdouble() * 8 - 4));
   particleVariation.scalarMultiplyUpdate(0.2);
   //curPoint = position->add(randomPoint);
   initialOffset.movePoint(curPoint);
   randomOffset.movePoint(curPoint);
   EngineParticle::Add(new Point3D(curPoint),
         new Vector3D(baseDirection.scalarMultiply(10.0).add(particleVariation)), color, gameState);
}

void AsteroidShip::addNewLowHealthParticle(Point3D& emitter, Vector3D& baseDirection,
      Vector3D& offsetDirectionX, Vector3D& offsetDirectionY, double color) {
   static Vector3D particleVariation;
   static Point3D curPoint;
   static Vector3D initialOffset;
   static Vector3D randomOffset;
   const float randomAmount = 0.5f;
   curPoint = emitter;

   // Translate the point in 2D
   randomOffset.add(offsetDirectionX.scalarMultiply(randomAmount * (randdouble() - 0.5)));
   randomOffset.add(offsetDirectionY.scalarMultiply(randomAmount * (randdouble() - 0.5)));
   randomOffset.add(baseDirection.scalarMultiply(randomAmount * (randdouble() -0.5)));
   //randomOffset.scalarMultiplyUpdate(0.01);

   particleVariation.updateMagnitude(baseDirection.scalarMultiply(randdouble() * 30));
   particleVariation.addUpdate(offsetDirectionX.scalarMultiply(randdouble() * 8 - 4));
   particleVariation.addUpdate(offsetDirectionY.scalarMultiply(randdouble() * 8 - 4));
   particleVariation.scalarMultiplyUpdate(0.2);
   //curPoint = position->add(randomPoint);
   initialOffset.movePoint(curPoint);
   randomOffset.movePoint(curPoint);
   EngineParticle::AddLowHealth(new Point3D(curPoint),
         new Vector3D(baseDirection.scalarMultiply(10.0).add(particleVariation)), color, gameState);
}

void AsteroidShip::createEngineParticles(double timeDiff) {
   //add particles in the opposite direction of the acceration

   int maxParticlesPerFrame = 10;
   int newParticlesPerSecond = 50;
   Vector3D baseParticleAcceleration;
   Point3D emitter;

   double accelerationTime = doubleTime() - accelerationStartTime;
   double colorVariation = 0.2 * randdouble();
   int particlesThisFrame = 0;

   while ((double) particlesEmitted / accelerationTime < newParticlesPerSecond &&
         particlesThisFrame < maxParticlesPerFrame) {
      //printf("Totally getting here %f\n", particlesThisFrame);
      // First do up Acceleration.
      if (curUpAccel != 0) {
         baseParticleAcceleration = up->scalarMultiply(-curUpAccel * 0.2);
         emitter = *position;
         forward->movePoint(emitter, -0.5);
         addNewParticle(emitter, baseParticleAcceleration, *forward, *right, color1 + colorVariation);
      }

      // Next do right Acceleration.
      if (curRightAccel != 0) {
         if(curRightAccel > 10) {
            baseParticleAcceleration = right->scalarMultiply(-10 * 0.2);
         } else if (curRightAccel < -10) {
            baseParticleAcceleration = right->scalarMultiply(10 * 0.2);
         } else {
            baseParticleAcceleration = right->scalarMultiply(-curRightAccel * 0.2);
         }
         emitter = *position;
         forward->movePoint(emitter, -0.7);
         addNewParticle(emitter, baseParticleAcceleration, *forward, *up, color2 + colorVariation);
      }

      // Next do forward Acceleration.
      if (curForwardAccel != 0) {
         // We want to do two streams.
         baseParticleAcceleration = forward->scalarMultiply(-curForwardAccel * 0.05);
         Point3D initialPoint(*position);
         forward->movePoint(initialPoint,  -(curForwardAccel * 0.07));

         // First do the right side.
         right->movePoint(initialPoint, 1);
         addNewParticle(initialPoint, baseParticleAcceleration, *right, *up, color1 - colorVariation);

         // Next do the left side.
         right->movePoint(initialPoint, -2);
         addNewParticle(initialPoint, baseParticleAcceleration, *right, *up, color2 - colorVariation);
      }

      ++particlesEmitted;
      ++particlesThisFrame;
   }
}

void AsteroidShip::createLowHealthParticles(double timeDiff){
   //add particles in the opposite direction of the acceration

   int maxParticlesPerFrame = 8;
   Vector3D baseParticleAcceleration;
   Point3D emitter;

   double colorVariation = 0.2 * randdouble();
   int particlesThisFrame = 0;

   while (particlesThisFrame < maxParticlesPerFrame && particlesThisFrame < ((50 - health) /5)) {
      // First do up Acceleration.
      //if (curUpAccel != 0) {
      //printf("Get here\n");
      baseParticleAcceleration = up->scalarMultiply(.5 * (.5 - randdouble())) + right->scalarMultiply(.5 * (.5 - randdouble()));
      emitter = *position;
      forward->movePoint(emitter, -0.5);
      //up->movePoint(emitter, .1);
      addNewLowHealthParticle(emitter, baseParticleAcceleration, *forward, *right, color1 + colorVariation);
      //}

      // Next do right Acceleration.
      /*if (curRightAccel != 0) {
        baseParticleAcceleration = right->scalarMultiply(-curRightAccel * 0.2);
        emitter = *position;
        forward->movePoint(emitter, -0.7);
        addNewParticle(emitter, baseParticleAcceleration, *forward, *up, color2 + colorVariation);
        }

      // Next do forward Acceleration.
      if (curForwardAccel != 0) {
      // We want to do two streams.
      baseParticleAcceleration = forward->scalarMultiply(-curForwardAccel * 0.05);
      Point3D initialPoint(*position);
      forward->movePoint(initialPoint, -0.7 - (curForwardAccel * 0.02));

      // First do the right side.
      right->movePoint(initialPoint, 1);
      addNewParticle(initialPoint, baseParticleAcceleration, *right, *up, color1 - colorVariation);

      // Next do the left side.
      right->movePoint(initialPoint, -2);
      addNewParticle(initialPoint, baseParticleAcceleration, *right, *up, color2 - colorVariation);
      }*/

      //++particlesEmitted;
      ++particlesThisFrame;
   }
}

void AsteroidShip::update(double timeDiff) {
   if (life == 0) {
      return;
   }
   if (health <= 0) {
      const double respawnTime = 4;
      shakeAmount = 0;
      stopSounds();

      // Handle respawning.
      if (!respawnTimer.isRunning) {
         respawnTimer.setCountDown(respawnTime);
         timeLeftToRespawn = respawnTimer.getTimeLeft();
         ++deaths;
         --life;

         // Make sure to stop barrel rolling.
         isBarrelRollingLeft = -1;
         isBarrelRollingRight = -1;

         if (life <= 0 && this == gameState->ship) {
            gameState->gameOver();
            gameState->usingShipCamera = false;
            return;
         } else if (life <= 0) {
            shouldRemove = true;
         }
         AsteroidShip* lastDamagerShip = dynamic_cast<AsteroidShip*>(lastDamager);
         if (lastDamagerShip != NULL) {
            lastDamagerShip->kills++;
            std::cout << lastDamagerShip->id << " killed " << id << " with a " << weapons[lastDamagerWeapon]->getName() << "." << std::endl;
         } else {
            std::cout << id << " was killed by an asteroid." << std::endl;
         }

         // Update weapons one last time.
         for (std::vector<Weapon*>::iterator iter = weapons.begin();
               iter != weapons.end(); ++iter) {
            (*iter)->update(timeDiff);
         }

         // Fix all the velocities with anything added from the killer.
         Object3D::update(timeDiff);
         // Make some sparkles when you die!~~~
         for (int i = 0; i < 500; ++i) {
            Point3D* particleStartPoint = new Point3D(*position);
            Vector3D* particleDirection = new Vector3D();
            particleDirection->randomMagnitude();
            particleDirection->setLength(3 * randdouble());
            particleDirection->addUpdate(velocity->scalarMultiply(randdouble()));
            ElectricityImpactParticle::Add(particleStartPoint, particleDirection, gameState);
            // These may crash everything terribly when a ship explodes.
            //delete particleStartPoint;
            //delete particleDirection;
         }

         // Release all the shards.
         while (gameState->gsm != ClientMode && unbankedShards > 0) {
            custodian->add(makeShard());
            --nShards;
            --unbankedShards;
         }

         // Make a few more for good measure.
         while(gameState->gsm != ClientMode && rand() % 2 == 0) {
            custodian->add(makeShard());
         }
      }

      timeLeftToRespawn = respawnTimer.getTimeLeft();

      if (this == gameState->ship) {
         std::ostringstream gameMsg;
         gameMsg << "Respawning in " << (int)(timeLeftToRespawn);
         GameMessage::Add(gameMsg.str(), 30, 0);
      }
      if (gameState->gameIsRunning && respawnTimer.isRunning && timeLeftToRespawn <= 1.5) {
         timeLeftToRespawn = 1.5;
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

   if (timeLeftToRespawn > 0 && (gameState->gsm != MenuMode)) {
      if (this == gameState->ship && !isFirstSpawn) {
         std::ostringstream gameMsg;
         gameMsg << "Respawning in " << (int)(timeLeftToRespawn);
         GameMessage::Add(gameMsg.str(), 30, 0);
      }
      timeLeftToRespawn -= timeDiff;
      fire(false);
      setRollSpeed(0);
      accelerateForward(0);
      accelerateUp(0);
      accelerateRight(0);
      setYawSpeed(0.0);
      setPitchSpeed(0.0);
      setRollSpeed(0.0);
      drawSpawn = true;
      return;
   } else {
      drawSpawn = false;
   }

   health += regenHealthLevel * timeDiff;
   if (health > healthMax) {
      health = healthMax;
   }

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

   if (isBarrelRollingLeft <= 0 && isBarrelRollingRight <= 0) {
      roll(timeDiff * rollSpeed);
   }
   pitch(timeDiff * pitchSpeed);
   yaw(timeDiff * yawSpeed);

   if (!shooter->isEnabled()) {
      updateShotDirectionVector();
   }

   // Bank shards.
   if (bankTimer.isRunning) {
      if (unbankedShards <= 0) {
         // This is awkward. Recover silently.
         bankTimer.reset();
      } else if (bankTimer.getTimeLeft() < 0) {
         bankTimer.reset();
         bankedShards++;
         totalBankedShards++;
         unbankedShards--;

         // Play the sound effect only sometimes.
         // Something else special should happen.
         std::ostringstream sstream;

         bool showBankedShardsMessage = false;

         switch (totalBankedShards) {
            case 1: case 5: case 10: case 25: case 50:
               showBankedShardsMessage = true;
               break;
            default:
               if (totalBankedShards % 100 == 0) {
                  showBankedShardsMessage = true;
               }
         }
                  
         if (gameState->ship == this && showBankedShardsMessage) {
            SoundEffect::playSoundEffect("ShardBank", position, true);
            if (totalBankedShards == 1) {
               sstream << "Banked first shard!";
            } else {
               sstream << "Banked " << totalBankedShards << " total shards!";
            }

            GameMessage::Add(sstream.str(), 1, 2);
         }

      }
   } else if (unbankedShards > 0) {
      bankTimer.setCountDown(bankPeriod);
   }

   // Update weapons.
   for (std::vector<Weapon*>::iterator iter = weapons.begin();
         iter != weapons.end(); ++iter) {
      (*iter)->update(timeDiff);
   }

   if ((gameState->gsm != MenuMode) &&
         (curForwardAccel != 0 || curUpAccel != 0 || curRightAccel != 0)) {
      if (soundHandle == -1)
         soundHandle = SoundEffect::playSoundEffect("ShipEngine.wav",
               position, (this == gameState->ship), DEFAULT_VOLUME, true);
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

   if (doubleTime() - justGotHit < invulnerableTime) {
      drawHit = true;
   } else {
      drawHit = false;
   }
   
   if (isBarrelRollingLeft == 1) addInstantAcceleration(new Vector3D(right->scalarMultiply(-20)));
   if (isBarrelRollingRight == 1) addInstantAcceleration(new Vector3D(right->scalarMultiply(20)));
   if (isBarrelRollingLeft > 0) {
      roll(timeDiff * -2 * M_PI);
      /*if (isBarrelRollingLeft > .5) {
         curRightAccel = -300 * isBarrelRollingLeft + 5;
      } else {
         curRightAccel = 300 * isBarrelRollingLeft + 5;
      }*/
      isBarrelRollingLeft -= timeDiff;
   }

   if (isBarrelRollingRight > 0) {
      roll(timeDiff * 2 * M_PI);
      /*if (isBarrelRollingRight > .5) {
         curRightAccel = 300 * isBarrelRollingRight + 5;
      } else {
         curRightAccel = -300 * isBarrelRollingRight + 5;
      }*/
      isBarrelRollingRight -= timeDiff;
   }
   
   /*if (isBarrelRollingLeft > 0) {
      //glRotated((1 - isBarrelRollingLeft) * 360, forward->x, forward->y, forward->z);
      if (isBarrelRollingLeft > .5) {
         //glTranslated(sin(M_PI * (1 - isBarrelRollingLeft)) * 2 * up->x, sin(M_PI * (1 - isBarrelRollingLeft)) * 2 * up->y, sin(M_PI * (1 - isBarrelRollingLeft)) * 2 * up->z);
         shotOrigin.x += 
      } else {
         glTranslated(sin(M_PI * isBarrelRollingLeft) * 2 * up->x, sin(M_PI * isBarrelRollingLeft) * 2 * up->y, sin(M_PI * isBarrelRollingLeft) * 2 * up->z);
      }
   }

   if (isBarrelRollingRight > 0) {
      //glRotated((1 - isBarrelRollingRight) * -360, forward->x, forward->y, forward->z);
      if (isBarrelRollingRight > .5) {
         glTranslated(sin(M_PI * (1 - isBarrelRollingRight)) * 2 * up->x, sin(M_PI * (1 - isBarrelRollingRight)) * 2 * up->y, sin(M_PI * (1 - isBarrelRollingRight)) * 2 * up->z);
      } else {
         glTranslated(sin(M_PI * isBarrelRollingRight) * 2 * up->x, sin(M_PI * isBarrelRollingRight) * 2 * up->y, sin(M_PI * isBarrelRollingRight) * 2 * up->z);
      }
   }*/


   createEngineParticles(timeDiff);
   if(health < 50) createLowHealthParticles(timeDiff);

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
   shotPhi = yOffset * SHOT_ANGLE_FACTOR;
   shotTheta = -xOffset * SHOT_ANGLE_FACTOR;
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
   //fboBegin();
   GLenum buffers[] = {ALBEDO_BUFFER, NORMAL_BUFFER, GLOW_BUFFER, NOLIGHT_BUFFER};
   glDrawBuffers(4, buffers);

   glUseProgram(shipYShader);

   float r, g, b;
   getBrightColor(color1, r, g, b);
   glColor3d(r, g, b);

   glUseProgram(shipYShader);

   glBegin(GL_TRIANGLES);
   // Right panel, top half.
   Point3D p1 = Point3D(frontX, frontY, frontZ);
   Point3D p2 = Point3D(cornerX, cornerY, cornerZ);
   Point3D p3 = Point3D(middleXY, skew, middleZ);
   Vector3D s1 = p1 - p2;
   Vector3D s2 = p3 - p2;
   Vector3D normal = s2.cross(s1);
   normal.addNormal();
   glVertex3d(frontX, frontY, frontZ);
   glVertex3d(cornerX, cornerY, cornerZ);
   glVertex3d(middleXY, skew, middleZ);

   // Left panel, top half.
   p1 = Point3D(frontX, frontY, frontZ);
   p2 = Point3D(-middleXY, skew, middleZ);
   p3 = Point3D(-cornerX, cornerY, cornerZ);
   s1 = p1 - p2;
   s2 = p3 - p2;
   normal = s1.cross(s2);
   normal.addNormal();
   glVertex3d(frontX, frontY, frontZ);
   glVertex3d(-middleXY, skew, middleZ);
   glVertex3d(-cornerX, cornerY, cornerZ);

   // Left panel, bottom half.
   p1 = Point3D(frontX, frontY, frontZ);
   p2 = Point3D(-cornerX, -cornerY, cornerZ);
   p3 = Point3D(-middleXY, skew, middleZ);
   s1 = p1 - p2;
   s2 = p3 - p2;
   normal = s2.cross(s1);
   normal.addNormal();
   glVertex3d(frontX, frontY, frontZ);
   glVertex3d(-cornerX, -cornerY, cornerZ);
   glVertex3d(-middleXY, skew, middleZ);

   // Right panel, bottom half.
   p1 = Point3D(frontX, frontY, frontZ);
   p2 = Point3D(middleXY, skew, middleZ);
   p3 = Point3D(cornerX, -cornerY, cornerZ);
   s1 = p1 - p2;
   s2 = p3 - p2;
   normal = s1.cross(s2);
   normal.addNormal();
   glVertex3d(frontX, frontY, frontZ);
   glVertex3d(middleXY, skew, middleZ);
   glVertex3d(cornerX, -cornerY, cornerZ);
   glEnd();
   glUseProgram(0);

   glUseProgram(shipXShader);
   glBegin(GL_TRIANGLES);

   // Top panel, right half.
   p1 = Point3D(frontX, frontY, frontZ);
   p2 = Point3D(skew, middleXY, middleZ);
   p3 = Point3D(cornerX, cornerY, cornerZ);
   s1 = p1 - p2;
   s2 = p3 - p2;
   normal = s2.cross(s1);
   normal.addNormal();
   glVertex3d(frontX, frontY, frontZ);
   glVertex3d(skew, middleXY, middleZ);
   glVertex3d(cornerX, cornerY, cornerZ);

   // Top panel, left half.
   p1 = Point3D(frontX, frontY, frontZ);
   p2 = Point3D(-cornerX, cornerY, cornerZ);
   p3 = Point3D(skew, middleXY, middleZ);
   s1 = p1 - p2;
   s2 = p3 - p2;
   normal = s2.cross(s1);
   normal.addNormal();
   glVertex3d(frontX, frontY, frontZ);
   glVertex3d(-cornerX, cornerY, cornerZ);
   glVertex3d(skew, middleXY, middleZ);

   // Bottom panel, left half.
   p1 = Point3D(frontX, frontY, frontZ);
   p2 = Point3D(skew, -middleXY, middleZ);
   p3 = Point3D(-cornerX, -cornerY, cornerZ);
   s1 = p1 - p2;
   s2 = p3 - p2;
   normal = s2.cross(s1);
   normal.addNormal();
   glVertex3d(frontX, frontY, frontZ);
   glVertex3d(skew, -middleXY, middleZ);
   glVertex3d(-cornerX, -cornerY, cornerZ);

   // Bottom panel, right half.
   p1 = Point3D(frontX, frontY, frontZ);
   p2 = Point3D(cornerX, -cornerY, cornerZ);
   p3 = Point3D(skew, -middleXY, middleZ);
   s1 = p1 - p2;
   s2 = p3 - p2;
   normal = s2.cross(s1);
   normal.addNormal();
   glVertex3d(frontX, frontY, frontZ);
   glVertex3d(cornerX, -cornerY, cornerZ);
   glVertex3d(skew, -middleXY, middleZ);
   glEnd();
   glUseProgram(0);
   //fboEnd();
}

void AsteroidShip::draw_backpanels() {
   float r, g, b;
   getBrightColor(color2, r, g, b);
   glColor3d(r, g, b);

   //fboBegin();
   GLenum buffers[] = {ALBEDO_BUFFER, NORMAL_BUFFER, GLOW_BUFFER, NOLIGHT_BUFFER};
   glDrawBuffers(4, buffers);

   glUseProgram(backShader);
   glBegin(GL_TRIANGLES);

   // Top right corner panel.
   Point3D p1 = Point3D(middleXY, skew, middleZ);
   Point3D p2 = Point3D(cornerX, cornerY, cornerZ);
   Point3D p3 = Point3D(skew, middleXY, middleZ);
   Vector3D s1 = p1 - p2;
   Vector3D s2 = p3 - p2;
   Vector3D normal = s2.cross(s1);
   normal.addNormal();
   glVertex3d(middleXY, skew, middleZ);
   glVertex3d(cornerX, cornerY, cornerZ);
   glVertex3d(skew, middleXY, middleZ);

   // Bottom right corner panel.
   p1 = Point3D(middleXY, skew, middleZ);
   p2 = Point3D(skew, -middleXY, middleZ);
   p3 = Point3D(cornerX, -cornerY, cornerZ);
   s1 = p1 - p2;
   s2 = p3 - p2;
   normal = s2.cross(s1);
   normal.addNormal();
   glVertex3d(middleXY, skew, middleZ);
   glVertex3d(skew, -middleXY, middleZ);
   glVertex3d(cornerX, -cornerY, cornerZ);

   // Bottom left corner panel.
   p1 = Point3D(-middleXY, skew, middleZ);
   p2 = Point3D(cornerX, -cornerY, cornerZ);
   p3 = Point3D(skew, -middleXY, middleZ);
   s1 = p1 - p2;
   s2 = p3 - p2;
   normal = s2.cross(s1);
   normal.addNormal();
   glVertex3d(-middleXY, skew, middleZ);
   glVertex3d(-cornerX, -cornerY, cornerZ);
   glVertex3d(skew, -middleXY, middleZ);

   // Top left corner panel.
   p1 = Point3D(-middleXY, skew, middleZ);
   p2 = Point3D(skew, middleXY, middleZ);
   p3 = Point3D(-cornerX, cornerY, cornerZ);
   s1 = p1 - p2;
   s2 = p3 - p2;
   normal = s2.cross(s1);
   normal.addNormal();
   glVertex3d(-middleXY, skew, middleZ);
   glVertex3d(skew, middleXY, middleZ);
   glVertex3d(-cornerX, cornerY, cornerZ);

   glEnd();
   glUseProgram(0);
   //fboEnd();
}

void AsteroidShip::draw_spaceboner() {
   //fboBegin();
   if (gameSettings->drawDeferred) {
      GLenum buffers[] = {NORMAL_BUFFER, ALBEDO_BUFFER, GLOW_BUFFER, NOLIGHT_BUFFER};
      glDrawBuffers(4, buffers);

      glUseProgram(bonerShader);
   }
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
   if (gameSettings->drawDeferred) {
      glUseProgram(0);
   }
   //fboEnd();
}

void AsteroidShip::draw_bonerlines() {
   float r, g, b;
   getBrightColor(color2, r, g, b);

   //fboBegin();
   GLenum buffers[] = {GLOW_BUFFER, NOLIGHT_BUFFER};
   glDrawBuffers(2, buffers);

   if (curForwardAccel == 10.0) {
      glLineWidth(1.0);
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
      glColor3d(1, 0, 0);

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
      glColor3d(r, g, b);
      glVertex3d(middleXY, skew, middleZ);
      glVertex3d(skew, middleXY, middleZ);
      glVertex3d(-middleXY, skew, middleZ);
      glVertex3d(skew, -middleXY, middleZ);
      glVertex3d(middleXY, skew, middleZ);
      glEnd();
   } else {
      glLineWidth(1.0);
      glBegin(GL_LINE_LOOP);
      glColor3d(r, g, b);
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
      glColor3d(r, g, b);
      glVertex3d(middleXY, skew, middleZ);
      glVertex3d(skew, middleXY, middleZ);
      glVertex3d(-middleXY, skew, middleZ);
      glVertex3d(skew, -middleXY, middleZ);
      glVertex3d(middleXY, skew, middleZ);
      glEnd();
   }
   glLineWidth(1.0);
   //fboEnd();
}

void AsteroidShip::draw_frontlines() {
   float r, g, b;
   getBrightColor(color1, r, g, b);

   //fboBegin();
   GLenum buffers[] = {ALBEDO_BUFFER, GLOW_BUFFER};
   glDrawBuffers(2, buffers);

   glBegin(GL_LINES);
   glColor3d(r, g, b);
   glVertex3d(frontX, frontY, frontZ);
   glVertex3d(cornerX, cornerY, cornerZ);

   glVertex3d(frontX, frontY, frontZ);
   glVertex3d(-cornerX, cornerY, cornerZ);

   glVertex3d(frontX, frontY, frontZ);
   glVertex3d(-cornerX, -cornerY, cornerZ);

   glVertex3d(frontX, frontY, frontZ);
   glVertex3d(cornerX, -cornerY, cornerZ);
   glEnd();

   //fboEnd();
}

void AsteroidShip::draw_backlines() {
   //fboBegin();
   GLenum buffers[] = {ALBEDO_BUFFER, GLOW_BUFFER};
   glDrawBuffers(2, buffers);

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

   //fboEnd();
}

void AsteroidShip::draw_spawn() {
   if (!isFirstSpawn && drawSpawn) {
      glPushMatrix();
      if (timeLeftToRespawn > 2 * spawnRate) {
         glScaled(.05, .05, (3 * spawnRate - timeLeftToRespawn) / spawnRate);
      } else if (timeLeftToRespawn > spawnRate) {
         glScaled((2 * spawnRate - timeLeftToRespawn) / spawnRate, .05, 1);
      } else if (timeLeftToRespawn > 0) {
         glScaled(1, (spawnRate - timeLeftToRespawn) / spawnRate, 1);
      }
      draw_ship();
      glPopMatrix();
   } else {
      glPushMatrix();
      if (aliveTimer.getTimeRunning() < (spawnRate)) {
         glScaled(.05, .05, (aliveTimer.getTimeRunning()) / spawnRate);
      } else if (aliveTimer.getTimeRunning() < (2 * spawnRate)) {
         glScaled((aliveTimer.getTimeRunning() - spawnRate) / spawnRate, .05, 1);
      } else if (aliveTimer.getTimeRunning() < (3 * spawnRate)) {
         glScaled(1, (aliveTimer.getTimeRunning() - 2 * spawnRate) / spawnRate, 1);
      } else if (aliveTimer.getTimeRunning() > (6 * spawnRate)) {
         isFirstSpawn = false;
      }
      draw_ship();
      glPopMatrix();
   }
   if (drawSpawn) {
      glPushMatrix();
      glTranslated(0, 0, -4);
      glScaled(7.5, 2.5, 4);
      //fboBegin();
      glDrawBuffer(ALBEDO_BUFFER);
      draw_hitEffect();
      //fboEnd();
      glPopMatrix();
   }
}

void AsteroidShip::draw_ship() {
   glPolygonOffset(1.0f, 1.0f);
   glEnable(GL_POLYGON_OFFSET_FILL);
   glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

   glDisable(GL_CULL_FACE);
   glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
   glDisable(GL_LIGHTING);
   glEnable(GL_COLOR_MATERIAL);

   // Make the ship start off flat and expand it to its normal size.

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
   //glDisable(GL_LIGHTING);

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
   glColor4d(0, 0, 0, 1);
   /*if (isBarrelRollingLeft > 0) {
      if (isBarrelRollingLeft > .5) {
         glTranslated(0, sin(M_PI * (1 - isBarrelRollingLeft)) * 2, 0);
      } else {
         glTranslated(0, sin(M_PI * isBarrelRollingLeft) * 2, 0);
      }

      glRotated((1 - isBarrelRollingLeft) * 360, 0, 0, 1);
   }

   if (isBarrelRollingRight > 0) {
      if (isBarrelRollingRight > .5) {
         glTranslated(0, sin(M_PI * (1 - isBarrelRollingRight)) * 2, 0);
      } else {
         glTranslated(0, sin(M_PI * isBarrelRollingRight) * 2, 0);
      }

      glRotated((1 - isBarrelRollingRight) * -360, 0, 0, 1);
   }*/
   //glRotated(90, 1, 0, 0);

   if (drawSpawn && !(gameState->gsm == MenuMode)) {
      spawnInvulnerable = true;
      if (timeLeftToRespawn < 1.5) {
         glPushMatrix();
         draw_spawn();
         glPopMatrix();
      }
   } else if (isFirstSpawn && timeLeftToRespawn == -1) {
      timeLeftToRespawn = 1.5;
   } else {
      spawnInvulnerable = false;
      draw_ship();
      if(drawHit) {
         draw_hitEffect();
      }
   }
   if (aliveTimer.getTimeRunning() > (6 * spawnRate)) {
      isFirstSpawn = false;
   }

   glPopMatrix();
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

void AsteroidShip::updateShotDirection(Vector3D& dir) {
   shotDirection = dir;
}

Radar* AsteroidShip::getRadar() {
   return radar;
}

/**
 * This is the crosshair for first person.
 */
void AsteroidShip::drawCrosshair() {
   if (getHealth() <= 0 || timeLeftToRespawn > 0) {
      return;
   }

   // If we should be drawing the boxes, do that instead.
   if (currentView != VIEW_FIRSTPERSON_SHIP &&
         currentView != VIEW_FIRSTPERSON_GUN) {
      return drawShotDirectionIndicators();
   }

   double crosshairSizeX = 0.05;
   double thicknessX = 0.01;
   glPushMatrix();
   glLoadIdentity();

   // Make it white
   setMaterial(WhiteSolid);
   glColor3f(1, 1, 1);
   useOrtho();
   glDisable(GL_LIGHTING);

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
   if (isFiring && (currentWeapon == TRACTOR_WEAPON_INDEX || gameState->godMode)) {
      return;
   }
   // The coords of the boxes.
   Point3D drawPoint = shotOrigin;
   double boxSize = shotOriginScale > 0 ? 1.0 : 0.6;
   const double boxDecrement = 0.2;
   const double distanceIncrement = shotOriginScale > 0 ? 5 : 1;

   glPushMatrix();
   /*if (isBarrelRollingLeft > 0) {
      //glRotated((1 - isBarrelRollingLeft) * 360, forward->x, forward->y, forward->z);
      if (isBarrelRollingLeft > .5) {
         glTranslated(sin(M_PI * (1 - isBarrelRollingLeft)) * 2 * up->x, sin(M_PI * (1 - isBarrelRollingLeft)) * 2 * up->y, sin(M_PI * (1 - isBarrelRollingLeft)) * 2 * up->z);
      } else {
         glTranslated(sin(M_PI * isBarrelRollingLeft) * 2 * up->x, sin(M_PI * isBarrelRollingLeft) * 2 * up->y, sin(M_PI * isBarrelRollingLeft) * 2 * up->z);
      }
   }

   if (isBarrelRollingRight > 0) {
      //glRotated((1 - isBarrelRollingRight) * -360, forward->x, forward->y, forward->z);
      if (isBarrelRollingRight > .5) {
         glTranslated(sin(M_PI * (1 - isBarrelRollingRight)) * 2 * up->x, sin(M_PI * (1 - isBarrelRollingRight)) * 2 * up->y, sin(M_PI * (1 - isBarrelRollingRight)) * 2 * up->z);
      } else {
         glTranslated(sin(M_PI * isBarrelRollingRight) * 2 * up->x, sin(M_PI * isBarrelRollingRight) * 2 * up->y, sin(M_PI * isBarrelRollingRight) * 2 * up->z);
      }
   }*/
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


   // Move again
   shotDirection.movePoint(drawPoint, distanceIncrement);
   // top right
   glColor3d(1, 1, 0.2);
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
   glColor3d(1, 0.2, 1.0);
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


   shotDirection.movePoint(drawPoint, distanceIncrement);
   // top right
   glColor3d(.2, 1, 1);
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
   selectWeapon(getNextWeaponID());
}

int AsteroidShip::getNextWeaponID() {
   unsigned thisWeapon = currentWeapon;
   while (thisWeapon < weapons.size() - 1) {
      if (weapons[thisWeapon + 1]->purchased) {
         return thisWeapon + 1;
      } else {
         thisWeapon++;
      }
   }
   if (!weapons[thisWeapon]->purchased) {
      thisWeapon = currentWeapon;
   }
   return thisWeapon;
}

/**
 * Find the previous available weapon.
 */
void AsteroidShip::prevWeapon() {
   selectWeapon(getPrevWeaponID());
}

int AsteroidShip::getPrevWeaponID() {
   int thisWeapon = currentWeapon;
   while (thisWeapon > 0) {
      if (weapons[thisWeapon - 1]->purchased) {
         return thisWeapon - 1;
      } else {
         thisWeapon--;
      }
   }
   if (!weapons[thisWeapon]->purchased) {
      thisWeapon = currentWeapon;
   }
   return thisWeapon;
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
   // This is in spring.
   const double thirdPersonUpOffset = 0;
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
   if (currentView == VIEW_FIRSTPERSON_SHIP)
      currentView = VIEW_THIRDPERSON_SHIP;
   else
      currentView = VIEW_FIRSTPERSON_SHIP;
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

/**
 * Used by Radar to move the camera to the shot origin, so that VFC for
 * the ShootingAI will only target things it could see.
 */
void AsteroidShip::setCameraToShotOrigin() {
   gluLookAt(shotOrigin.x, shotOrigin.y, shotOrigin.z,
         shotOrigin.x + forward->x,
         shotOrigin.y + forward->y,
         shotOrigin.z + forward->z,
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
   //printf("Is it respawning? : %d\n", respawnTimer.isRunning && (respawnTimer.getTimeLeft() + spawnRate) > 0);
   return respawnTimer.isRunning && respawnTimer.getTimeLeft() > 0 && timeLeftToRespawn > 0;
}

// serialize
std::string AsteroidShip::serialize() {
   return "";
}

Shard* AsteroidShip::makeShard() {
   Vector3D randomOffset;
   randomOffset.randomMagnitude();
   randomOffset.scalarMultiplyUpdate(3);

   Shard* shard;
   shard = new Shard(0.5, gameState->worldSize, gameState);
   shard->position->clone(position);
   randomOffset.movePoint(*shard->position);
   shard->velocity->updateMagnitude(position, shard->position);
   shard->velocity->scalarMultiplyUpdate(8 * randdouble());
   return shard;
}

void AsteroidShip::readCommand(ClientCommand& command) {
   accelerateForward(command.forwardAcceleration);
   accelerateRight(command.rightAcceleration);
   accelerateUp(command.upAcceleration);

   setYawSpeed(command.yawSpeed);
   setRollSpeed(command.rollSpeed);
   setPitchSpeed(command.pitchSpeed);

   fire(command.fire);

   setBrake(command.brake);

   selectWeapon(command.currentWeapon);

   // This works as long as VERT_FOV is the same on both sides.
   updateShotDirection(command.mouseX, command.mouseY);

   if (command.rightAcceleration == -1 && isBarrelRollingLeft < 0 && isBarrelRollingRight < 0){
      isBarrelRollingLeft = 1;
   }
   
   if (command.rightAcceleration == 1 && isBarrelRollingLeft < 0 && isBarrelRollingRight < 0) {
      isBarrelRollingRight = 1;
   }

}

/**
 * This is called on the ship when the level ends.
 */
void AsteroidShip::atLevelEnd() {
   SoundEffect::playSoundEffect("ShardBank", position, true);
   bankedShards += unbankedShards;
   totalBankedShards += unbankedShards;

   unbankedShards = 0;
   health = healthMax;
   shakeAmount = 0;

   stopSounds();
}

void AsteroidShip::stopSounds() {
   if (soundHandle != -1) {
      SoundEffect::stopSoundEffect(soundHandle);
      soundHandle = -1;
   }

   for (unsigned i = 0; i < weapons.size(); ++i) {
      weapons[i]->stopSounds();
   }
}

