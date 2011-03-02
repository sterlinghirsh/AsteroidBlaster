/**
 * GlowSquare is a glowing square on the side.
 * @author Sterling Hirsh
 * @date 2/24/2011
 */

#ifndef __GLOWSQUARE_H__
#define __GLOWSQUARE_H__

#include "Utility/Point3D.h"
#include "Utility/Vector3D.h"
#include "Utility/GlobalUtility.h"

class GlowSquare {
   public:
      //double lifetime;
      double timeLastHit;
      float alpha;
      Color* color;
      Point3D p1, p2, p3, p4;
      Vector3D normal;

      void draw();
      void update(double timeDiff);
      GlowSquare(Color* _color, 
       float size, float x, float y, float z, int wallID);
      static float lifetime;
      void hit();
};

#endif
