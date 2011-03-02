/**
 * Cube constrains objects' motion.
 * Sterling Hirsh / Taylor Arnicar
 * 1-20-11
 * CPE 476
 */

#include "Items/BoundingSpace.h"
#include <math.h>

BoundingSpace::BoundingSpace(double extentIn, double x, double y, double z) {
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
      walls[i] = new BoundingWall(squareSize, extent, wallColors[i], i);
   }
}

BoundingSpace::~BoundingSpace() {
}

int BoundingSpace::getNumSquares() {
   return round(extent / squareSize);
}

void BoundingSpace::constrain(Object3D* item) {
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
   return;
   // Old stuff.
   //glClearColor(0.0,0.0,0.0,0.0);
   //glDisable(GL_LIGHTING);
   glBegin(GL_LINES);
   //glEnable(GL_BLEND);
   //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
   
   // NOTE: When you put wall / 20, it makes these sweet Ls instead of boxes.
   const float increment = wall / 20;
   glLineWidth(2.5);
   for (double i = -wall; i <= wall; i += increment) {
      glColor4f(0.0, 1.0, 0.0, alpha);
      // Floor
      for(double j = -wall; j <= wall - 1; j += increment)
      {
            glVertex3f(j, -wall, i);
            glVertex3f(j + 1, -wall, i);
      }
      
      for(double j = -wall; j <= wall - 1; j += increment)
      {
            glVertex3f(i, -wall, j);
            glVertex3f(i, -wall, j + 1);
      }

      // Ceiling
      glColor4f(0.0, 0.0, 1.0, alpha);
      for(double j = -wall; j <= wall - 1; j += increment)
      {
            glVertex3f(j, wall, i);
            glVertex3f(j +1, wall, i);
      }
      
      for(double j = -wall; j <= wall - 1; j += increment)
      {
            glVertex3f(i, wall, j);
            glVertex3f(i, wall, j + 1);
      }
      
      
      // Left Wall
      glColor4f(1, 0.0, 1.0, alpha);
      for(double j = -wall; j <= wall - 1; j += increment)
      {
            glVertex3f(-wall, j, i);
            glVertex3f(-wall, j + 1, i);
      }

      for(double j = -wall; j <= wall - 1; j += increment)
      {
            glVertex3f(-wall, i, j);
            glVertex3f(-wall, i, j + 1);
      }
      // Right Wall
      glColor4f(0.0, 1.0, 1.0, alpha);
      for(double j = -wall; j <= wall - 1; j += increment)
      {
            glVertex3f(wall, j, i);
            glVertex3f(wall, j + 1, i);
      }
      
      for(double j = -wall; j <= wall - 1; j += increment)
      {
            glVertex3f(wall, i, j);
            glVertex3f(wall, i, j + 1);
      }

      // Back Wall
      glColor4f(1.0, 1.0, 0.0, alpha);
      for(double j = -wall; j <= wall - 1; j += increment)
      {
            glVertex3f(j, i, -wall);
            glVertex3f(j + 1, i, -wall);
      }
      
      for(double j = -wall; j <= wall - 1; j += increment)
      {
            glVertex3f(i, j, -wall);
            glVertex3f(i, j + 1, -wall);
      }

      // Front Wall
      glColor4f(1, 0, 0, alpha);
      for(double j = -wall; j <= wall - 1; j += increment)
      {
            glVertex3f(j, i, wall);
            glVertex3f(j + 1, i, wall);
      }
      
      for(double j = -wall; j <= wall - 1; j += increment)
      {
            glVertex3f(i, j, wall);
            glVertex3f(i, j + 1, wall);
      }
   }
   glEnd();
   glUseProgram(0);

   glEnable(GL_LIGHTING);
   glDisable(GL_LIGHT0);
   glEnable(GL_LIGHT1);
   glEnable(GL_BLEND);
   glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
   setMaterial(BlackSolid);
   glEnable(GL_COLOR_MATERIAL);
   glDisable(GL_CULL_FACE);
   glBegin(GL_QUADS);
   std::list<GlowSquare*>::iterator glowSquare;
   int count = 0;
   for (glowSquare = glowingSquares.begin(); glowSquare != glowingSquares.end();
    ++glowSquare) {
      (*glowSquare)->draw();   
      count++;
   }
   glEnd();
   glEnable(GL_CULL_FACE);
   glDisable(GL_LIGHT1);
   glEnable(GL_LIGHT0);
   glDisable(GL_COLOR_MATERIAL);

   glLineWidth(1.0);
}

void BoundingSpace::update(double timeDiff) {
   for (int i = 0; i < 6; ++i) {
      walls[i]->update(timeDiff);
   }
}
