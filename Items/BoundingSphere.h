/**
 * Sphere constrains objects' motion.
 * Chris Brenton
 * 2-25-11
 * CPE 476
 */

#ifndef __BOUNDINGSPHERE_H__
#define __BOUNDINGSPHERE_H__

#include "Utility/Point3D.h"
#include "Items/BoundingSpace.h"

class BoundingSphere : public BoundingSpace {
   public:
      double radius;
      double cx, cy, cz;
      Point3D *center;

      BoundingSphere(double extentIn, double x, double y, double z);
      ~BoundingSphere();
      virtual void constrain(Object3D* item);
      void draw();
      void update(double timeDiff);
   private:
      std::list<GlowSquare*> glowingSquares;
};

#endif
