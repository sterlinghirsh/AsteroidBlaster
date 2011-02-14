/**
 * @file
 * A simple 4x4 Matrix class.
 *
 * <pre>
 * A basic Matrix class to provide operations for 4x4 matrices. Included are
 * functions to invert, transpose, multiply, add, subtract, scale, and
 * find determinants. I'm also cutting support for the Vector3D class. Point3D
 * is the correct class to use, despite the incorrect name.
 * </pre>
 *
 * @author Mike Smith
 */

#ifndef MATRIX4_H
#include "Utility/Point3D.h"

struct Matrix4 {
   union {
      struct {
         double _11, _12, _13, _14,
                _21, _22, _23, _24,
                _31, _32, _33, _34,
                _41, _42, _43, _44;
      };
      double m[4][4];
   };
   
   Matrix4(double m11, double m12, double m13, double m14,
           double m21, double m22, double m23, double m24,
           double m31, double m32, double m33, double m34,
           double m41, double m42, double m43, double m44);

   Matrix4(double _m[4][4]);
   Matrix4(const Matrix4& copy);

   ~Matrix4();

   static Matrix4 fromPoint3D(const Point3D& point);

   Point3D getRow(int row) const;
   Point3D getCol(int col) const;
   
   Matrix4 &operator=(const Matrix4& rhs);
   Matrix4 &operator+=(const Matrix4& rhs);
   Matrix4 &operator-=(const Matrix4& rhs);
   Matrix4 &operator*=(double scale);
   const Point3D operator*(const Point3D& rhs) const;
   const Matrix4 operator*(const Matrix4& rhs) const;
   const Matrix4 operator-(const Matrix4& rhs) const;
   const Matrix4 operator*(double scale) const;
   const Matrix4 operator+(const Matrix4& rhs) const;

   double det() const;
   Matrix4& toIdent();
   Matrix4 inverse() const;
   Matrix4& toInverse();

   static const Matrix4 Identity;

};

#endif
