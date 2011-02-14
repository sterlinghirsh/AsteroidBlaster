/**
 * @file
 * Implementation of 4x4 Matrix class.
 *
 * @author Mike Smith
 */

#include "Utility/Matrix4.h"
#include "math.h"

const Matrix4 Matrix4::Identity = Matrix4(1, 0, 0, 0,
                                          0, 1, 0, 0,
                                          0, 0, 1, 0,
                                          0, 0, 0, 1);

/** Yes, this is a very ugly constructor. But it's convenient, so there is no
 * reason for it to be changed.
 */
Matrix4::Matrix4(double m11=0, double m12=0, double m13=0, double m14=0,
                 double m21=0, double m22=0, double m23=0, double m24=0,
                 double m31=0, double m32=0, double m33=0, double m34=0,
                 double m41=0, double m42=0, double m43=0, double m44=0) {
   _11 = m11;
   _12 = m12;
   _13 = m13;
   _14 = m14;
   _21 = m21;
   _22 = m22;
   _23 = m23;
   _24 = m24;
   _31 = m31;
   _32 = m32;
   _33 = m33;
   _34 = m34;
   _41 = m41;
   _42 = m42;
   _43 = m43;
   _44 = m44;
}

Matrix4::Matrix4(double _m[4][4]) {
   for (int i = 0; i < 4; i++)
      for (int j = 0; j < 4; j++)
         m[i][j] = _m[i][j];
}

Matrix4::Matrix4(const Matrix4& copy) {
   for (int i = 0; i < 4; i++)
      for (int j = 0; j < 4; j++)
         m[i][j] = copy.m[i][j];
}

Matrix4::~Matrix4() {}

Matrix4 Matrix4::fromPoint3D(const Point3D& point) {
   Matrix4 temp(point.x, 0, 0, 0,
                point.y, 0, 0, 0,
                point.z, 0, 0, 0,
                0 , 0, 0, 0);
   return temp;
}

Point3D Matrix4::getRow(int row) const {
   Point3D t;
   if (row < 0 || row >= 4)
      return t;

   t.x = m[row][0];
   t.y = m[row][1];
   t.z = m[row][2];

   return t;
}

Point3D Matrix4::getCol(int col) const {
   Point3D t;
   if (col < 0 || col >=4)
    return t;

   t.x = m[0][col];
   t.y = m[1][col];
   t.z = m[2][col];

   return t;
}

Matrix4 &Matrix4::operator=(const Matrix4& rhs) {
   if (&rhs == this)
      return *this;

   for (int i = 0; i < 4; i++)
      for (int j = 0; j < 4; j++)
         m[i][j] = rhs.m[i][j];

   return *this;
}

Matrix4 &Matrix4::operator+=(const Matrix4 &rhs) {
   for (int i = 0; i < 4; i++)
      for (int j = 0; j < 4; j++)
         m[i][j] += rhs.m[i][j];

   return *this;
}

const Matrix4 Matrix4::operator+(const Matrix4 &rhs) const {
   return Matrix4(*this) += rhs;
}

Matrix4 &Matrix4::operator-=(const Matrix4 &rhs) {
   for (int i = 0; i < 4; i++)
      for (int j = 0; j < 4; j++)
         m[i][j] -= rhs.m[i][j];

   return *this;
}

const Matrix4 Matrix4::operator-(const Matrix4 &rhs) const {
   return Matrix4(*this) -= rhs;
}

Matrix4 &Matrix4::operator*=(double scale) {
   for (int i = 0; i < 4; i++)
      for (int j = 0; j < 4; j++)
         m[i][j] *= scale;

   return *this;
}

const Matrix4 Matrix4::operator*(double scale) const {
   return Matrix4(*this) *= scale;
}

const Matrix4 Matrix4::operator*(const Matrix4& rhs) const {
   Matrix4 t;

   for (int i = 0; i < 4; i++)
      for (int j = 0; j < 4; j++) {
         t.m[i][j] = getRow(i) * rhs.getCol(j) + m[i][3] * rhs.m[3][j];
      }

   return t;
}

