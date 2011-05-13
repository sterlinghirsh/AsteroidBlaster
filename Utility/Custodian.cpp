/**
 * Custodian - Stores where all the stuff is.
 * @author Sterling Hirsh
 * @date 1-26-11
 * CPE 476 
 */

#include "Utility/Custodian.h"
#include "Utility/Collision.h"

#include "Utility/SoundEffect.h"

#include <algorithm>
#include <assert.h>
#include <vector>

#include "Items/Asteroid3D.h"
#include "Items/Shard.h"
#include "Shots/BeamShot.h"
#include "Shots/ProjectileShot.h"
#include "Shots/TractorBeamShot.h"
#include "Shots/ElectricityShot.h"
#include "Shots/AntiInertiaShot.h"
#include "Shots/EnergyShot.h"
#include "Shots/ExplosiveShot.h"
#include "Shots/TimedBombShot.h"

#include "Particles/ElectricityImpactParticle.h"
#include "Particles/BlasterImpactParticle.h"
#include "Particles/TractorAttractionParticle.h"

/**
 * AsteroidShip collisions======================================================
 */
template<>
void Collision<AsteroidShip, AsteroidShip>::handleCollision() {
   if (a->isRespawning()) { return;}
   if (b->isRespawning()) { return;}

   if (a->gameState->inMenu)
      return;

   double d = a->position->distanceFrom(*b->position);
   double combinedRad = a->radius + b->radius;
   if (combinedRad < d) {
      return;
   }
   
   double maxR = std::max(a->radius, b->radius);
   Vector3D* pushOnA = new Vector3D(*b->position, *a->position);
   Vector3D* pushOnB = new Vector3D(pushOnA->scalarMultiply(-1));

   if (d <= maxR) {
      //printf("d <= maxR, d=%f|maxR=%f\n",d,maxR);
      if (d == 0) {
         printf("AsteroidShip is stuck\n");
         pushOnA->randomMagnitude();
      }
      pushOnA->setLength(0.1); // Units per sec per sec.

      *pushOnB = pushOnA->scalarMultiply(-1);

      a->addAcceleration(pushOnA);
      b->addAcceleration(pushOnB);
   }

   double speed_a = a->velocity->getLength();
   double speed_b = b->velocity->getLength();

   Vector3D reflectionAxis(*b->position, *a->position);
   reflectionAxis.normalize();

   double m1 = a->radius; // This Asteroid's Mass.
   double m2 = b->radius; // Other Asteroid's Mass.

   Vector3D* newVelocity_a = new Vector3D(*a->velocity);
   Vector3D* newVelocity_b = new Vector3D(*b->velocity);

   // Added the 0.2 to make stuff lose some energy.
   newVelocity_a->scalarMultiplyUpdate(m1 - m2);
   newVelocity_a->addUpdate(b->velocity->scalarMultiply(2 * m2));
   newVelocity_a->scalarMultiplyUpdate(1 / (m1 + m2));
   
   newVelocity_b->scalarMultiplyUpdate(m2 - m1);
   newVelocity_b->addUpdate(a->velocity->scalarMultiply(2 * m1));
   newVelocity_b->scalarMultiplyUpdate(1 / (m1 + m2));
   
   // We want to only add acceleration away from the other item.
   if (newVelocity_a->dot(*pushOnA) < 0) {
      newVelocity_a->scalarMultiplyUpdate(-1);
   }
   
   if (newVelocity_b->dot(*pushOnB) < 0) {
      newVelocity_b->scalarMultiplyUpdate(-1);
   }
   
   a->addInstantAcceleration(newVelocity_a);
   b->addInstantAcceleration(newVelocity_b);

   Vector3D* reverseVelocity = new Vector3D(*a->velocity);
   reverseVelocity->scalarMultiplyUpdate(-1);
   a->addInstantAcceleration(reverseVelocity);

   reverseVelocity = new Vector3D(*b->velocity);
   reverseVelocity->scalarMultiplyUpdate(-1);
   b->addInstantAcceleration(reverseVelocity);
}

