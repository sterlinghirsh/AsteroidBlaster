/**
 * Drawable. Any item that is to be drawn to the screen.
 * Taylor Arnicar
 * 3-6-11
 * CPE 476
 */

#ifndef __DRAWABLE_H__
#define __DRAWABLE_H__

#include "Utility/Point3D.h"


// Incomplete class declaration so we can have the pointer to it.
class Custodian;

class Drawable {
   public:
      Point3D* position;
      Point3D* minPosition;
      Point3D* maxPosition;
      double cullRadius;
      bool shouldRemove; // True when custodian should remove this.
      unsigned int minXRank, maxXRank;
      // Specifies whether or not this Object should be culled by VFC.
      bool shouldBeCulled;
      double radius;
      bool shouldConstrain;
      bool shouldDrawInMinimap;
      // This might not belong here.
      double minX, minY, minZ, maxX, maxY, maxZ;
      Vector3D* velocity;



      Drawable(double x, double y, double z, GLuint displayListIn);
      virtual ~Drawable();
      virtual void init();
      /*
       * draw() is here to be overwritten by all subclasses.
       */
      virtual void draw();

      /*
       * drawGlow() is here to be overwritten by all subclasses.
       */
      virtual void drawGlow();

      double unrootedDist(Point3D *other);
      double unrootedDist(Drawable *other);

      /**
       * Returns the radius of the object which should be used for View Frustum Culling.
       */
      double getCullRadius();

      /**
       * To be overwritten by all subclasses. This is an empty stub only.
       */
      virtual void update(double timeDifference);
      void setCustodian(Custodian* cust);
      virtual bool detectCollision(Drawable* other, bool checkOther = true);
      virtual void handleCollision(Drawable* other);
      virtual void drawInMinimap();
   protected:
      Custodian* custodian;
};

#endif
