/**
 * Cube constrains objects' motion.
 * Sterling Hirsh / Taylor Arnicar
 * 1-20-11
 * CPE 476
 */

#include "Items/BoundingSpace.h"

BoundingSpace::BoundingSpace(double extentIn, double x, double y, double z) {
   extent = extentIn;
   xMax = x + extent;
   yMax = y + extent;
   zMax = z + extent;
   xMin = x - extent;
   yMin = y - extent;
   zMin = z - extent;
}

BoundingSpace::~BoundingSpace() {
}

void BoundingSpace::constrain(Object3D* item) {
   if (item->maxPosition->x > xMax) {
      item->position->x = xMax - item->maxX;
      item->velocity->negativeX();
   }
   if (item->maxPosition->y > yMax) {
      item->position->y = yMax - item->maxY;
      item->velocity->negativeY();
   }
   if (item->maxPosition->z > zMax) {
      item->position->z = zMax - item->maxZ;
      item->velocity->negativeZ();
   }

   if (item->minPosition->x < xMin) {
      item->position->x = xMin - item->minX;
      item->velocity->positiveX();
   }
   if (item->minPosition->y < yMin) {
      item->position->y = yMin - item->minY;
      item->velocity->positiveY();
   }
   if (item->minPosition->z < zMin) {
      item->position->z = zMin - item->minZ;
      item->velocity->positiveZ();
   }
}