template<>
void Collision<AsteroidShip, Asteroid3D>::handleCollision() {
   if (a->isRespawning()) { return;}
   if (doubleTime() - a->justGotHit > a->invulnerableTime || !(a->spawnInvulnerable)) {
      /* Remove health from both equal to minHealthDeduction + randomHealthDeduction 
       * or b->health or a->health, whichever is smallest.
       */
      const double minHealthDeduction = 50;
      const double randomHealthDeduction = randdouble() * 40;

      double healthToRemove = std::min(std::min(a->health, b->health), 
       minHealthDeduction + randomHealthDeduction);

      a->health -= healthToRemove;
      b->health -= healthToRemove;

      a->shakeAmount = 8;
      SoundEffect::playSoundEffect("ShipHit.wav");

      a->justGotHit = doubleTime();
      a->addInstantAcceleration(new Vector3D(*(b->velocity)));
      a->addInstantAcceleration(new Vector3D(*b->position, *a->position));
      b->addInstantAcceleration(new Vector3D(*(a->velocity)));
      b->addInstantAcceleration(new Vector3D(*a->position, *b->position));
   }
}

template<>
void Collision<AsteroidShip, Shard>::handleCollision() {
   if (a->isRespawning()) { return;}
   a->health += 10;
   if (a->health > a->healthMax)
      a->health = a->healthMax;

   a->nShards++;
   a->curRoundShards++;
   a->score += 69;
   b->shouldRemove = true;
   SoundEffect::playSoundEffect("CrystalCollect.wav");
}

template<>
void Collision<AsteroidShip, ProjectileShot>::handleCollision() {
   if (a->isRespawning()) { return;}
   int particlesToEmit = 10;
   if (a != b->owner  && !(a->spawnInvulnerable)) {
      a->health -= b->getDamage(a);
      a->shakeAmount = 1;
      a->justGotHit = doubleTime();
      SoundEffect::playSoundEffect("BlasterHit.wav");
      for (int i = 0; i <= particlesToEmit; ++i) {
         Point3D* particleStartPoint = new Point3D(*(b->position));
         Vector3D* particleDirection = new Vector3D();
         particleDirection->randomMagnitude();
         particleDirection->setLength(3);
         BlasterImpactParticle::Add(particleStartPoint, particleDirection, b->gameState);
      }
      b->shouldRemove = true;
   }
}

template<>
void Collision<AsteroidShip, BeamShot>::handleCollision() {
   if (a->isRespawning()) { return;}
   if (a != b->owner && !b->hitYet && curFrame - 1 <= b->firstFrame  && !(a->spawnInvulnerable)) {
      //TODO addInstantVelocity b->velocity->scalarMultiply(10)
      a->health -= b->getDamage(a);
      b->hitYet = true;
      b->hitItem = a;
      b->lastHitFrame = curFrame;
      b->owner->score += (int) a->radius * 10;
      b->drawLength = a->position->distanceFrom(*b->position);
      a->shakeAmount = 1;
      a->justGotHit = doubleTime();
      SoundEffect::playSoundEffect("BlasterHit.wav");
   }
}

template<>
void Collision<AsteroidShip, ElectricityShot>::handleCollision() {
   if (a->isRespawning()) { return;}
   const int numElecParticles = 1;
   double hitDistance = 0;

   if (a != b->owner && !b->hitYet  && !(a->spawnInvulnerable)) {

      /* Normal collision detection gets done in CollisionTypes.h. 
       * I can't think of an efficient way to do this, so I'm leaving it here
       * to avoid creating a hack.
       */
      Point3D* closestPoint = a->sphereCollideWithRay(*b->position, *b->velocity, &hitDistance);

      // Sometimes, we decide that it's not a real hit. No big deal.
      if (closestPoint != NULL) {
         b->hitYet = true;
         a->health -= b->getDamage(a);
         a->shakeAmount = 1;
         a->justGotHit = doubleTime();
      
         b->length = hitDistance;
         Vector3D centerToImpactPoint(*a->position, *closestPoint);
         centerToImpactPoint.setLength(5);
         for (int i = 0; i < numElecParticles; ++i) {
            Point3D* particleStartPoint = new Point3D(*closestPoint);
            Vector3D* particleDirection = new Vector3D();
            particleDirection->randomMagnitude();
            particleDirection->setLength(3);
            particleDirection->addUpdate(centerToImpactPoint);
            ElectricityImpactParticle::Add(particleStartPoint, particleDirection, b->gameState);
         }

         delete closestPoint;
      }
   }
}

