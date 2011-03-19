/**
 * Custodian - Stores where all the stuff is.
 * @author Sterling Hirsh
 * @date 1-26-11
 * CPE 476
 */

#include "Utility/Custodian.h"
#include "Items/Asteroid3D.h"
#include "Items/Shard.h"
#include <algorithm>
#include <assert.h>

Custodian::Custodian() {
   shardCount = asteroidCount = 0;
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

bool compareByDistance::operator() (Drawable* const& lhs, Drawable* const& rhs) const {
   d1.updateMagnitude(curObject->position, lhs->position);
   d2.updateMagnitude(curObject->position, rhs->position);
   return d1.getComparisonLength() < 
    d2.getComparisonLength();
}
Drawable* compareByDistance::curObject;
Vector3D compareByDistance::d1;
Vector3D compareByDistance::d2;


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
   int firstNull = objectsByMinX.size();

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
   for (unsigned i = firstNull; i < objectsByMinX.size(); ++i) {
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
   objectIn->setCustodian(this);
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
std::set<Drawable*, compareByDistance>* Custodian::findCollisions(Drawable* item, bool searchBackwards) {

   compareByDistance::curObject = item;
   // TODO: Order this by distance.
   std::set<Drawable*, compareByDistance >* sublist = new std::set<Drawable*, compareByDistance>();
   // If we were passed a null pointer, don't worry about it, just return an empty list.
   if (item == NULL)
      return sublist;

   std::set<Drawable*, compareByDistance >::iterator iter;
   int numElements = objectsByMinX.size();
   Drawable* other;

   if (searchBackwards) {
      /* Start at the current maxX - 1. Check whether elements to 
       * the left have a maxX that is above item's minX.
       */
      for (int i = item->maxXRank - 1; i > 0; --i) {
         other = objectsByMaxX[i];
         if (other == NULL)
            continue;
         if (other->maxPosition->x > item->minPosition->x)
            sublist->insert(other);
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
      if (other->minPosition->x < item->maxPosition->x)
         sublist->insert(other);
      else break;
   }

   /* Now check the y direction. From now on, we only 
    * remove elements.
    */
   iter = sublist->begin();
   std::set<Drawable*, compareByDistance >::iterator current;
   while (iter != sublist->end()) {
      current = iter++;
      other = *current;

      /* If the item does not collide, remove it from the sublist.
       * Note: detectCollision may be overridden, as in AsteroidShotBeam.
       */
      if (!item->detectCollision(other, true)) {
         sublist->erase(current);
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
