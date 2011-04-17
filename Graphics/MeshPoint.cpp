/**
 * Sterling Hirsh
 * CPE 476
 * 1-16-11
 *
 * MeshPoint: A class to store a point in 3D space for use in a Mesh3D.
 */

#include "Graphics/MeshPoint.h"
#include <math.h>

MeshPoint::MeshPoint(double xIn, double yIn, double zIn) : Point3D((GLdouble)xIn, (GLdouble)yIn, (GLdouble)zIn) {
   normal.updateMagnitude(0, 0, 0);
}

void MeshPoint::draw(bool drawNormal) {
   if (drawNormal)
      normal.addNormal();
   glVertex3d(x, y, z);
}