template<>
void Collision<AsteroidShip, EnergyShot>::handleCollision() {
   if (a->isRespawning()) { return;}
   if (a != b->owner  && !(a->spawnInvulnerable)) {
      a->health -= clamp(b->chargeTime, 0, 5) * 15.0;

      a->velocity->updateMagnitude(0, 0, 0);
      a->acceleration->updateMagnitude(0, 0, 0);

      b->shouldRemove = true;
      SoundEffect::playSoundEffect("BlasterHit.wav");
      if (b->weapon->chargingShot == b) {
         b->weapon->resetChargingShot();
      }
      a->shakeAmount = 1;
      a->justGotHit = doubleTime();
   }
}

template<>
void Collision<AsteroidShip, ExplosiveShot>::handleCollision() {
   if (a->isRespawning()) { return;}

   if (!b->isExploded && a != b->owner) {
      b->shouldExplode = true;
      SoundEffect::playSoundEffect("BlasterHit.wav");
   } else {
      Vector3D* shotToShip = new Vector3D(*b->position, *a->position);
      double distance = shotToShip->getLength() - a->radius;
      double newSpeed = 1000 / ((1 + (distance * distance) / b->explodeRadius + 1) * a->radius);
      shotToShip->setLength(newSpeed);
      a->addInstantAcceleration(shotToShip);
      a->health -= b->getDamage(a);
      a->shakeAmount = 1;
      a->justGotHit = doubleTime();
   }
}

template<>
void Collision<AsteroidShip, TimedBombShot>::handleCollision() {
   return;
   if (a->isRespawning()) { return;}
   if (!b->isExploded && a != b->owner) {
      Vector3D positionToShip(*b->position, *a->position);
      double distance = positionToShip.getLength();
      if (distance < b->seekRadius + a->radius) {
         if (distance > b->collisionRadius + a->radius) {
            Vector3D* attraction = new Vector3D(positionToShip);
            attraction->setLength(20.0);
            b->addAcceleration(attraction);
         } else {
            b->shouldExplode = true;
            SoundEffect::playSoundEffect("BlasterHit.wav");
         }
      }
   } else if (a != b->owner) {
      a->health -= b->getDamage(a);
      Vector3D* shotToShip = new Vector3D(*b->position, *a->position);
      double distance = shotToShip->getLength() - a->radius;
      double newSpeed = 1000 / ((1 + (distance * distance) / b->explodeRadius + 1) * a->radius);
      shotToShip->setLength(newSpeed);
      a->addInstantAcceleration(shotToShip);
      a->shakeAmount = 1;
      a->justGotHit = doubleTime();
   }
}

/**
 * Asteroid3D collisions========================================================
 */
template<>
void Collision<Asteroid3D, ProjectileShot>::handleCollision() {
   const int particlesToEmit = 10;
   
   static Vector3D particleVariation;
   static Vector3D positionDifference;
   const double particleSpeed = 15;
   double speed = 6 / (a->radius);

   b->shouldRemove = true;
   
   a->lastHitShotOwner = b->owner;
   a->health -= b->getDamage(a);

   Vector3D* newAcceleration = new Vector3D(*b->position, *a->position);
   newAcceleration->setLength(speed);
   a->addInstantAcceleration(newAcceleration);
   
   SoundEffect::playSoundEffect("BlasterHit.wav");
   // Make some particles!

   positionDifference.updateMagnitude(*a->position, *b->position);
   positionDifference.setLength(particleSpeed);

   for (int i = 0; i <= particlesToEmit; ++i) {
      particleVariation.randomMagnitude();
      particleVariation.setLength(particleSpeed);
      particleVariation.addUpdate(positionDifference);
      BlasterImpactParticle::Add(new Point3D(*b->position), 
       new Vector3D(particleVariation), b->gameState);
   }
}

template<>
void Collision<Asteroid3D, BeamShot>::handleCollision() {
   if (!b->hitYet && curFrame - 1 <= b->firstFrame) {
      a->velocity->addUpdate(b->velocity->scalarMultiply(10));
      a->lastHitShotOwner = b->owner;
      a->health -= b->getDamage(a);

      b->hitYet = true;
      b->hitItem = a;
      b->lastHitFrame = curFrame;
      b->owner->score += (int) a->radius * 10;
      b->drawLength = a->position->distanceFrom(*b->position);
   }
}

