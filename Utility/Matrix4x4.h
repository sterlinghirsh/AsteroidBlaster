/**
 * Matrix4x4 - Used in view frustum culling.
 * Taylor Arnicar / Sterling Hirsh
 * 2-7-11
 * CPE 476
 */

#ifndef __MATRIX4X4_H
#define __MATRIX4X4_H

struct Matrix4x4
{
   // The elements of the 4x4 Matrix are stored in column-major order.
   float _11, _21, _31, _41;
   float _12, _22, _32, _42;
   float _13, _23, _33, _43;
   float _14, _24, _34, _44;
   void doTranspose() {
      float temp;
      temp = _12; _12 = _21; _21 = temp;
      temp = _13; _13 = _31; _31 = temp;
      temp = _14; _14 = _41; _41 = temp;
      temp = _32; _32 = _23; _23 = temp;
      temp = _34; _34 = _43; _43 = temp;
      temp = _42; _42 = _24; _24 = temp;
   }
};

#endif
