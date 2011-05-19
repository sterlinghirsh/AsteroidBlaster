/**
 * Plane : This class represents a plane's equations, used for view frustum culling.
 * Taylor Arnicar / Sterling Hirsh
 * 2-7-11
 * CPE 476
 */

#ifndef __PLANE_H__
#define __PLANE_H__

#include <math.h>

class Plane {
   public:
      float a;
      float b;
      float c;
      float d;
      float mag;
      // Putting the constructor here since it's short.
      Plane() {
         a = b = c = d = 0;
      }
      
      void calcMag() {
         mag = (float) sqrt((a*a) + (b*b) + (c*c));
      }
      
      inline double distanceTo(double x, double y, double z) {
         return (a * x) + (b * y) + (c * z) + d;
      }

      bool onTargetablePositiveSide(Drawable* obj) {
         // Multiply by 1.75 instead of 2.25 to make sure the AI only targets objects which are mostly on screen, rather than being allowed to target things which are barely off screen.
         return distanceTo(obj->position->x, obj->position->y, obj->position->z) > 
          -1.65 * obj->getCullRadius();
      }

      bool onDrawablePositiveSide(Drawable* obj) {
         // Multiply by 2.25 instead of 2 to make sure things slightly off screen are still drawn, to be sure we prevent popping.
         return distanceTo(obj->position->x, obj->position->y, obj->position->z) > 
          -2.25 * obj->getCullRadius(); 
         // Test with radius instead of Axis Aligned Bounding Boxes
         /*
            distanceTo(obj->maxPosition->x, obj->maxPosition->y, obj->maxPosition->z) > 0 ||
            distanceTo(obj->maxPosition->x, obj->maxPosition->y, obj->minPosition->z) > 0 ||
            distanceTo(obj->maxPosition->x, obj->minPosition->y, obj->maxPosition->z) > 0 ||
            distanceTo(obj->maxPosition->x, obj->minPosition->y, obj->minPosition->z) > 0 ||
            distanceTo(obj->minPosition->x, obj->maxPosition->y, obj->maxPosition->z) > 0 ||
            distanceTo(obj->minPosition->x, obj->maxPosition->y, obj->minPosition->z) > 0 ||
            distanceTo(obj->minPosition->x, obj->minPosition->y, obj->maxPosition->z) > 0 ||
            distanceTo(obj->minPosition->x, obj->minPosition->y, obj->minPosition->z) > 0;
         */
      }
      
      void print() {
         printf("A, B, C, D: %f, %f, %f, %f\n", a, b, c, d);
      }
      
      void printNormalized() {
         double tmpA, tmpB, tmpC,tmpD;
         double mag = sqrt((a*a) + (b*b) + (c*c));
         tmpA = a/mag;
         tmpB = b/mag;
         tmpC = c/mag;
         tmpD = d/mag;
         
         printf("Normalized A, B, C, D: %f, %f, %f, %f\n", tmpA, tmpB, tmpC, tmpD);
         
      }

   private:

};

#endif