template<>
void Collision<Asteroid3D, TractorBeamShot>::handleCollision() {
   a->velocity->updateMagnitude(b->owner->velocity);
}

template<>
void Collision<Asteroid3D, ElectricityShot>::handleCollision() {
   const int numElecParticles = 1;
   double hitDistance = 0;

   a->lastHitShotOwner = b->owner;
   a->health -= b->getDamage(a);
   
   /* Normal collision detection gets done in CollisionTypes.h. 
    * I can't think of an efficient way to do this, so I'm leaving it here
    * to avoid creating a hack.
    */
   Point3D* closestPoint = a->sphereCollideWithRay(*b->position, *b->velocity, &hitDistance);

   // Sometimes, we decide that it's not a real hit. No big deal.
   if (closestPoint != NULL) {
      b->length = hitDistance;
      Vector3D centerToImpactPoint(*a->position, *closestPoint);
      centerToImpactPoint.setLength(5);
      for (int i = 0; i < numElecParticles; ++i) {
         Point3D* particleStartPoint = new Point3D(*closestPoint);
         Vector3D* particleDirection = new Vector3D();
         particleDirection->randomMagnitude();
         particleDirection->setLength(3);
         particleDirection->addUpdate(centerToImpactPoint);
         ElectricityImpactParticle::Add(particleStartPoint, particleDirection, b->gameState);
      }

      delete closestPoint;
   }
}

template<>
void Collision<Asteroid3D, EnergyShot>::handleCollision() {
   a->energyHitAsteroid = true;
   a->lastHitShotOwner = b->owner;
   a->health -= b->getDamage(a);
   
   // Calculate any remaining damage to be done, and factor it in to the new damage per second.
   // TODO: 5.0 is the time damage is dealt. We may want to put this in energyshot.

   const double energyDamageTime = 5.0;
   double damageTimeLeft = clamp(doubleTime() - a->timeLastHitByEnergy, 0.0, energyDamageTime);
   double damageLeft = a->damagePerSecond * damageTimeLeft;
   a->damagePerSecond = (damageLeft / energyDamageTime) + b->damagePerSecond;
   a->timeLastHitByEnergy = doubleTime();
   a->newVelocity->updateMagnitude(a->velocity);
   a->newAcceleration->updateMagnitude(a->acceleration);
   
   a->velocity->updateMagnitude(0, 0, 0);
   a->acceleration->updateMagnitude(0, 0, 0);

   b->shouldRemove = true;
   SoundEffect::playSoundEffect("BlasterHit.wav");
   if (b->weapon->chargingShot == b) {
      b->weapon->resetChargingShot();
   }
}

template<>
void Collision<Asteroid3D, ExplosiveShot>::handleCollision() {
   a->lastHitShotOwner = b->owner;
   if (!b->isExploded) {
      b->shouldExplode = true;
      SoundEffect::playSoundEffect("BlasterHit.wav");
   } else {
      Vector3D* shotToAsteroid;
      shotToAsteroid = new Vector3D(*b->position, *a->position);
      double distance = shotToAsteroid->getLength() - a->radius;
      double newSpeed = 1000 / ((1 + (distance * distance) / b->explodeRadius + 1) * a->radius);
      shotToAsteroid->setLength(newSpeed);
      a->addInstantAcceleration(shotToAsteroid);
      a->health -= b->getDamage(a);
   }
}

template<>
void Collision<Asteroid3D, TimedBombShot>::handleCollision() {
   a->lastHitShotOwner = b->owner;
   if (!b->isExploded) {
      Vector3D positionToAsteroid(*b->position, *a->position);
      double distance = positionToAsteroid.getLength();
      if (distance < b->seekRadius + a->radius) {
         if (distance > b->collisionRadius + a->radius) {
            Vector3D* attraction = new Vector3D(positionToAsteroid);
            attraction->setLength(20.0);
            b->addAcceleration(attraction);
         } else {
            b->shouldExplode = true;
            SoundEffect::playSoundEffect("BlasterHit.wav");
         }
      }
   } else {
      a->health -= b->getDamage(a);
      Vector3D* shotToAsteroid;
      shotToAsteroid = new Vector3D(*b->position, *a->position);
      double distance = shotToAsteroid->getLength() - a->radius;
      double newSpeed = 1000 / ((1 + (distance * distance) / b->explodeRadius + 1) * a->radius);
      shotToAsteroid->setLength(newSpeed);
      a->addInstantAcceleration(shotToAsteroid);
   }
}

