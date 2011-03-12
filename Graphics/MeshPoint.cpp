/**
 * Sterling Hirsh
 * CPE 476
 * 1-16-11
 *
 * MeshPoint: A class to store a point in 3D space for use in a Mesh3D.
 */

#include "Graphics/MeshPoint.h"
#include <math.h>

MeshPoint::MeshPoint(float xIn, float yIn, float zIn) : Point3D(xIn, yIn, zIn) { 
   normal.updateMagnitude(0, 0, 0);
   //setTexPoint(0.0, 0.0);
   /*
   float u, v;
   float uDen = (sqrt(xIn*xIn + yIn*yIn + zIn*zIn));
   if (uDen == 0.0)
      u = 0.0;
   else
      u = xIn / uDen;
   float vDen = (sqrt(xIn*xIn + yIn*yIn + zIn*zIn));
   if (vDen == 0.0)
      v = 0.0;
   else
      v = xIn / uDen;
   */
   //printf("u: %f, v: %f\n", u, v);
   //setTexPoint(xIn, yIn);
}

void MeshPoint::setTexPoint(float xIn, float yIn) {
   texX = xIn;
   texY = yIn;
   texDefined = true;
}

void MeshPoint::draw(bool drawNormal, bool drawTex) {
   if (drawNormal)
      normal.addNormal();
   //if (drawTex && texDefined)
      //glTexCoord2f(texX, texY);
   glVertex3f(x, y, z);
}
