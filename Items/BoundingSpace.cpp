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
   double itemXMax = item->position->x + item->maxX;
   double itemYMax = item->position->y + item->maxY;
   double itemZMax = item->position->z + item->maxZ;
   double itemXMin = item->position->x + item->minX;
   double itemYMin = item->position->y + item->minY;
   double itemZMin = item->position->z + item->minZ;

   if (itemXMax > xMax) {
      item->position->x = xMax - item->maxX;
      item->velocity->negativeX();
   }
   if (itemYMax > yMax) {
      item->position->y = yMax - item->maxY;
      item->velocity->negativeY();
   }
   if (itemZMax > zMax) {
      item->position->z = zMax - item->maxZ;
      item->velocity->negativeZ();
   }

   if (itemXMin < xMin) {
      item->position->x = xMin - item->minX;
      item->velocity->positiveX();
   }
   if (itemYMin < yMin) {
      item->position->y = yMin - item->minY;
      item->velocity->positiveY();
   }
   if (itemZMin < zMin) {
      item->position->z = zMin - item->minZ;
      item->velocity->positiveZ();
   }
}