template<>
void Collision<Asteroid3D, Asteroid3D>::handleCollision() {
   double d = a->position->distanceFrom(*b->position);
   double combinedRad = a->radius + b->radius;
   double maxR = std::max(a->radius, b->radius);
   Vector3D* pushOnA = new Vector3D(*b->position, *a->position);
   Vector3D* pushOnB = new Vector3D(pushOnA->scalarMultiply(-1));

   if (d <= maxR) {
      if (d == 0) {
         //printf("stroid is stuck\n");
         pushOnA->randomMagnitude();
      }
      pushOnA->setLength(0.1); // Units per sec per sec.

      *pushOnB = pushOnA->scalarMultiply(-1);

      a->addAcceleration(pushOnA);
      b->addAcceleration(pushOnB);
   }

   double speed_a = a->velocity->getLength();
   double speed_b = b->velocity->getLength();

   Vector3D reflectionAxis(*b->position, *a->position);
   reflectionAxis.normalize();

   double m1 = a->radius; // This Asteroid's Mass.
   double m2 = b->radius; // Other Asteroid's Mass.

   Vector3D* newVelocity_a = new Vector3D(*a->velocity);
   Vector3D* newVelocity_b = new Vector3D(*b->velocity);

   // Added the 0.2 to make stuff lose some energy.
   newVelocity_a->scalarMultiplyUpdate(m1 - m2);
   newVelocity_a->addUpdate(b->velocity->scalarMultiply(2 * m2));
   newVelocity_a->scalarMultiplyUpdate(1 / (m1 + m2 + 0.2));
   
   newVelocity_b->scalarMultiplyUpdate(m2 - m1);
   newVelocity_b->addUpdate(a->velocity->scalarMultiply(2 * m1));
   newVelocity_b->scalarMultiplyUpdate(1 / (m1 + m2 + 0.2));
   
   // We want to only add acceleration away from the other item.
   if (newVelocity_a->dot(*pushOnA) < 0) {
      newVelocity_a->scalarMultiplyUpdate(-1);
   }
   
   if (newVelocity_b->dot(*pushOnB) < 0) {
      newVelocity_b->scalarMultiplyUpdate(-1);
   }
   
   a->addInstantAcceleration(newVelocity_a);
   b->addInstantAcceleration(newVelocity_b);

   Vector3D* reverseVelocity = new Vector3D(*a->velocity);
   reverseVelocity->scalarMultiplyUpdate(-1);
   a->addInstantAcceleration(reverseVelocity);

   reverseVelocity = new Vector3D(*b->velocity);
   reverseVelocity->scalarMultiplyUpdate(-1);
   b->addInstantAcceleration(reverseVelocity);
}

template<>
void Collision<Shard, TractorBeamShot>::handleCollision() {
   const int numParticles = 1;

   // Set the new speed.
   Vector3D* TBshotToShip = new Vector3D(*a->position, *b->owner->position);
   TBshotToShip->setLength(1000 / sqrt((1 + TBshotToShip->getLength())));
   
   // Pull the shard in.
   a->addAcceleration(TBshotToShip);

   // Now do particles.
   Vector3D random;
   for (int i = 0; i < numParticles; ++i) {
      Point3D* particlePosition = new Point3D(*a->position);
      random.randomMagnitude();
      random.movePoint(*particlePosition);
      Vector3D* particleVelocity = new Vector3D(*particlePosition, *b->position);
      particleVelocity->setLength(10); // Speed of the particle.
      particleVelocity->addUpdate(*a->velocity);
      // Make this go toward the ship.
      TractorAttractionParticle::Add(particlePosition, particleVelocity, b->owner->position, a->gameState);
   }
}

template<>
void Collision<Shard, BeamShot>::handleCollision() {
   if (!b->hitYet && curFrame - 1 <= b->firstFrame) {
      double speed = 80; // High speed from hard-hitting railgun.
      a->velocity->updateMagnitude(*b->position, *a->position);
      a->velocity->setLength(speed);
      b->hitYet = true;
      b->hitItem = a;
      b->lastHitFrame = curFrame;
      b->drawLength = a->position->distanceFrom(*b->position);
   }
}

