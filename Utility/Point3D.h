/**
 * Sterling Hirsh
 * CPE 471 
 * 10-19-10
 *
 * Point3D: A class to store a point in 3D space.
 */

#ifndef __POINT3D_H__
#define __POINT3D_H__

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <stdio.h>
#include "Graphics/GlutUtility.h"

struct Point3D {
   double x, y, z;
   Point3D(double xIn = 0, double yIn = 0, double zIn = 0) :
    x(xIn), y(yIn), z(zIn) {}
   
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
};

#endif
