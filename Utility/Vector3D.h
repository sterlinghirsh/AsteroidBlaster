/**
 * Sterling Hirsh
 * Vector3D
 */

#ifndef __VECTOR3D_H__
#define __VECTOR3D_H__

#include <stdio.h>

class Vector3D {
   public:
      double x, y, z;
      //const static double drawScale = 0.01;

      Vector3D(double _x = 0, double _y = 0, double _z = 0);
      Vector3D(Vector3D, Vector3D);

      double dot(Vector3D&);
      Vector3D cross(Vector3D&);
      Vector3D add(Vector3D);
      Vector3D subtract(Vector3D&);
      void subtractUpdate(const Vector3D);
      void movePoint(Vector3D&, double scale = 1);
      void addUpdate(Vector3D);
      void addNormal();
      double getLength();
      const double magnitude() const;
      double getComparisonLength();
      double getAngleInRadians(Vector3D&);
      double getAngleInDegrees(Vector3D&);
      void draw();
      void normalize();
      void abs();
      const Vector3D getNormalized() const;
      void updateMagnitude(double, double, double);
      void updateMagnitude(Vector3D, Vector3D);
      void updateMagnitude(Vector3D*, Vector3D*);
      void updateMagnitude(Vector3D);
      void updateMagnitude(Vector3D*);
      void updateFromVirtualTrackball(double, double);
      void updateFromVirtualTrackball(int, int);
      void rotate(double deg, double _x, double _y, double _z);
      void rotate(double angle, const Vector3D& axis);
      void rotateByDegrees(double angle, const Vector3D& axis);
      void randomMagnitude();
      Vector3D scalarMultiply(double);
      inline void scalarMultiplyUpdate(double scalar) {
         x *= scalar;
         y *= scalar;
         z *= scalar;
      }
      void setLength(double);
      void glTranslate(double length = 1);
      void reflect(Vector3D& axis);
      Vector3D getNormalVector();
      void negativeX();
      void negativeY();
      void negativeZ();
      void positiveX();
      void positiveY();
      void positiveZ();
      void clone(Vector3D* other);
      void update(double x2, double y2, double z2);
      double distanceFrom(Vector3D& rhs);
      void offsetBy(double x2, double y2, double z2);
      void midpoint(const Vector3D& p1, const Vector3D& p2);

      void print() {
         printf("Vector: (%f, %f, %f)\n", x, y, z);
      }

      const Vector3D operator+(const Vector3D& rhs) const;
      const Vector3D operator-(const Vector3D& rhs) const;
      const double operator*(const Vector3D& rhs) const;
      bool operator==(const Vector3D& rhs) const;
      Vector3D &operator=(const Vector3D &src);
      Vector3D& operator/=(double scalar); 
      const Vector3D operator*(double scalar) const;
      const Vector3D operator^(const Vector3D& rhs) const;

};

#endif