template<>
void Collision<Asteroid3D, Shard>::handleCollision() {
   double speed = a->velocity->getLength();
   b->velocity->updateMagnitude(*a->position, *b->position);
   b->velocity->setLength(speed);
}

template<>
void Collision<Shard, Shard>::handleCollision() {
   Vector3D* pushOnA = new Vector3D(*b->position, *a->position);
   if (pushOnA->getComparisonLength() == 0) {
      pushOnA->randomMagnitude();
   }
   pushOnA->setLength(5);
   a->addAcceleration(pushOnA);
   b->addAcceleration(new Vector3D(pushOnA->scalarMultiply(-1)));
}

template<>
void Collision<Shard, ExplosiveShot>::handleCollision() {
   Vector3D* shotToShard;
   if (!b->isExploded) {
      b->shouldExplode = true;
   } else {
      shotToShard = new Vector3D(*b->position, *a->position);
      double distance = shotToShard->getLength();
      double newSpeed = 100 / ((distance * distance) / b->explodeRadius + 1);
      shotToShard->setLength(newSpeed);
      a->addInstantAcceleration(shotToShard);
   }
}

template<>
void Collision<Shard, Shot>::handleCollision() {
   // Set speed to between the speed of the shot and the current speed.
   double speed = b->velocity->getLength();
   speed += a->velocity->getLength() * 2;
   speed = speed / 3;
   a->velocity->updateMagnitude(*b->position, *a->position);
   a->velocity->setLength(speed);
   b->shouldRemove = true;
}

static std::vector<CollisionBase*>* collisionHandlers = NULL;

Custodian::Custodian(const GameState* _gameState) :
 gameState(_gameState) {
   shardCount = asteroidCount = 0;
   nextID = 0;
   
   if (collisionHandlers == NULL) {
      collisionHandlers = new std::vector<CollisionBase*>();
      
      // Collision for AsteroidShip
      collisionHandlers->push_back(new Collision<AsteroidShip, AsteroidShip>);
      collisionHandlers->push_back(new Collision<AsteroidShip, Asteroid3D>);
      collisionHandlers->push_back(new Collision<AsteroidShip, Shard>);
      collisionHandlers->push_back(new Collision<AsteroidShip, ProjectileShot>);
      collisionHandlers->push_back(new Collision<AsteroidShip, BeamShot>);
      collisionHandlers->push_back(new Collision<AsteroidShip, ElectricityShot>);
      collisionHandlers->push_back(new Collision<AsteroidShip, EnergyShot>);
      collisionHandlers->push_back(new Collision<AsteroidShip, TimedBombShot>);
      collisionHandlers->push_back(new Collision<AsteroidShip, ExplosiveShot>);

      // Collision for Asteroid3D
      collisionHandlers->push_back(new Collision<Asteroid3D, ProjectileShot>);
      collisionHandlers->push_back(new Collision<Asteroid3D, BeamShot>);
      collisionHandlers->push_back(new Collision<Asteroid3D, TractorBeamShot>);
      collisionHandlers->push_back(new Collision<Asteroid3D, ElectricityShot>);
      collisionHandlers->push_back(new Collision<Asteroid3D, EnergyShot>);
      collisionHandlers->push_back(new Collision<Asteroid3D, TimedBombShot>);
      collisionHandlers->push_back(new Collision<Asteroid3D, ExplosiveShot>);
      collisionHandlers->push_back(new Collision<Asteroid3D, Asteroid3D>);
      collisionHandlers->push_back(new Collision<Asteroid3D, Shard>);

      collisionHandlers->push_back(new Collision<Shard, TractorBeamShot>);
      collisionHandlers->push_back(new Collision<Shard, BeamShot>);
      collisionHandlers->push_back(new Collision<Shard, ExplosiveShot>);
      collisionHandlers->push_back(new Collision<Shard, Shot>);
      collisionHandlers->push_back(new Collision<Shard, Shard>);
   }
}

/**
 * Compares by minX in Object3D.
 * Returns true if first argument goes before the second argument.* Sorts nulls last.
 */
static bool compareByMinX (Object3D* item1, Object3D* item2) {
   // If the first item is null, the second item comes first.
   if (item1 == NULL)
      return false;
   // If the second item is null, the first item comes first.
   if (item2 == NULL)
      return true;
   return item1->minPosition->x < item2->minPosition->x;
}

