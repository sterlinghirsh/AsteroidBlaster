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
#include "Utility/GlobalUtility.h"
#include "Utility/Vector3D.h"

struct Matrix4;

#include <boost/serialization/base_object.hpp>

class Point3D : public Vector3D {
   public:
      Point3D(double xIn = 0, double yIn = 0, double zIn = 0) :
       Vector3D(xIn, yIn, zIn) {}
      
      Point3D(const Vector3D& copy);

      virtual void draw() {
         glVertex3d(x, y, z);
      }


      void print();

      static const Point3D Zero;

      
   // Serialization
   public:
      template<class Archive> 
            void serialize(Archive & ar, const unsigned int version) {
         ar & boost::serialization::base_object<Vector3D>(*this);
      }
};

#endif
