/** * AsteroidShip.h
 * Sterling Hirsh
 * This is the ship that a player plays from or can see.
 */

#ifndef __QUATERNION_H__
#define __QUATERNION_H__

#include "Point3D.h"
#include "Vector3D.h"
#include "Matrix4.h"

class Quaternion {
   public:
      double x, y, z, w;

      Quaternion();
      Quaternion(double a, double b, double c, double d);
      void normalize();
      Quaternion getConjugate();
      Quaternion operator*(const Quaternion &rq) const;
      Vector3D operator*(Vector3D &vec);
      Point3D operator*(Point3D &vec);

      void FromAxis(const Vector3D &v, double angle);
      void FromAxis(const Point3D &v, double angle);
      void FromEuler(double pitch, double yaw, double roll);
      void getAxisAngle(Vector3D *axis, double *angle);
      void getAxisAngle(Point3D *axis, double *angle);
      void Rotate(Vector3D rotVec, double angle);
      void Rotate(Point3D rotVec, double angle);
      void getMatrix(GLfloat*);
      Matrix4 getMatrix();
      //Quaternion slerp(const Quat &q1, const Quat &q2, float t);
      void print();

};

#endif
