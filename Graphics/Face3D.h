/**
 * Sterling Hirsh
 * CPE 471
 * 10-19-10
 *
 * Point3D: A class to store a face in 3D space.
 * I decided not to make a .cpp for this since it's so simple.
 * The points (p1-3) that this stores key into an array in the enclosing Mesh3D object.
 */

#ifndef __FACE3D_H__
#define __FACE3D_H__

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include "Utility/Vector3D.h"

struct Face3D {
   int p1, p2, p3;
   Vector3D normal;
   Face3D(int a, int b, int c) :
    p1(a), p2(b), p3(c) {}
   
   Face3D() {
      p1 = p2 = p3 = 0;
   }
};

#endif
