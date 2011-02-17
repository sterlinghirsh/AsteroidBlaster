/**
 * Enemy: Someone you want to run into in a dark alley.
 * Sterling Hirsh / Taylor Arnicar
 * 1-18-11
 * CPE 476
 */

#include "Items/Enemy.h"
#include "Items/Player.h"
#include "Graphics/GlutUtility.h"

Enemy::Enemy(double x, double z, double velocityX,
 double velocityZ, int displayListIn) :
 Object3D(x, 0, z, displayListIn) {
   velocity = new Vector3D(velocityX, 0, velocityZ);
   wasHit = false;
   minX = minZ -0.5;
   maxX = maxZ = 0.5;
   minY = 0;
   maxY = 1;
   angle = randdouble() * 360;
   axis = new Vector3D(0, 1, 0);
}

void Enemy::draw() {
   if (wasHit)
      setMaterial(RedFlat);
   else
      setMaterial(BlueShiny);
   Object3D::draw();
}

void Enemy::handleCollision(Object3D* other) {
   if (dynamic_cast<Player*>(other) != NULL) {
      wasHit = true;
      velocity->updateMagnitude(0, 0, 0);
   } else if (dynamic_cast<Enemy*>(other) != NULL) {
      velocity->updateMagnitude(0, 0, 0);
   }
}

