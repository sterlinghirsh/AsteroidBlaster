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

/* Draw the bounding box grid on the world.
*/
void BoundingSpace::draw() {
   const double wall = extent;
   const double alpha = 1;
   glDisable(GL_LIGHTING);
   glBegin(GL_LINES);
   for (double i = -wall; i <= wall; i += wall / 40) {
      glColor4f(0.0, 1.0, 0.0, alpha);
      // Floor
      glVertex3f(-wall, -wall, i);
      glVertex3f(wall, -wall, i);
      glVertex3f(i, -wall, -wall);
      glVertex3f(i, -wall, wall);

      // Ceiling
      glColor4f(0.0, 0.0, 1.0, alpha);
      glVertex3f(-wall, wall, i);
      glVertex3f(wall, wall, i);
      glVertex3f(i, wall, -wall);
      glVertex3f(i, wall, wall);

      // Left Wall
      glColor4f(1.0, 0.0, 1.0, alpha);
      glVertex3f(-wall, -wall, i);
      glVertex3f(-wall, wall, i);
      glVertex3f(-wall, i, -wall);
      glVertex3f(-wall, i, wall);

      // Right Wall
      glColor4f(0.0, 1.0, 1.0, alpha);
      glVertex3f(wall, -wall, i);
      glVertex3f(wall, wall, i);
      glVertex3f(wall, i, -wall);
      glVertex3f(wall, i, wall);

      // Back Wall
      glColor4f(1.0, 1.0, 0.0, alpha);
      glVertex3f(-wall, i, -wall);
      glVertex3f(wall, i, -wall);
      glVertex3f(i, -wall, -wall);
      glVertex3f(i, wall, -wall);

      // Front Wall
      glColor4f(1, 0, 0, alpha);
      glVertex3f(-wall, i, wall);
      glVertex3f(wall, i, wall);
      glVertex3f(i, -wall, wall);
      glVertex3f(i, wall, wall);
   }
   glEnd();
   glEnable(GL_LIGHTING);
}
