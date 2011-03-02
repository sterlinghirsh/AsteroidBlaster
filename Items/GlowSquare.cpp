/**
 * GlowSquare is a glowing square on the side.
 * @author Sterling Hirsh
 * @date 2/24/2011
 */

#include "Items/GlowSquare.h"
#include <algorithm>
#include <math.h>

float GlowSquare::lifetime = 2;

GlowSquare::GlowSquare(Color* _color,
 float size, float x, float y, float z, int wallID) : color(_color) {
   if (wallID % 3 == 0) {
      // Top or bottom
      p1.update(x, y, z);
      p2.update(x + size, y, z);
      p3.update(x + size, y, z + size);
      p4.update(x, y, z + size);
   } else if (wallID % 3 == 1) {
      // Front or back
      p1.update(x, y, z);
      p2.update(x + size, y, z);
      p3.update(x + size, y + size, z);
      p4.update(x, y + size, z);
   } else {
      // Left or right
      p1.update(x, y, z);
      p2.update(x, y + size, z);
      p3.update(x, y + size, z + size);
      p4.update(x, y, z + size);
   }

   
   /*
   if (x == wallSize || x == -wallSize) {
      x1 = x2 = x;
      y1 = floor(y / size) * size;
      y2 = y1 + size;
      z1 = floor(z / size) * size;
      z2 = z1 + size;
      p1.update(x, y1, z1);
      p2.update(x, y1, z2);
      p3.update(x, y2, z2);
      p4.update(x, y2, z1);
      normal.updateMagnitude(-x / fabs(x), 0, 0);
   } else if (y == wallSize || y == -wallSize) {
      y1 = y2 = y;
      x1 = floor(x / size) * size;
      x2 = x1 + size;
      z1 = floor(z / size) * size;
      z2 = z1 + size;
      p1.update(x1, y, z1);
      p2.update(x1, y, z2);
      p3.update(x2, y, z2);
      p4.update(x2, y, z1);
      normal.updateMagnitude(0, -y / fabs(y), 0);
   } else {
      z1 = z2 = z;
      x1 = floor(x / size) * size;
      x2 = x1 + size;
      y1 = floor(y / size) * size;
      y2 = y1 + size;
      p1.update(x1, y1, z);
      p2.update(x1, y2, z);
      p3.update(x2, y2, z);
      p4.update(x2, y1, z);
      normal.updateMagnitude(0, 0, -z / fabs(z));
   }
   */
}

void GlowSquare::draw() {
   alpha = 1 - clamp(doubleTime() - timeLastHit, 0, 1);
   if (alpha == 0) {
      color->setColorWithAlpha(1);
      glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
      glUseProgram(fadeShader);
   } else {
      glUseProgram(0);
      glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
      color->setColorWithAlpha(alpha);
   }

   glBegin(GL_QUADS);
   p1.draw();
   p2.draw();
   p3.draw();
   p4.draw();
   glEnd();
}

void GlowSquare::update(double timeDiff) {
}

void GlowSquare::hit() {
   timeLastHit = doubleTime();
}
