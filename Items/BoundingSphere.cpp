/**
 * Cube constrains objects' motion.
 * Sterling Hirsh / Taylor Arnicar
 * 1-20-11
 * CPE 476
 */

#include "Items/BoundingSphere.h"

BoundingSphere::BoundingSphere(double radiusIn, double x, double y, double z) :
   BoundingSpace(0, 0, 0, 0) {
      radius = radiusIn;
      cx = x;
      cy = y;
      cz = z;
      center = new Point3D(cx, cy, cz);
   }

BoundingSphere::~BoundingSphere() {
}

void BoundingSphere::constrain(Object3D* item) {
   if (!item->shouldConstrain)
      return;

   const float squareSize = radius / 40; // How big the drawn squares should be.

   double d = item->position->distanceFrom(*center);

   if (d >= radius) {
      //printf("collision with wall at %f, %f, %f\n", *item->position->x, *item->position->y, *item->position->z);
      //printf("collision with wall at %f, %f, %f\n", item->position->x, item->position->y, item->position->z);

      Vector3D *normal = new Vector3D(*center, *item->position);

      //d = item->position->distanceFrom(*center);

      double xNeg, yNeg, zNeg;
      xNeg = yNeg = zNeg = 1.0;
      if (item->position->x < 0) {
         xNeg = -1.0;
      }
      if (item->position->y < 0) {
         yNeg = -1.0;
      }
      if (item->position->z < 0) {
         zNeg = -1.0;
      }
      item->position->x = normal->xMag - xNeg * item->maxX;
      item->position->y = normal->yMag - yNeg * item->maxY;
      item->position->z = normal->zMag - zNeg * item->maxZ;

      glowingSquares.push_back(new GlowSquare(0, 1, 1, squareSize,
               item->position->z, item->position->y, item->position->z));

      //printf("moved to %f, %f, %f (distance of %f)\n", item->position->x, item->position->y, item->position->z,
      //      item->position->distanceFrom(*center));

      normal->normalize();

      item->velocity->scalarMultiplyUpdate(-1);
      item->velocity->reflect(*normal);
   }
}

/* Draw the bounding sphere grid on the world.
*/
void BoundingSphere::draw() {
   glUseProgram(fadeShader);
   const double wall = radius;
   const double alpha = 0;
   //glClearColor(0.0,0.0,0.0,0.0);
   glDisable(GL_LIGHTING);
   glColor4f(0.0, 1.0, 0.0, alpha);
   glutWireSphere(wall, 48, 48);
   //glutWireSphere(wall, 24, 24);
   glColor4f(0.0, 0.0, 1.0, 1.0);
   glutSolidSphere(radius * 0.1, 24, 24);
   glEnable(GL_LIGHTING);
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
   //printf("drew %d squares\n", count);
   glEnd();
   glEnable(GL_CULL_FACE);
   glDisable(GL_LIGHT1);
   glEnable(GL_LIGHT0);
   glDisable(GL_COLOR_MATERIAL);

   glLineWidth(1.0);
}

void BoundingSphere::update(double timeDiff) {
   std::list<GlowSquare*>::iterator glowSquare;
   double curTime = doubleTime();

   for (glowSquare = glowingSquares.begin(); glowSquare != glowingSquares.end();) {
      (*glowSquare)->update(timeDiff);
      if (curTime - (*glowSquare)->timeCreated > GlowSquare::lifetime) {
         glowSquare = glowingSquares.erase(glowSquare);
      } else {
         glowSquare++;
      }
   }
}