/**
 * Compares by maxX in Object3D.
 * Returns true if first argument goes before the second argument.* Sorts nulls last.
 */
static bool compareByMaxX (Object3D* item1, Object3D* item2) {
   // If the first item is null, the second item comes first.
   if (item1 == NULL)
      return false;
   // If the second item is null, the first item comes first.
   if (item2 == NULL)
      return true;
   return item1->maxPosition->x < item2->maxPosition->x;
}

bool compareByDistance::operator() (CollisionBase* const& lhs, CollisionBase* const& rhs) const {
   return lhs->squaredDistance < rhs->squaredDistance;
}
Object3D* compareByDistance::curObject;

/**
 * Run this after all objects have moved.
 * This sorts all the lists, then calculates each item's rank.
 * All NULL items are pushed to the end of the lists, where 
 * they are removed from the list.
 */
void Custodian::update() {
   Object3D* tempObject;
   // Iterate over objects that need to be added.
   while (objectsToAdd.size() > 0) {
      // Get the first item in the list.
      tempObject = objectsToAdd.front();
      if (tempObject == NULL) {
         printf("Trying to insert null object.\n");
         objectsToAdd.pop_front();
         continue;
      }
      // Run the update function to make sure everything is right.
      tempObject->update(0);
      // Add the item to the sorted lists.
      objectsByMinX.push_back(tempObject);
      objectsByMaxX.push_back(tempObject);
      objectsByID.insert(std::pair<unsigned, Object3D*>(tempObject->id, tempObject));
      // Remove the item.
      objectsToAdd.pop_front();
   }

   // This lets us know where we can stop searching.
   size_t firstNull = objectsByMinX.size();

   // Remove items that should be removed.
   for (unsigned i = 0; i < objectsByMinX.size(); ++i) {
      if (objectsByMinX[i] == NULL)
         continue;
      if (objectsByMinX[i]->shouldRemove) {
         remove(objectsByMinX[i]);
      }
   }

   // Sort. Duh.
   sort(objectsByMinX.begin(), objectsByMinX.end(), compareByMinX);
   sort(objectsByMaxX.begin(), objectsByMaxX.end(), compareByMaxX);

   // Set up ranks in objects.
   for (unsigned i = 0; i < objectsByMinX.size(); ++i) {
      if (objectsByMinX[i] == NULL) {
         firstNull = i;
         break;
      }
      objectsByMinX[i]->minXRank = i;
      objectsByMaxX[i]->maxXRank = i;
   }

   // Remove null pointers from the ends.
   for (size_t i = firstNull; i < objectsByMinX.size(); ++i) {
      objectsByMinX.pop_back();
      objectsByMaxX.pop_back();
   }
   
   assert (objectsByMinX.size() == objectsByMaxX.size());
}

/**
 * Call this on a Object3D object if you want it to be custodian'd.
 * This doesn't add the item right away, but waits until the next udpate 
 * to do it. In effect, items are added to a queue of items to add.
 */
void Custodian::add(Object3D* objectIn) {
   objectsToAdd.push_back(objectIn);

   Asteroid3D* asteroid;
   Shard* shard;
   Shot* shot;
   AsteroidShip* ship;

   if ((asteroid = dynamic_cast<Asteroid3D*>(objectIn)) != NULL) {
      asteroidCount++;
      asteroids.insert(asteroid);
   } else if ((shard = dynamic_cast<Shard*>(objectIn)) != NULL) {
      shardCount++;
      shards.insert(shard);
   } else if ((shot = dynamic_cast<Shot*>(objectIn)) != NULL) {
      shots.insert(shot);
   } else if ((ship = dynamic_cast<AsteroidShip*>(objectIn)) != NULL) {
      ships.insert(ship);
   }
}

/**
 * Remove a Object3D object.
 * This is a private function. To make an object not be in the custodian, set
 * the object's shouldRemove = true.
 */
