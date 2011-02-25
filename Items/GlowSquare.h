/**
 * GlowSquare is a glowing square on the side.
 * @author Sterling Hirsh
 * @date 2/24/2011
 */

#ifndef __GLOWSQUARE_H__
#define __GLOWSQUARE_H__

#include "Utility/Point3D.h"
#include "Utility/Vector3D.h"

class GlowSquare {
   public:
      //double lifetime;
      double timeCreated;
      float r, g, b, alpha;
      Point3D p1, p2, p3, p4;
      Vector3D normal;

      void draw();
      void update(double timeDiff);
      GlowSquare(float _r, float _g, float _b, 
       float size, float x, float y, float z);
      static float lifetime;
};

#endif
