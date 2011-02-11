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

Point3D Point3D::operator+(const Point3D &rhs) {
   return Point3D(x + rhs.x, y + rhs.y, z + rhs.z);
}

Point3D Point3D::operator-(const Point3D &rhs) {
   return Point3D(x - rhs.x, y - rhs.y, z - rhs.z);
}

double Point3D::operator*(const Point3D &rhs) {
   return x * rhs.x + y * rhs.y + z * rhs.z;
}

Point3D& Point3D::operator/=(double scalar) {
   x /= scalar;
   y /= scalar;
   z /= scalar;

   return *this;
}
   
int Point3D::operator==(const Point3D &rhs) const {
   if (fabs(x - rhs.x) < EPSILON)
      if (fabs(y - rhs.y) < EPSILON)
         if (fabs(z - rhs.z) < EPSILON)
            return 1;
   return 0;
}

Point3D& Point3D::operator=(const Point3D &src) {
   if (&src == this)
      return *this;

   x = src.x;
   y = src.y;
   z = src.z;

   return *this;
}

Point3D Point3D::operator*(double scalar)
{
   return Point3D(x * scalar, y * scalar, z * scalar);
}

Point3D Point3D::normalize() const
{
   double len = magnitude();
   return Point3D(x / len, y / len, z / len);
}

double Point3D::magnitude() const
{
   return sqrt(x * x + y * y + z * z);
}