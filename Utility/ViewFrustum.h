/**
 * viewFrustum: It's got useful functions to cull items that aren't in your view frustum
 * Taylor Arnicar / Sterling Hirsh
 * 2-7-11
 * CPE 476
 */
 
#ifndef __VIEW_FRUSTUM_H__
#define __VIEW_FRUSTUM_H__

#include "Utility/Object3D.h"
#include "Utility/Plane.h"
#include "Utility/Matrix4x4.h"
#include <vector>
#include <set>

class ViewFrustum {
   public:
      
      ViewFrustum();
      virtual ~ViewFrustum();
      
      /* Takes in a list of all the Object 3D's around, and culls them down to only the ones
       * that are inside the view frustum.
       */
      virtual std::list<Object3D*>* cullToViewFrustum(std::vector<Object3D*>* all);

   private:
      Matrix4x4 projMatrix;
      Matrix4x4 modelViewMatrix;
      Plane* top;
      Plane* bottom;
      Plane* left;
      Plane* right;
      Plane* near;
      Plane* far;

      /* Returns true if the object3D is completely outside of the view frustum planes.
       * Returns false if it's even part-way inside.
       */
      virtual bool checkOutside(Object3D* obj);
};

#endif
