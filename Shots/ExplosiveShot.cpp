/**
 * ExplosiveShot.cpp
 * All bombs, missiles, etc will extend this in some way.
 */

#include "Shots/ExplosiveShot.h"
#include "Particles/BlasterShotParticle.h"
#include "Particles/BlasterImpactParticle.h"
#include "Utility/SoundEffect.h"
#include "Graphics/Sprite.h"

#ifdef WIN32
#include "Utility/WindowsMathLib.h"
#endif

//const int particleCycle = 100;

ExplosiveShot::ExplosiveShot(Point3D& posIn, Vector3D dirIn,
 AsteroidShip* const ownerIn, const GameState* _gameState) : Shot(posIn, dirIn, ownerIn, _gameState) {
   /* When isExploded becomes true, we check for collisions within the
      explodeRadius. */
   explodeRadius = 5;
   lifetime = 30;
   // All ExplosiveShots should start off not yet exploded.
   isExploded = false;
   shouldExplode = false;

   collisionType = collisionSphere = new CollisionSphere(0, *position);
}

/**
 * update should do all generic shot updating, but then should be
 * overridden, just like draw, by all classes that extend ExplosiveShot.
 */
void ExplosiveShot::update(double timeDiff) {
   Shot::update(timeDiff);
}

/**
 * This is called when the ExplosiveShot realizes it isExploded.
 * This should be overridden by all classes that extend ExplosiveShot to
 * do more specific features.
 */
void ExplosiveShot::explode() {
   // Set the radius of the bomb to the explodeRadius so that collision detection can happen.
   minX = minY = minZ = -1 * explodeRadius;
   maxX = maxY = maxZ = explodeRadius;
   // Update bounding box's world coords w/ these new sizes.
   updateBoundingBox();
   collisionSphere->updateRadius(explodeRadius);

   isExploded = true;

   shouldConstrain = false;

   // Play an explosion animation.
   Sprite::sprites.push_back(
         new Sprite(Texture::getTexture("AsteroidExplosion"), 4, 5, 20, *position, explodeRadius * 3, explodeRadius * 3, gameState));
}

void ExplosiveShot::hitWall(BoundingWall* wall) {
   Shot::hitWall(wall);
}

double ExplosiveShot::getDamage(Object3D* other) {
   if (!isExploded) {
      return 0;
   } else {
      double distance = position->distanceFrom(*other->position) - other->radius;
      double actualDamage = damage / (1 + ((distance * distance) / explodeRadius));
      return actualDamage;
   }
}
