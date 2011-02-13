/**
 * Plane : This class represents a plane's equations, used for view frustum culling.
 * Taylor Arnicar / Sterling Hirsh
 * 2-7-11
 * CPE 476
 */

#ifndef __PLANE_H__
#define __PLANE_H__

class Plane {
   public:
      float a;
      float b;
      float c;
      float d;
      // Putting the constructor here since it's short.
      Plane() {
         a = b = c = d = 0;
      }
       
      float distanceTo(float x, float y, float z) {
         float dist =  (a * x) + (b * y) + (c * z) + d;
         return dist;
      }

      bool onPositiveSide(Object3D* obj) {
         return
            distanceTo(obj->maxPosition->x, obj->maxPosition->y, obj->maxPosition->z) > 0 ||
            distanceTo(obj->maxPosition->x, obj->maxPosition->y, obj->minPosition->z) > 0 ||
            distanceTo(obj->maxPosition->x, obj->minPosition->y, obj->maxPosition->z) > 0 ||
            distanceTo(obj->maxPosition->x, obj->minPosition->y, obj->minPosition->z) > 0 ||
            distanceTo(obj->minPosition->x, obj->maxPosition->y, obj->maxPosition->z) > 0 ||
            distanceTo(obj->minPosition->x, obj->maxPosition->y, obj->minPosition->z) > 0 ||
            distanceTo(obj->minPosition->x, obj->minPosition->y, obj->maxPosition->z) > 0 ||
            distanceTo(obj->minPosition->x, obj->minPosition->y, obj->minPosition->z) > 0;
      }

   private:

};

#endif
