/**
 * Sterling Hirsh
 * CPE 471 
 * 10-19-10
 *
 * Point3D: A class to store a point in 3D space.
 */

#ifndef __POINT3D_H__
#define __POINT3D_H__

#include <stdio.h>
#include "Utility/GlobalUtility.h"
#include "Utility/Vector3D.h"

struct Matrix4;

class Point3D : public Vector3D {
   public:
      Point3D(double xIn = 0, double yIn = 0, double zIn = 0) :
       Vector3D(xIn, yIn, zIn) {}
      
      Point3D(const Vector3D& copy);

      virtual void draw();
      void print();

      static const Point3D Zero;
};

#endif
