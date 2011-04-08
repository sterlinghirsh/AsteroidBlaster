/**
 * Cube constrains objects' motion.
 * Sterling Hirsh / Taylor Arnicar
 * 1-20-11
 * CPE 476
 */

#include "Items/BoundingSpace.h"
#include <math.h>

#ifdef WIN32
#include "Utility/WindowsMathLib.h"
#endif

BoundingSpace::BoundingSpace(double extentIn, double x, double y, double z, const GameState* _gameState) {
   extent = extentIn;
   xMax = x + extent;
   yMax = y + extent;
   zMax = z + extent;
   xMin = x - extent;
   yMin = y - extent;
   zMin = z - extent;

   // 4 performs well, 2 looks better, 1 looks best but is slow.
   squareSize = 2; // this is how big a square should be. Should be a power of 2.

   wallColors[WALL_TOP] = new Color(0, 0, 1);
   wallColors[WALL_FRONT] = new Color(0, 1, 0);
   wallColors[WALL_RIGHT] = new Color(1, 0, 0);
   wallColors[WALL_BOTTOM] = new Color(1, 0, 1);
   wallColors[WALL_BACK] = new Color(0, 1, 1);
   wallColors[WALL_LEFT] = new Color(1, 1, 0);
   
   // Initialize walls.
   for (int i = 0; i < 6; ++i) {
      walls[i] = new BoundingWall((int) squareSize, (int) extent, wallColors[i], i, _gameState);
   }
}

BoundingSpace::~BoundingSpace() {
   for (int i = 0; i < 6; ++i) {
      delete walls[i];
      delete wallColors[i];
   }
}

int BoundingSpace::getNumSquares() {
   return (int) round(extent / squareSize);
}

void BoundingSpace::constrain(Drawable* item) {
   if (!item->shouldConstrain)
      return;

   if (item->maxPosition->x > xMax) {
      // Right
      item->position->x = xMax - item->maxX;
      item->velocity->negativeX();
      walls[WALL_RIGHT]->constrain(item);
   }
   if (item->maxPosition->y > yMax) {
      // Top
      item->position->y = yMax - item->maxY;
      item->velocity->negativeY();
      walls[WALL_TOP]->constrain(item);
   }
   if (item->maxPosition->z > zMax) {
      // Front
      item->position->z = zMax - item->maxZ;
      item->velocity->negativeZ();
      walls[WALL_FRONT]->constrain(item);
   }

   if (item->minPosition->x < xMin) {
      // Left
      item->position->x = xMin - item->minX;
      item->velocity->positiveX();
      walls[WALL_LEFT]->constrain(item);
   }
   if (item->minPosition->y < yMin) {
      // Bottom
      item->position->y = yMin - item->minY;
      item->velocity->positiveY();
      walls[WALL_BOTTOM]->constrain(item);
   }
   if (item->minPosition->z < zMin) {
      // Back
      item->position->z = zMin - item->minZ;
      item->velocity->positiveZ();
      walls[WALL_BACK]->constrain(item);
   }
}

/* Draw the bounding box grid on the world.
*/
void BoundingSpace::draw() {
   const double wall = extent;
   const double alpha = 0;
   glDisable(GL_LIGHTING);
   glDisable(GL_CULL_FACE);

   for (int i = 0; i < 6; ++i) {
      walls[i]->draw();
   }

   glEnable(GL_LIGHTING);
   glEnable(GL_CULL_FACE);


   glUseProgram(0);
   glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
}

void BoundingSpace::drawGlow() {
   for (int i = 0; i < 6; ++i) {
      walls[i]->drawGlow();
   }
}

void BoundingSpace::update(double timeDiff) {
   for (int i = 0; i < 6; ++i) {
      walls[i]->update(timeDiff);
   }
}
