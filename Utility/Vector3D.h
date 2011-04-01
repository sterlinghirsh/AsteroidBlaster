/**
 * Sterling Hirsh
 * Vector3D
 */

#ifndef __VECTOR3D_H__
#define __VECTOR3D_H__

#include <stdio.h>
#include "Utility/Point3D.h"

class Vector3D {
   public:
      double xMag, yMag, zMag;
      //const static double drawScale = 0.01;

      Vector3D(double x = 0, double y = 0, double z = 0);
      Vector3D(Point3D);
      Vector3D(Point3D, Point3D);
      double dot(Vector3D&);
      Vector3D cross(Vector3D&);
      Vector3D add(Vector3D);
      Vector3D subtract(Vector3D&);
      void subtractUpdate(const Vector3D);
      void movePoint(Point3D&, double scale = 1);
      void addUpdate(Vector3D);
      void addNormal();
      double getLength();
      double getComparisonLength();
      double getAngleInRadians(Vector3D&);
      double getAngleInDegrees(Vector3D&);
      void draw();
      void normalize();
      void updateMagnitude(double, double, double);
      void updateMagnitude(Point3D, Point3D);
      void updateMagnitude(Point3D*, Point3D*);
      void updateMagnitude(Point3D);
      void updateMagnitude(Vector3D);
      void updateMagnitude(Vector3D*);
      void updateFromVirtualTrackball(double, double);
      void updateFromVirtualTrackball(int, int);
      void rotate(double deg, double x, double y, double z);
      void rotate(double angle, const Vector3D& axis);
      void rotateByDegrees(double angle, const Vector3D& axis);
      void randomMagnitude();
      Vector3D scalarMultiply(double);
      void scalarMultiplyUpdate(double);
      void setLength(double);
      void glTranslate(double length);
      void reflect(Vector3D& axis);
      Vector3D getNormalVector();
      void negativeX();
      void negativeY();
      void negativeZ();
      void positiveX();
      void positiveY();
      void positiveZ();
      void print() {
         printf("Vector: (%f, %f, %f)\n", xMag, yMag, zMag);
      }

      Vector3D &operator=(const Vector3D& rhs);
};

#endif
