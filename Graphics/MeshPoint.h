/**
 * Sterling Hirsh
 * CPE 471 
 * 11-15-10
 *
 * MeshPoint: A class to store a point in 3D space for use in a Mesh3D.
 */

#ifndef __MESHPOINT_H__
#define __MESHPOINT_H__

#include "Utility/Point3D.h"
#include "Utility/Vector3D.h"

struct MeshPoint : public Point3D {
   Vector3D normal;
   MeshPoint(double xIn = 0, double yIn = 0, double zIn = 0);
   void draw(bool drawNormal = false);
};

#endif
