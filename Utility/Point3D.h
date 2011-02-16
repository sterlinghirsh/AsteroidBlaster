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
#include "Graphics/GlutUtility.h"

struct Matrix4;

struct Point3D {
   double x, y, z;
   Point3D(double xIn = 0, double yIn = 0, double zIn = 0) :
    x(xIn), y(yIn), z(zIn) {}

   void clone(Point3D* other) {
      x = other->x;
      y = other->y;
      z = other->z;
   }

   void update(double x2, double y2, double z2) {
      x = x2;
      y = y2;
      z = z2;
   }
   
   double distanceFrom(Point3D& rhs) {
      return distance3D(rhs.x - x, rhs.y - y, rhs.z - z);
   }

   virtual void draw() {
      glVertex3f(x, y, z);
   }
   void print() {
      printf("(%f, %f, %f)\n", x, y, z);
   }
   void glTranslate() {
      glTranslatef(x, y, z);
   }

   void offsetBy(double x2, double y2, double z2) {
      x += x2;
      y += y2;
      z += z2;
   }

   Point3D add(Point3D other) {
      Point3D answer;
      answer.x = x + other.x;
      answer.y = y + other.y;
      answer.z = z + other.z;
      return answer;
   }

   const Point3D operator+(const Point3D& rhs) const;
   const Point3D operator-(const Point3D& rhs) const;
   const double operator*(const Point3D& rhs) const;
   const int operator==(const Point3D& rhs) const;
   Point3D &operator=(const Point3D &src);
   Point3D& operator/=(double scalar); 
   const Point3D operator*(double scalar) const;
   const Point3D operator^(const Point3D& rhs) const;

   const Point3D normalize() const;
   const double magnitude() const;
   static const Point3D Zero;
};

#endif
