/**
 * viewFrustum: It's got useful functions to cull items that aren't in your view frustum
 * Taylor Arnicar / Sterling Hirsh
 * 2-7-11
 * CPE 476
 */
 
#ifndef __VIEW_FRUSTUM_H__
#define __VIEW_FRUSTUM_H__

#include "Items/Object3D.h"
#include "Utility/Plane.h"
#include "Utility/Matrix4.h"
#include <vector>
#include <set>

class ViewFrustum {
   public:
      
      ViewFrustum();
      virtual ~ViewFrustum();
      
      /* Takes in a list of all the Object 3D's around, and culls them down to only the ones
       * that are inside the view frustum.
       */
      virtual std::list<Drawable*>* cullToViewFrustum(std::vector<Drawable*>* all, bool skipParticles);
      virtual std::list<Object3D*>* cullToViewFrustum(std::vector<Object3D*>* all);
      
      /* Prints out details about all of the planes of the view frustum.
       */
      virtual void print();

   private:
      Matrix4 projMatrix;
      Matrix4 modelViewMatrix;
      Plane* top;
      Plane* bottom;
      Plane* left;
      Plane* right;
      Plane* near;
      Plane* far;

      /** 
       * A modified version of chedDrawableOutside, used by the AI. This stops the
       * AI from targeting Drawable objects which are slightly outside the field
       * of view, which still need to be drawn.
       */
      bool checkTargetableOutside(Drawable* obj);

      /* Returns true if the Drawable object is completely outside of the 
       * view frustum planes.
       * Returns false if it's even part-way inside.
       */
      virtual bool checkDrawableOutside(Drawable* obj);
};

#endif