void Custodian::remove(Object3D* objectIn) {
   objectsByMinX[objectIn->minXRank] = NULL;
   objectsByMaxX[objectIn->maxXRank] = NULL;
   objectsByID.erase(objectIn->id);
   
   Asteroid3D* asteroid;
   Shard* shard;
   Shot* shot;
   AsteroidShip* ship;

   if ((asteroid = dynamic_cast<Asteroid3D*>(objectIn)) != NULL) {
      asteroidCount--;
      asteroids.erase(asteroid);
   } else if ((shard = dynamic_cast<Shard*>(objectIn)) != NULL) {
      shardCount--;
      shards.erase(shard);
   } else if ((shot = dynamic_cast<Shot*>(objectIn)) != NULL) {
      shots.erase(shot);
   } else if ((ship = dynamic_cast<AsteroidShip*>(objectIn)) != NULL) {
      ships.erase(ship);
   }

   delete objectIn;
}

/**
 * This returns a pointer to a new std::list. You must delete
 * if after you use it.
 */
std::set<CollisionBase*, compareByDistance>* Custodian::findCollisions(Object3D* item, bool searchBackwards) {

   compareByDistance::curObject = item;
   // TODO: Order this by distance.
   std::set<CollisionBase*, compareByDistance >* sublist = new std::set<CollisionBase*, compareByDistance>();
   
   CollisionBase* curCollision;
   std::vector<CollisionBase*>::iterator curCollisionHandler;

   // If we were passed a null pointer, don't worry about it, just return an empty list.
   if (item == NULL)
      return sublist;

   std::set<Object3D*, compareByDistance >::iterator iter;
   size_t numElements = objectsByMinX.size();
   Object3D* other;

   if (searchBackwards) {
      /* Start at the current maxX - 1. Check whether elements to 
       * the left have a maxX that is above item's minX.
       */
      for (int i = item->maxXRank - 1; i > 0; --i) {
         other = objectsByMaxX[i];
         if (other == NULL)
            continue;
         if (other->maxPosition->x >= item->minPosition->x) {
            curCollision = NULL;

            for (curCollisionHandler = collisionHandlers->begin(); 
             curCollisionHandler != collisionHandlers->end();
             ++curCollisionHandler) {
               curCollision = (*curCollisionHandler)->tryExecute(item, other);
               if (curCollision != NULL) {
                  sublist->insert(curCollision);
                  break;
               }
            }
         }
         else break;
      }
   }
   
   /* Start at the current minX + 1. Check whether elements to
    * the right have a minX that is below item's maxX.
    */
   for (int i = item->minXRank + 1; i < numElements; ++i) {
      other = objectsByMinX[i];
      if (other == NULL)
         continue;

      if (other->minPosition->x <= item->maxPosition->x) {
         curCollision = NULL;

         for (curCollisionHandler = collisionHandlers->begin(); 
          curCollisionHandler != collisionHandlers->end();
          ++curCollisionHandler) {
            curCollision = (*curCollisionHandler)->tryExecute(item, other);
            if (curCollision != NULL) {
               sublist->insert(curCollision);
               break;
            }
         }
      } else {
         break;
      }
   }

   return sublist;
}

std::vector<Object3D*>* Custodian::getListOfObjects() {
   return &objectsByMinX;
}

void Custodian::clear() {
   objectsByMinX.clear();
   objectsByMaxX.clear();
   objectsByID.clear();

   ships.clear();
   asteroids.clear();
   shards.clear();
   shots.clear();

   asteroidCount = 0;
   shardCount = 0;
}

/**
 * Do things at the end of the level such as remove asteroids/shards
 */
void Custodian::atLevelEnd() {
   Asteroid3D* tempAst;
   Shard* tempShard;
   for (unsigned i = 0; i < objectsByMinX.size(); ++i) {
      if ((tempAst = dynamic_cast<Asteroid3D*>(objectsByMinX[i])) != NULL) {
         tempAst->shouldRemove = true;
      } else if ((tempShard = dynamic_cast<Shard*>(objectsByMinX[i])) != NULL) {
         tempShard->shouldRemove = true;
      }
   }
}

/**
 * Return the current value of nextID and then increment nextID.
 */
unsigned Custodian::getNextID() {
   return nextID++;
}

/**
 * We want to return a null pointer if we search for a bad value.
 * I'm also going to add a debug output, but if anyone needs
 * the debug output, you can remove it.
 */
Object3D* Custodian::operator[] (unsigned i) { 
   std::map<unsigned, Object3D*>::iterator iter;
   iter = objectsByID.find(i);
   if (iter == objectsByID.end())
      return NULL;
   else
      return iter->second;
}
