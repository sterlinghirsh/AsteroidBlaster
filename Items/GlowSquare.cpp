/**
 * GlowSquare is a glowing square on the side.
 * @author Sterling Hirsh
 * @date 2/24/2011
 */

#include "Items/GlowSquare.h"
#include "Utility/GlobalUtility.h"
#include <algorithm>
#include <math.h>

float GlowSquare::lifetime = 2;

GlowSquare::GlowSquare(float _r, float _g, float _b,
 float size, float x, float y, float z) : r(_r), g(_g), b(_b) {
   float x1, x2, y1, y2, z1, z2;
   float wallSize = 40;
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
   alpha = 1;
   timeCreated = doubleTime();
}

void GlowSquare::draw() {
   alpha = (lifetime - (doubleTime() - timeCreated));
   // glBegin(GL_QUADS); // Called from BoundingSpace::draw()
   glColor4f(r, g, b, alpha);
   p1.draw();
   p2.draw();
   p3.draw();
   p4.draw();
}

void GlowSquare::update(double timeDiff) {
}
