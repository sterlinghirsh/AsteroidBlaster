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

struct Matrix4;
class Vector3D;

struct Point3D {
   double x, y, z;
   Point3D(double xIn = 0, double yIn = 0, double zIn = 0) :
    x(xIn), y(yIn), z(zIn) {}
   
   Point3D(const Point3D& copy);
   Point3D(const Vector3D& copy);

   void clone(Point3D* other);
   void update(double x2, double y2, double z2);
   double distanceFrom(Point3D& rhs);

   virtual void draw();
   void print();
   void glTranslate();
   void offsetBy(double x2, double y2, double z2);
   Point3D add(Point3D other);
   void midpoint(const Point3D& p1, const Point3D& p2);

   const Point3D operator+(const Point3D& rhs) const;
   const Point3D operator-(const Point3D& rhs) const;
   const double operator*(const Point3D& rhs) const;
   bool operator==(const Point3D& rhs) const;
   Point3D &operator=(const Point3D &src);
   Point3D& operator/=(double scalar); 
   const Point3D operator*(double scalar) const;
   const Point3D operator^(const Point3D& rhs) const;

   const Point3D normalize() const;
   const double magnitude() const;
   static const Point3D Zero;
};

#endif
