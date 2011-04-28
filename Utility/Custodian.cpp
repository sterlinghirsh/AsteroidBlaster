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

template<>
void Collision<AsteroidShip, Asteroid3D>::handleCollision() {
   if ((doubleTime() - a->justGotHit > 1) && 
    !(a->currentWeapon == 4 && a->isFiring && a->weapons[4]->curAmmo > 0)) {
      //a->justGotHit = 1;
      a->health -= b->health;
      a->shakeAmount = 8;
      SoundEffect::playSoundEffect("ShipHit.wav");
      a->justGotHit = doubleTime();
      a->addInstantAcceleration(new Vector3D(*(b->velocity)));
   }

   b->health = 0;
}

template<>
void Collision<AsteroidShip, Shard>::handleCollision() {
   a->health += 10;
   if (a->health > a->healthMax)
      a->health = a->healthMax;

   a->nShards++;
   a->score += 69;
   b->shouldRemove = true;
   SoundEffect::playSoundEffect("CrystalCollect.wav");
}

template<>
void Collision<AsteroidShip, Shot>::handleCollision() {
   if (a != b->owner) {
      a->health -= b->getDamage(a);
      b->shouldRemove = true;
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
   
   // TODO: Refactor this so we do all collision detection elsewhere.
   Point3D* closestPoint = a->sphereCollideWithRay(*b->position, *b->velocity, &hitDistance);

   // Sometimes, we decide that it's not a real hit. No big deal.
   if (closestPoint != NULL) {
      b->length = hitDistance;
      b->velocity->setLength(hitDistance);
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
void Collision<Asteroid3D, AntiInertiaShot>::handleCollision() {
   a->lastHitShotOwner = b->owner;
   Vector3D* newVelocity = new Vector3D(*a->velocity);
   newVelocity->scalarMultiplyUpdate(-0.1);
   a->addInstantAcceleration(newVelocity);
   a->rotationSpeed = std::max(a->rotationSpeed - 0.5, 0.0);
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
void Collision<Asteroid3D, ProjectileShot>::handleCollision() {
   const int particlesToEmit = 10;
   
   static Vector3D particleVariation;
   static Vector3D positionDifference;
   const double particleSpeed = 15;
   double speed = 10 / sqrt(a->radius);

   b->shouldRemove = true;
   
   a->lastHitShotOwner = b->owner;
   a->health -= b->getDamage(a);

   Vector3D* newAcceleration = new Vector3D(*b->position, *a->position);
   a->newAcceleration->setLength(speed);
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
void Collision<Asteroid3D, Shot>::handleCollision() {
   a->lastHitShotOwner = b->owner;
   a->health -= b->getDamage(a);
   b->shouldRemove = true;
}

template<>
void Collision<Asteroid3D, ExplosiveShot>::handleCollision() {
   a->lastHitShotOwner = b->owner;
   if (!b->isExploded) {
      b->shouldExplode = true;
      SoundEffect::playSoundEffect("BlasterHit.wav");
      return;
   }

   a->health -= b->getDamage(a);
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
      return;
   }

   a->health -= b->getDamage(a);
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
void Collision<Shard, AntiInertiaShot>::handleCollision() {
   Vector3D* newVelocity = new Vector3D(*a->velocity);
   newVelocity->scalarMultiplyUpdate(-0.1);
   a->addInstantAcceleration(newVelocity);
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
void Collision<Shard, Shot>::handleCollision() {
   // Set speed to between the speed of the shot and the current speed.
   double speed = b->velocity->getLength();
   speed += a->velocity->getLength() * 2;
   speed = speed / 3;
   a->velocity->updateMagnitude(*b->position, *a->position);
   a->velocity->setLength(speed);
}

static std::vector<CollisionBase*>* collisionHandlers = NULL;

Custodian::Custodian(const GameState* _gameState) :
 gameState(_gameState) {
   shardCount = asteroidCount = 0;
   
   if (collisionHandlers == NULL) {
      collisionHandlers = new std::vector<CollisionBase*>();
      // Initialize collision handlers.
      collisionHandlers->push_back(new Collision<AsteroidShip, Asteroid3D>);
      collisionHandlers->push_back(new Collision<AsteroidShip, Shard>);
      collisionHandlers->push_back(new Collision<AsteroidShip, Shot>);

      collisionHandlers->push_back(new Collision<Asteroid3D, ProjectileShot>);
      collisionHandlers->push_back(new Collision<Asteroid3D, BeamShot>);
      collisionHandlers->push_back(new Collision<Asteroid3D, TractorBeamShot>);
      collisionHandlers->push_back(new Collision<Asteroid3D, ElectricityShot>);
      collisionHandlers->push_back(new Collision<Asteroid3D, AntiInertiaShot>);
      collisionHandlers->push_back(new Collision<Asteroid3D, EnergyShot>);
      collisionHandlers->push_back(new Collision<Asteroid3D, TimedBombShot>);
      collisionHandlers->push_back(new Collision<Asteroid3D, ExplosiveShot>);
      collisionHandlers->push_back(new Collision<Asteroid3D, Shot>);
      
      collisionHandlers->push_back(new Collision<Asteroid3D, Asteroid3D>);
      collisionHandlers->push_back(new Collision<Asteroid3D, Shard>);

      collisionHandlers->push_back(new Collision<Shard, TractorBeamShot>);
      collisionHandlers->push_back(new Collision<Shard, BeamShot>);
      collisionHandlers->push_back(new Collision<Shard, AntiInertiaShot>);
      collisionHandlers->push_back(new Collision<Shard, Shot>);
      collisionHandlers->push_back(new Collision<Shard, Shard>);
   }
}

/**
 * Compares by minX in Object3D.
 * Returns true if first argument goes before the second argument.* Sorts nulls last.
 */
static bool compareByMinX (Drawable* item1, Drawable* item2) {
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
static bool compareByMaxX (Drawable* item1, Drawable* item2) {
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
Drawable* compareByDistance::curObject;

/**
 * Run this after all objects have moved.
 * This sorts all the lists, then calculates each item's rank.
 * All NULL items are pushed to the end of the lists, where 
 * they are removed from the list.
 */
void Custodian::update() {
   Drawable* tempObject;
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
      // Remove the item.
      objectsToAdd.pop_front();
      assert (objectsByMinX.size() == objectsByMaxX.size());
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
}

/**
 * Call this on a Drawable object if you want it to be custodian'd.
 * This doesn't add the item right away, but waits until the next udpate 
 * to do it. In effect, items are added to a queue of items to add.
 */
void Custodian::add(Drawable* objectIn) {
   objectsToAdd.push_back(objectIn);
   if (dynamic_cast<Asteroid3D*>(objectIn) != NULL) {
      asteroidCount++;
   } else if (dynamic_cast<Shard*>(objectIn) != NULL) {
      shardCount++;
   }
}

/**
 * Remove a Drawable object.
 */
void Custodian::remove(Drawable* objectIn) {
   objectsByMinX[objectIn->minXRank] = NULL;
   objectsByMaxX[objectIn->maxXRank] = NULL;
   if (dynamic_cast<Asteroid3D*>(objectIn) != NULL) {
      asteroidCount--;
   } else if (dynamic_cast<Shard*>(objectIn) != NULL) {
      shardCount--;
   }
   delete objectIn;
}

/**
 * This returns a pointer to a new std::list. You must delete
 * if after you use it.
 */
std::set<CollisionBase*, compareByDistance>* Custodian::findCollisions(Drawable* item, bool searchBackwards) {

   compareByDistance::curObject = item;
   // TODO: Order this by distance.
   std::set<CollisionBase*, compareByDistance >* sublist = new std::set<CollisionBase*, compareByDistance>();
   
   CollisionBase* curCollision;
   std::vector<CollisionBase*>::iterator curCollisionHandler;

   // If we were passed a null pointer, don't worry about it, just return an empty list.
   if (item == NULL)
      return sublist;

   std::set<Drawable*, compareByDistance >::iterator iter;
   size_t numElements = objectsByMinX.size();
   Drawable* other;

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

std::vector<Drawable*>* Custodian::getListOfObjects() {
   return &objectsByMinX;
}

void Custodian::clear() {
   objectsByMinX.clear();
   objectsByMaxX.clear();
   asteroidCount = 0;
   shardCount = 0;
}
