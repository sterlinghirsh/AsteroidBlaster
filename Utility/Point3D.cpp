/**
 * @file
 * Point3D implementation.
 *
 * @author Sterling Hirsh, Mike Smith
 */

#include "Utility/Point3D.h"
#include <math.h>

/** Doubles have a lot of precision.
 * We want to cut that down a bit.
 */
#define EPSILON 0.001

const Point3D Point3D::Zero(0, 0, 0);

inline Point3D Point3D::operator+(const Point3D &rhs)
{
   return Point3D(x + rhs.x, y + rhs.y, z + rhs.z);
}

inline Point3D Point3D::operator-(const Point3D &rhs)
{
   return Point3D(x - rhs.x, y - rhs.y, z - rhs.z);
}

inline double Point3D::operator*(const Point3D &rhs)
{
   return x * rhs.x + y * rhs.y + z * rhs.z;
}

int Point3D::operator==(const Point3D &rhs) const
{
   if (fabs(x - rhs.x) < EPSILON)
      if (fabs(y - rhs.y) < EPSILON)
         if (fabs(z - rhs.z) < EPSILON)
            return 1;
   return 0;
}

const Point3D &Point3D::operator=(const Point3D &src) {
   x = src.x;
   y = src.y;
   z = src.z;

   return *this;
}
