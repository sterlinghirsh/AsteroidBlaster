/**
 * TimedBombShot.cpp
 * This bomb lasts for a few seconds before exploding.
 */

#include "Shots/TimedBombShot.h"
#include "Shots/ExplosiveShot.h"
#include "Utility/SoundEffect.h"
#include "Graphics/Sprite.h"

#ifdef WIN32
#include "Utility/WindowsMathLib.h"
#endif


TimedBombShot::TimedBombShot(Point3D& posIn, Vector3D dirIn, AsteroidShip* const ownerIn, const GameState* _gameState) : ExplosiveShot(posIn, dirIn, ownerIn, _gameState) {
   radius = seekRadius;
   minX = minY = minZ = -radius;
   maxX = maxY = maxZ = radius;
   updateBoundingBox();

   // Blow up 15 seconds after it's fired.
   timeToExplode = 15;
   
   explodeRadius = 8;
   
   damage = 40;
   slowDownPerSecond = 5.0;
   seekRadius = 20.0;
   collisionRadius = 1.0;
}

// Needed to avoid obscure vtable compiler error.
TimedBombShot::~TimedBombShot() {
}

void TimedBombShot::draw() {
   // Don't draw the bomb again if it already exploded.
   if (isExploded) {
      return;
   }

   glPushMatrix();
      glDisable(GL_LIGHTING);

      double amountDone = (doubleTime() - timeFired) / timeToExplode;
      if (fmod(amountDone * amountDone * 80, 2) < 1) {
         glColor3d(0.4, 0.8, 0.4);
      } else {
         glColor3d(1.0, 0.4, 0.4);
      }
      setMaterial(ShotMaterial);
      position->glTranslate();

      // Radius 0.5, 5 slices, 5 stacks.
      gluSphere(quadric, 0.5, 5, 5);

      glEnable(GL_LIGHTING);
   glPopMatrix();
}

void TimedBombShot::update(double timeDiff) {
   double newSpeed = velocity->getLength();
   newSpeed -= timeDiff * slowDownPerSecond;
   newSpeed = std::max(0.0, newSpeed);
   velocity->setLength(newSpeed);

   ExplosiveShot::update(timeDiff);

   /* If the bomb has already blown up and we're back here again,
    * then we've already gone through a cycle of hit detection, so it's time
    * for the bomb to be removed.
    */
   if (isExploded) {
      shouldRemove = true;
   }

   // If the bomb should explode this frame and has not already, then explode.
   if (shouldExplode && !isExploded) {
      explode();
   }
   // If more time has passed than the bomb's timeToExplode, blow it up.
   if (!isExploded && (doubleTime() - timeFired > timeToExplode)) {
      explode();
   }

      // Find out how projectileShot's are removed.
}

/**
 * Make it only hit asteroids.
 * Later we'll want to hit ships, too.
 * Also shards, for pushin'.
 */
bool TimedBombShot::detectCollision(Drawable* other, bool checkOther) {
   Asteroid3D* asteroid;
   if ((asteroid = dynamic_cast<Asteroid3D*>(other)) != NULL) {
      return ExplosiveShot::detectCollision(other, checkOther);
   } else {
      return false;
   }
}

void TimedBombShot::hitWall(BoundingWall* wall) {
   // Check to see if we're actually within exploderadius.
   minX = minY = minZ = -collisionRadius;
   maxX = maxY = maxZ = collisionRadius;
   updateBoundingBox();

   ExplosiveShot::hitWall(wall);

   minX = minY = minZ = -seekRadius;
   maxX = maxY = maxZ = seekRadius;
   updateBoundingBox();
}

/**
 * Called when the bomb should be exploding.
 * Make the size of the bomb much bigger so that it will collide with
 * everything it should, and auto-handle the collisions.
 */
void TimedBombShot::explode() {
   // Do all the generic exploding actions that every bomb should do.
   ExplosiveShot::explode();

   // Play a sound
   SoundEffect::playSoundEffect("Explosion1.wav");

   // set it as not collidable
}

// Override detectCollision function
// if isExploded and 
