/**
 * RemoteBombShot.cpp
 * This bomb lasts for a few seconds before exploding.
 */

#include "Shots/RemoteBombShot.h"
#include "Shots/ExplosiveShot.h"
#include "Utility/SoundEffect.h"
#include "Graphics/Sprite.h"

#ifdef WIN32
#include "Utility/WindowsMathLib.h"
#endif

double pulseSize = 0;

RemoteBombShot::RemoteBombShot(Point3D& posIn, Vector3D dirIn,
 AsteroidShip* const ownerIn, RemoteBomber* const _weapon, const GameState* _gameState) : 
 ExplosiveShot(posIn, dirIn, ownerIn, _gameState),
 weapon(_weapon) {
   minX = minY = minZ = -1;
   maxX = maxY = maxZ = 1;
   // Blow up 8 seconds after it's fired.
   timeToExplode = 8;

   updateBoundingBox();
}

// Needed to avoid obscure vtable compiler error.
RemoteBombShot::~RemoteBombShot() {
}

void RemoteBombShot::draw() {
   // Don't draw the bomb again if it already exploded.
   if (isExploded) {
      return;
   }

   glPushMatrix();
      // This is not very efficient.
      glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
      glDisable(GL_LIGHTING);
      position->glTranslate();
      if (fmod(doubleTime() * 6, 2) < 1) {
         glColor3d(0.4, 0.8, 0.4);
      } else {
         glColor3d(1.0, 0.4, 0.4);
      }
      gluSphere(quadric, pulseSize, 10, 10);
      glColor4d(0.4, 0.8, 1, 1);
      gluSphere(quadric, .3, 10, 10);
      //glUseProgram(bombShader);
      glColor4d(0.4, 0.8, 0.4, .1);
      gluSphere(quadric, .4, 10, 10);
      glColor4d(0.4, 0.8, 0.4, .1);
      gluSphere(quadric, .5, 10, 10);
      glColor4d(0.4, 0.8, 0.4, .1);
      gluSphere(quadric, .6, 10, 10);
      glColor4d(0.4, 0.8, 0.4, .1);
      gluSphere(quadric, .7, 10, 10);
      glColor4d(0.4, 0.8, 0.4, .1);
      gluSphere(quadric, .8, 10, 10);
      glColor4d(0.4, 0.8, 0.4, .1);
      gluSphere(quadric, .9, 10, 10);
      glColor4d(0.4, 0.8, 0.4, .1);
      gluSphere(quadric, 1, 10, 10);
      //glUseProgram(0);
      
      // Radius 0.5, 5 slices, 5 stacks.
      
      glEnable(GL_LIGHTING);
   glPopMatrix();
}

void RemoteBombShot::update(double timeDiff) {
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
   
   pulseSize += timeDiff * 2;
   if (pulseSize > 1) pulseSize = 0;

      // Find out how projectileShot's are removed.
}

void RemoteBombShot::handleCollision(Drawable* other) {
   ExplosiveShot::handleCollision(other); 
}

void RemoteBombShot::hitWall(BoundingWall* wall) {
   //particleDirection.reflect(wall->normal);
}

/**
 * Called when the bomb should be exploding.
 * Make the size of the bomb much bigger so that it will collide with
 * everything it should, and auto-handle the collisions.
 */
void RemoteBombShot::explode() {
   printf("%ld Bomb exploded!\n", curFrame);
   // Do all the generic exploding actions that every bomb should do.
   ExplosiveShot::explode();

   // Play a sound
   SoundEffect::playSoundEffect("Explosion1.wav");

   // set it as not collidable (elsewhere)
   // Remove reference in weapon.
   if (weapon->firedShot == this) {
      weapon->firedShot = NULL;
   }
}

// Override detectCollision function
// if isExploded and 
