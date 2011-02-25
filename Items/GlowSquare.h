/**
 * GlowSquare is a glowing square on the side.
 * @author Sterling Hirsh
 * @date 2/24/2011
 */

#ifndef __GLOWSQUARE_H__
#define __GLOWSQUARE_H__

#include "Utility/Point3D.h"

class GlowSquare {
   public:
      //double lifetime;
      double timeCreated;
      float r, g, b;
      Point3D p1, p2;

      void draw();
      void update(double timeDiff);
      GlowSquare(double _timeCreated, float _r, float _g, float _b, 
       float size, float x, float y, float z);
};

#endif
