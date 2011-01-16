/**
 * Sterling Hirsh
 * CPE 476
 * 1-16-11
 *
 * MeshPoint: A class to store a point in 3D space for use in a Mesh3D.
 */

#include "Graphics/MeshPoint.h"

MeshPoint::MeshPoint(float xIn, float yIn, float zIn) : Point3D(xIn, yIn, zIn) { 
   normal.updateMagnitude(0, 0, 0);
}

void MeshPoint::draw(bool drawNormal) {
   if (drawNormal)
      normal.addNormal();
   glVertex3f(x, y, z);
}
