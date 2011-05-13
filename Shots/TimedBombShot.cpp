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
   
   spin = 90;
   
   rx = 0;
   ry = 0;
   rz = 0;
   
   timeSinceExploded = -1;
   
   damage = 40;
   slowDownPerSecond = 1.0;
   seekRadius = 0.0;
   collisionRadius = 0.25;
   collisionSphere->updateRadius(seekRadius);
}

// Needed to avoid obscure vtable compiler error.
TimedBombShot::~TimedBombShot() {
}

void TimedBombShot::draw() {
   // Don't draw the bomb again if it already exploded.
   if (isExploded && timeSinceExploded > 0) {
      drawExplosion();
      return;
   } else if (isExploded) {
      return;
   }

   glPushMatrix();
      glDisable(GL_LIGHTING);

      /*double amountDone = (doubleTime() - timeFired) / timeToExplode;
      if (fmod(amountDone * amountDone * 80, 2) < 1) {
         glColor3d(0.4, 0.8, 0.4);
      } else {
         glColor3d(1.0, 0.4, 0.4);
      }*/
      //glDisable(GL_
      glColor3d(1, .6, 0);
      setMaterial(ShotMaterial);
      position->glTranslate();

      // Radius 0.5, 5 slices, 5 stacks.
      gluSphere(quadric, 0.3, 5, 5);
      glColor3d(0, 1, 0);
      gluCylinder(quadric, .3, .3, 1, 5, 5);

      glEnable(GL_LIGHTING);
   glPopMatrix();
}

void TimedBombShot::drawExplosion() {
   double scaleSize;
   glPushMatrix();
      glDisable(GL_LIGHTING);

      glColor3d(1, .6, 0);
      setMaterial(ShotMaterial);
      position->glTranslate();

      // Radius 0.5, 5 slices, 5 stacks.
      rx += (double) (rand() % 10);
      ry += (double) (rand() % 20);
      rz += (double) (rand() % 30);

      /*double sx, sy, sz;
      sx = 5 / (1.5 * 5);
      sy = 5 / (.5 * 5);
      sz = 5 / (.8 * 5);*/

      if (rx > 100) {
         rx = 0;
      }
      if (ry > 100) {
         ry = 0;
      }
      if (rz > 100) {
         rz = 0;
      }
      glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
      glUseProgram(hitShader);
      glPushMatrix();
      glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
      glDisable(GL_LIGHTING);
      //printf("I got into draw hit effect\n");
      scaleSize = 5 * (2 - timeSinceExploded) * timeSinceExploded * timeSinceExploded * timeSinceExploded * timeSinceExploded;
      glScaled(scaleSize, scaleSize, scaleSize);
      
      //glTranslated(0, 0, .6);
      glRotated(spin, rx, ry, rz);
      gluSphere(quadric, .6, 20, 20);
      glPopMatrix();
      glUseProgram(0);

      glEnable(GL_LIGHTING);
   glPopMatrix();
}

void TimedBombShot::update(double timeDiff) {
   if(!isExploded){
      double newSpeed = velocity->getLength();
      newSpeed -= timeDiff * slowDownPerSecond;
      newSpeed = std::max(0.0, newSpeed);
      velocity->setLength(newSpeed);

      ExplosiveShot::update(timeDiff);
   }
   /* If the bomb has already blown up and we're back here again,
    * then we've already gone through a cycle of hit detection, so it's time
    * for the bomb to be removed.
    */
   if (isExploded && timeSinceExploded < 0) {
      shouldRemove = true;
   }

   // If the bomb should explode this frame and has not already, then explode.
   if (shouldExplode && !isExploded) {
      timeSinceExploded = 2;
      isExploded = true;
      //explode();
   }
   // If more time has passed than the bomb's timeToExplode, blow it up.
   if (!isExploded && (doubleTime() - timeFired > timeToExplode)) {
      timeSinceExploded = 2;
      isExploded = true;
      //explode();
   }
   
   timeSinceExploded -= timeDiff;
      // Find out how projectileShot's are removed.
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
