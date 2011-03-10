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
#include "Items/BoundingWall.h"
#include <queue>
#include <vector>

class BoundingWall;

// Comparison class for priority_queue
class GetMin {
   public:
      bool operator() (const double& lhs, const double& rhs) const {
         return lhs > rhs;
      }
};

class GlowSquare {
   public:
      double timeLastHit;
      float alpha;
      Color* color;
      Point3D p1, p2, p3, p4;
      Point3D midpoint1, midpoint2, midpoint3, midpoint4;
      Vector3D normal;
      BoundingWall* wall;
      int x, y; // X and Y index in the wall.
      std::priority_queue<double, std::vector<double>, GetMin> flashTimes;

      void draw();
      void update(double timeDiff);
      GlowSquare(Color* _color, 
       float size, float _x, float _y, float _z, BoundingWall* _wall, int _xIndex, int _yIndex);
      void hit(int distanceLimit, double delay);
      void drawLines();
   private:
      void initDisplayList();
      int displayList;
      void drawGlowingPart();
};

#endif
