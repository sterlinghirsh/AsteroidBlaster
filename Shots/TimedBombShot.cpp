/**
 * TimedBombShot.cpp
 * This bomb lasts for a few seconds before exploding.
 */

#include "Shots/TimedBombShot.h"
#include "Shots/ExplosiveShot.h"
#include "Utility/SoundEffect.h"

#ifdef WIN32
#include "Utility/WindowsMathLib.h"
#endif

TimedBombShot::TimedBombShot(Point3D& posIn, Vector3D dirIn, AsteroidShip* const ownerIn, const GameState* _gameState) : ExplosiveShot(posIn, dirIn, ownerIn, _gameState) {
   persist = false;
   minX = minY = minZ = -1;
   maxX = maxY = maxZ = 1;
   // The bomb should not be blowing up yet.
   isExploded = false;
   // Blow up 3 seconds after it's fired.
   timeToExplode = 3;

   updateBoundingBox();
}

// Needed to avoid obscure vtable compiler error.
TimedBombShot::~TimedBombShot() {
}

void TimedBombShot::draw() {
   glPushMatrix();
      glDisable(GL_LIGHTING);

      glColor3f(0, 1, 0);
      setMaterial(ShotMaterial);
      position->glTranslate();

      // Radius 0.5, 5 slices, 5 stacks.
      gluSphere(quadric, 0.5, 5, 5);

      glEnable(GL_LIGHTING);
   glPopMatrix();
}

void TimedBombShot::update(double timeDiff) {
   ExplosiveShot::update(timeDiff);
   // If more time has passed than the bomb's timeToExplode, blow it up.
   if (doubleTime() - timeFired > timeToExplode) {
      explode();
   }

      // Set a flag here to say that we exploded this frame, and come back on the next frame?
      // Remove the bomb in the gameState's update if that flag is set?
      // Find out how projectileShot's are removed.
   }

void TimedBombShot::handleCollision(Drawable* other) {  Shot::handleCollision(other); 
}

void TimedBombShot::hitWall(BoundingWall* wall) {
   //particleDirection.reflect(wall->normal);
}

/**
 * Called when the bomb should be exploding.
 * Make the size of the bomb much bigger so that it will collide with
 * everything it should, and auto-handle the collisions.
 */
void TimedBombShot::explode() {
   printf("Bomb exploded!\n");
   // Do all the generic exploding actions that every bomb should do.
   ExplosiveShot::explode();

   // Play a sound
   SoundEffect::playSoundEffect("Explosion1.wav");

   // play an animation
   // set it as not collidable
}
