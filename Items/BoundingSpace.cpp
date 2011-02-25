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
   if (!item->shouldConstrain)
      return;

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
   glUseProgram(fadeShader);
   const double wall = extent;
   const double alpha = 0;
   //glClearColor(0.0,0.0,0.0,0.0);
   //glDisable(GL_LIGHTING);
   glBegin(GL_LINES);
   //glEnable(GL_BLEND);
   //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
   const float increment = wall / 40;
   glLineWidth(2.5);
   for (double i = -wall; i <= wall; i += increment) {
      glColor4f(0.0, 1.0, 0.0, alpha);
      // Floor
      for(double j = -wall; j <= wall - 1; j += increment)
      {
            glVertex3f(j, -wall, i);
            glVertex3f(0 + 1, -wall, i);
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

   glBegin(GL_QUADS);
   std::list<GlowSquare*>::iterator glowSquare;
   for (glowSquare = glowingSquares.begin(); glowSquare != glowingSquares.end();
    ++glowSquare) {
      (*glowSquare)->draw();   
   }
   glEnd();

   glEnable(GL_LIGHTING);
   glLineWidth(1.0);
   glUseProgram(0);
}

void BoundingSpace::update(double timeDiff) {
   std::list<GlowSquare*>::iterator glowSquare;
   double curTime = doubleTime();
   double lifeTime = 1;
   for (glowSquare = glowingSquares.begin(); glowSquare != glowingSquares.end();
    ++glowSquare) {
      (*glowSquare)->update(timeDiff);
      if (curTime - (*glowSquare)->timeCreated < lifeTime) {
         glowingSquares.erase(glowSquare);
      }
   }
}
