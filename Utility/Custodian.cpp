/**
 * Custodian - Stores where all the stuff is.
 * @author Sterling Hirsh
 * @date 1-26-11
 * CPE 476
 */

#include "Utility/Custodian.h"
#include "Items/Asteroid3D.h"
#include "Items/Shard.h"
#include "Utility/SoundEffect.h"

#include <algorithm>
#include <assert.h>

#include "Utility/Collision.h"

#include <vector>

template<>
void Collision<AsteroidShip, Asteroid3D>::handleCollision() {
   if ((doubleTime() - a->justGotHit > 1) && 
    !(a->currentWeapon == 4 && a->isFiring && a->weapons[4]->curAmmo > 0)) {
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
   a->health -= b->getDamage(a);
   b->shouldRemove = true;
}

// Specialization for sphere sphere.
template<>
bool detectCollision<CollisionSphere, CollisionSphere>(CollisionSphere* obj1, CollisionSphere* obj2) {
   Vector3D tmp(obj1->getCenter(), obj2->getCenter());
   double radiusSum = obj1->getRadius() + obj2->getRadius();
   return tmp.getComparisonLength() <= radiusSum * radiusSum;
}

// Specialization for box box.
template<>
bool detectCollision<CollisionBox, CollisionBox>(CollisionBox* obj1, CollisionBox* obj2) {
   printf("in specialized detectCollision.\n");
   return !(obj1->getMinPosition().y > obj2->getMaxPosition().y ||
    obj1->getMaxPosition().y < obj2->getMinPosition().y ||
    obj1->getMinPosition().z > obj2->getMaxPosition().z ||
    obj1->getMaxPosition().z < obj2->getMinPosition().z);
}

// Specialization for sphere box.
template<>
bool detectCollision<CollisionSphere, CollisionBox>(CollisionSphere* sphere, CollisionBox* box) {
   Point3D& center = sphere->getCenter();
   Point3D centerToClosestPoint(sphere->getCenter());
   // Set centerToClosestPoint to the closest point on the box.
   centerToClosestPoint.x = clamp(center.x, box->getMinPosition().x, box->getMaxPosition().x);
   centerToClosestPoint.y = clamp(center.y, box->getMinPosition().y, box->getMaxPosition().y);
   centerToClosestPoint.z = clamp(center.z, box->getMinPosition().z, box->getMaxPosition().z);

   // Get the vector from the closest point to the center.
   centerToClosestPoint.subtractUpdate(center);

   // Make sure its distance is less than radius.
   return centerToClosestPoint.getComparisonLength() < (sphere->getRadius() * sphere->getRadius());
}

template<>
bool detectCollision<CollisionBox, CollisionSphere>(CollisionBox* obj1, CollisionSphere* obj2) {
   return detectCollision<CollisionSphere, CollisionBox>(obj2, obj1);
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
