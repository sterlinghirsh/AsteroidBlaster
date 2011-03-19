/**
 * Sterling Hirsh
 * CPE 476
 * 1-16-11
 *
 * MeshPoint: A class to store a point in 3D space for use in a Mesh3D.
 */

#include "Graphics/MeshPoint.h"
#include <math.h>

MeshPoint::MeshPoint(double xIn, double yIn, double zIn) : Point3D((GLfloat)xIn, (GLfloat)yIn, (GLfloat)zIn) {
   normal.updateMagnitude(0, 0, 0);
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
   glVertex3d(x, y, z);
}