double Matrix4::det() const {
   double det = _11 * _22 * _33 * _44 + _11 * _23 * _34 * _42 +
                _11 * _24 * _32 * _43 + _12 * _21 * _34 * _43 +
                _12 * _23 * _31 * _44 + _12 * _24 * _33 * _41 +
                _13 * _21 * _32 * _44 + _13 * _22 * _34 * _41 +
                _13 * _24 * _31 * _42 + _14 * _21 * _33 * _42 +
                _14 * _22 * _31 * _43 + _14 * _23 * _32 * _41 -
                _11 * _22 * _34 * _43 - _11 * _23 * _32 * _44 -
                _11 * _24 * _33 * _42 - _12 * _21 * _33 * _44 -
                _12 * _23 * _34 * _41 - _12 * _24 * _31 * _43 -
                _13 * _21 * _34 * _42 - _13 * _22 * _31 * _44 -
                _13 * _24 * _32 * _41 - _14 * _21 * _32 * _43 -
                _14 * _22 * _33 * _41 - _14 * _23 * _31 * _42;
   return det;
}

Matrix4& Matrix4::toIdent() {
   *this = Identity;
   return *this;
}

Matrix4 Matrix4::inverse() const {
   Matrix4 temp;
   double d = det();
   if (d == 0)
      return temp;

   temp._11 = _22 * _33 * _44 + _23 * _34 * _42 + _24 * _32 * _43 -
              _22 * _34 * _43 - _23 * _32 * _44 - _24 * _33 * _42;

   temp._12 = _12 * _34 * _43 + _13 * _32 * _44 + _14 * _33 * _42 -
              _12 * _33 * _44 - _13 * _34 * _42 - _14 * _32 * _43;

   temp._13 = _12 * _23 * _44 + _13 * _24 * _42 + _14 * _22 * _43 -
              _12 * _24 * _43 - _13 * _22 * _44 - _14 * _23 * _42;

   temp._14 = _12 * _24 * _33 + _13 * _22 * _34 + _14 * _23 * _32 -
              _12 * _23 * _34 - _13 * _24 * _32 - _14 * _22 * _33;

   temp._21 = _21 * _34 * _43 + _23 * _31 * _44 + _24 * _33 * _41 -
              _21 * _33 * _44 - _23 * _34 * _41 - _24 * _31 * _43;

   temp._22 = _11 * _33 * _44 + _13 * _34 * _41 + _14 * _31 * _43 -
              _11 * _34 * _43 - _13 * _31 * _44 - _14 * _33 * _41;

   temp._23 = _11 * _24 * _43 + _13 * _21 * _44 + _14 * _23 * _41 -
              _11 * _23 * _44 - _13 * _24 * _41 - _14 * _21 * _43;

   temp._24 = _11 * _23 * _34 + _13 * _24 * _31 + _14 * _21 * _33 -
              _11 * _24 * _33 - _13 * _21 * _34 - _14 * _23 * _31;

   temp._31 = _21 * _32 * _44 + _22 * _34 * _41 + _24 * _31 * _42 -
              _21 * _34 * _42 - _22 * _31 * _44 - _24 * _32 * _41;

   temp._32 = _11 * _34 * _42 + _12 * _31 * _44 + _14 * _32 * _41 -
              _11 * _32 * _44 - _12 * _34 * _41 - _14 * _31 * _42;

   temp._33 = _11 * _22 * _44 + _12 * _24 * _41 + _14 * _21 * _42 -
              _11 * _24 * _42 - _12 * _21 * _44 - _14 * _22 * _41;

   temp._34 = _11 * _24 * _32 + _12 * _21 * _34 + _14 * _22 * _31 -
              _11 * _22 * _34 - _12 * _24 * _31 - _14 * _21 * _32;

   temp._41 = _21 * _33 * _42 + _22 * _31 * _43 + _23 * _32 * _41 -
              _21 * _32 * _43 - _22 * _33 * _41 - _23 * _31 * _42;

   temp._42 = _11 * _32 * _43 + _12 * _33 * _41 + _13 * _31 * _42 -
              _11 * _33 * _42 - _12 * _31 * _43 - _13 * _32 * _41;

   temp._43 = _11 * _23 * _42 + _12 * _21 * _43 + _13 * _22 * _41 -
              _11 * _22 * _43 - _12 * _23 * _41 - _13 * _21 * _42;

   temp._44 = _11 * _22 * _33 + _12 * _23 * _31 + _13 * _21 * _32 -
              _11 * _23 * _32 - _12 * _21 * _33 - _13 * _22 * _31;

   temp *= 1 / d;

   return temp;
}

Matrix4& Matrix4::toInverse() {
   *this = inverse();

   return *this;
}
