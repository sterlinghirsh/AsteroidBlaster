/**
 * Enemy: Someone you want to run into in a dark alley.
 * Sterling Hirsh / Taylor Arnicar
 * 1-18-11
 * CPE 476
 */

#ifndef __ENEMY_H__
#define __ENEMY_H__

#include "Items/Object3D.h"

class Enemy : public Object3D {
   public:
      bool wasHit;
      Enemy(double x, double z, double velocityX, double velocityZ, int displayListIn);
      virtual void draw();
      virtual void handleCollision(Drawable* other);
};

#endif
