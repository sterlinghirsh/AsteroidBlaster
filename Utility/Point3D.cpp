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

const Point3D Point3D::operator+(const Point3D &rhs) const {
   return Point3D(x + rhs.x, y + rhs.y, z + rhs.z);
}

const Point3D Point3D::operator-(const Point3D &rhs) const {
   return Point3D(x - rhs.x, y - rhs.y, z - rhs.z);
}

const double Point3D::operator*(const Point3D &rhs) const {
   return x * rhs.x + y * rhs.y + z * rhs.z;
}

Point3D& Point3D::operator/=(double scalar) {
   x /= scalar;
   y /= scalar;
   z /= scalar;

   return *this;
}

const int Point3D::operator==(const Point3D &rhs) const {
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

const Point3D Point3D::operator*(double scalar) const {
   return Point3D(x * scalar, y * scalar, z * scalar);
}

const Point3D Point3D::normalize() const {
   double len = magnitude();

   if (len < EPSILON)
      return Point3D::Zero;

   if (*this == Point3D::Zero)
      return Point3D::Zero;

   return Point3D(x / len, y / len, z / len);
}

const double Point3D::magnitude() const
{
   return sqrt(x * x + y * y + z * z);
}

const Point3D Point3D::operator^(const Point3D &rhs) const {
  return Point3D(
   (y * rhs.z) - (z * rhs.y),
   (z * rhs.x) - (x * rhs.z),
   (x * rhs.y) - (y * rhs.x));
}

