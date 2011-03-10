/**
 * Drawable. Any item that is to be drawn to the screen.
 * Taylor Arnicar
 * 3-6-11
 * CPE 476
 */

#ifndef __DRAWABLE_H__
#define __DRAWABLE_H__

#include "Utility/Point3D.h"

class Drawable {
   public:
      Point3D* position;

      Drawable(double x, double y, double z, GLuint displayListIn);
      virtual ~Drawable();
      virtual void init();
      double unrootedDist(Point3D *other);
};

#endif
