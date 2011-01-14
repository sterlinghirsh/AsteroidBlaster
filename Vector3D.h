/**
 * Sterling Hirsh
 * Vector3D
 */

#ifndef __VECTOR3D_H__
#define __VECTOR3D_H__

#include <stdio.h>
#include "Point3D.h"

#define drawScale 1

class Vector3D {
   public:
      double xMag, yMag, zMag, xPos, yPos, zPos;
      //const static double drawScale = 0.01;

      Vector3D(double x = 0, double y = 0, double z = 0, double a = 0, 
       double b = 0, double c = 0);
      Vector3D(Point3D);
      Vector3D(Point3D, Point3D);
      double dot(Vector3D&);
      Vector3D cross(Vector3D&);
      Vector3D add(Vector3D);
      Vector3D subtract(Vector3D&);
      void subtractUpdate(const Vector3D);
      void movePoint(Point3D&);
      void addUpdate(Vector3D);
      void addToPosition(Vector3D&);
      void addNormal();
      double getLength();
      double getAngleInRadians(Vector3D&);
      double getAngleInDegrees(Vector3D&);
      void draw();
      void normalize();
      void updateMagnitude(double, double, double);
      void updateMagnitude(Point3D, Point3D);
      void updateMagnitude(Point3D);
      void updatePosition(double, double, double);
      void updatePosition(Point3D);
      void update(double, double, double, double, double, double);
      void updateFromVirtualTrackball(double, double);
      void updateFromVirtualTrackball(int, int);
      void rotate(double deg, double x, double y, double z);
      void setCamera(double, double, double);
      void rotate(double angle, Vector3D& axis);
      void randomMagnitude();
      Vector3D scalarMultiply(double);
      void setLength(double);
      void glTranslate(double length);
      Vector3D getNormalVector();
      void print() {
         printf("Vector: (%f, %f, %f)\n", xMag, yMag, zMag);
      }
};

#endif
