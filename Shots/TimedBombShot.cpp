/**
 * TimedBombShot.cpp
 * This bomb lasts for a few seconds before exploding.
 */

#include "Shots/TimedBombShot.h"
#include "Shots/ExplosiveShot.h"
#include "Utility/SoundEffect.h"
#include "Graphics/Sprite.h"
#include "Items/BoundingWall.h"

#ifdef WIN32
#include "Utility/WindowsMathLib.h"
#endif


TimedBombShot::TimedBombShot(Point3D& posIn, Vector3D dirIn, int _weaponIndex, AsteroidShip* const ownerIn, const GameState* _gameState) : ExplosiveShot(posIn, dirIn, _weaponIndex, ownerIn, _gameState) {
   radius = seekRadius;
   minX = minY = minZ = -radius;
   maxX = maxY = maxZ = radius;
   updateBoundingBox();

   // Blow up 15 seconds after it's fired.
   timeToExplode = 15;
   scaleSize = 0;
   secondScale = 0;
   explodeRadius = 8;
   addSize = 0;
   spin = 90;
   
   rotationSpeed = randdouble() * 10; // Degrees per sec.
   axis = new Vector3D(0, 1, 0);
   axis->randomMagnitude();
   axis->normalize();
   
   rx = 0;
   ry = 0;
   rz = 0;
   
   forward = new Vector3D((owner->shotDirection));
   up = new Vector3D(*(owner->up));
   right = new Vector3D(*(owner->right));
   
   timeSinceExploded = -1;
   
   damage = 40;
   slowDownPerSecond = 1.0;
   seekRadius = 20.0;
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
      glColor4d(1, 1, 1, 1);
      glDisable(GL_CULL_FACE);
      glDisable(GL_LIGHTING);
      glEnable(GL_NORMALIZE);

      glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
      glDisable(GL_POLYGON_OFFSET_LINE);

      position->glTranslate();
      glRotate();
      
      glTranslated(0, 0, -.75);
      
      glPushMatrix();
      
      gluCylinder(quadric, .35, .35, .2, 30, 30);
      glPushMatrix();
      gluDisk(quadric, 0, .4, 30, 30);
      gluDisk(quadric, .35, .4, 30, 30);
      glTranslated(0, 0, .2);
      gluDisk(quadric, .35, .4, 30, 30);
      glPopMatrix();
      
      gluCylinder(quadric, .4, .4, .2, 30, 30);
      
      glTranslated(0, 0, 1.3);
      
      gluCylinder(quadric, .35, .35, .2, 30, 30);
      
      glPushMatrix();
      gluDisk(quadric, .35, .4, 30, 30);
      glTranslated(0, 0, .2);
      gluDisk(quadric, .35, .4, 30, 30);
      gluDisk(quadric, 0, .4, 30, 30);
      glPopMatrix();
      
      gluCylinder(quadric, .4, .4, .2, 30, 30);
      
      glPopMatrix();
      
      glLineWidth(2);
      glPushMatrix();
      glRotated(spin/2, 0, 0, 1);
      glBegin(GL_LINES);

      glVertex3d(.32, 0, 0);
      glVertex3d(.32, 0, .5);
      glVertex3d(-.32, 0, 0);
      glVertex3d(-.32, 0, .5);
      glVertex3d(0, .32, 0);
      glVertex3d(0, .32, .5);
      glVertex3d(0, -.32, 0);
      glVertex3d(0, -.32, .5);
      glEnd();
      glPopMatrix();
      
      glPushMatrix();
      glColor4d(0, 1, 0, 1);
      glRotated(-spin, 0, 0, 1);
      glBegin(GL_LINES);
      glVertex3d(.32, 0, .5);
      glVertex3d(.32, 0, 1);
      glVertex3d(-.32, 0, .5);
      glVertex3d(-.32, 0, 1);
      glVertex3d(0, .32, .5);
      glVertex3d(0, .32, 1);
      glVertex3d(0, -.32, .5);
      glVertex3d(0, -.32, 1);
      glEnd();
      glPopMatrix();
      
      glPushMatrix();
      glColor4d(1, 1, 1, 1);
      glRotated(spin*2, 0, 0, 1);
      glBegin(GL_LINES);
      glVertex3d(.32, 0, 1);
      glVertex3d(.32, 0, 1.5);
      glVertex3d(-.32, 0, 1);
      glVertex3d(-.32, 0, 1.5);
      glVertex3d(0, .32, 1);
      glVertex3d(0, .32, 1.5);
      glVertex3d(0, -.32, 1);
      glVertex3d(0, -.32, 1.5);
      glEnd();
      glPopMatrix();
      
      glPushMatrix();
      glBegin(GL_LINES);
      glColor4d(.5, .5, 0, 1);
      double rando;
      
      glVertex3d(.4, 0, .2);
      for(double z = .3; z < 1.3; z+=.1){
         rando = randdouble();
         glVertex3d(.4 + (rando * .1), (.5 - rando) * .2, z);
         glVertex3d(.4 + (rando * .1), (.5 - rando) * .2, z);
      }
      
      glVertex3d(.4, 0, 1.3);
      
      glVertex3d(-.4, 0, .2);
      
      for(double z = .2; z < 1.2; z+=.1){
         rando = randdouble();
         glVertex3d(-.4 + (rando * -.1), (.5 - rando) * .2, z);
         glVertex3d(-.4 + (rando * -.1), (.5 - rando) * .2, z);
      }
      
      glVertex3d(-.4, 0, 1.3);
      
      
      
      glVertex3d(0, .4, .2);
      for(double z = .3; z < 1.3; z+=.1){
         rando = randdouble();
         glVertex3d((.5 - rando) * .2, .4 + (rando * .1), z);
         glVertex3d((.5 - rando) * .2, .4 + (rando * .1), z);
      }
      
      glVertex3d(0, .4, 1.3);
      
      glVertex3d(0, -.4, .2);
      
      for(double z = .2; z < 1.2; z+=.1){
         rando = randdouble();
         glVertex3d((.5 - rando) * .2, -.4 + (rando * -.1), z);
         glVertex3d((.5 - rando) * .2, -.4 + (rando * -.1), z);
      }
      
      glVertex3d(0, -.4, 1.3);
      glEnd();
      glPopMatrix();
      
      glPushMatrix();
      glColor4d(0, 0, 0, 1);
      gluCylinder(quadric, .31, .31, 1.5, 30, 30);
      glPopMatrix();

      glLineWidth(1.0);
      glEnable(GL_BLEND);
      glEnable(GL_CULL_FACE);
      glEnable(GL_LIGHTING);
   glPopMatrix();
}

void TimedBombShot::drawGlow(){
   draw();
}

void TimedBombShot::drawExplosion() {
   double xzScale;
   double subtractor = 1.6;
   double divider = 3.5;
   double ringTime = .6;
   glPushMatrix();
      GLint loc1;
      glDisable(GL_LIGHTING);
      glDisable(GL_CULL_FACE);
      setMaterial(ShotMaterial);
      position->glTranslate();
      glRotate();
      glRotated(90, 1, 0, 0);

      rx += (double) (rand() % 10);
      ry += (double) (rand() % 20);
      rz += (double) (rand() % 30);
      
      float helper = (float) (secondScale * .6);

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
      glUseProgram(explosionShader);
      glPushMatrix();
      glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
      glDisable(GL_LIGHTING);

      glColor4d(1, .6, 0, timeSinceExploded);

      if (scaleSize > 15) {
         scaleSize += addSize / 15;
      } else {
         scaleSize += addSize;
      }
      
      xzScale = scaleSize * (subtractor - timeSinceExploded) * (subtractor - timeSinceExploded) / divider;
      
         glPushMatrix();
         if(timeSinceExploded > ringTime) {
            glScaled(scaleSize / divider, scaleSize, scaleSize / divider);
         } else {
            glScaled(xzScale, scaleSize, xzScale);
         }
         glRotated(spin, rx, ry, rz);
         gluSphere(quadric, .6, 20, 20);
         glPopMatrix();
      
      loc1 = glGetUniformLocation(ringShader,"ratio");
      glUniform1f(loc1,helper);

      if(timeSinceExploded < ringTime) {
         glUseProgram(ringShader);
         glPushMatrix();
         glColor4d(0, .2, 1, timeSinceExploded*4);
         glRotated(90, 1, 0, 0);
         glScaled(secondScale, secondScale, 1);
         glPushMatrix();
         glTranslated(0, 0, -.3);
         gluDisk(quadric, .3, .6, 20, 20);
         glPopMatrix();
         glPushMatrix();
         glTranslated(0, 0, .3);
         gluDisk(quadric, .3, .6, 20, 20);
         glPopMatrix();
         glPushMatrix();
         glTranslated(0, 0, -.3);
         gluCylinder(quadric, .6, .6, .6, 20, 20);
         glPopMatrix();
         glPopMatrix();
         glUseProgram(explosionShader);
      }

      glColor4d(1, .6, 0, timeSinceExploded);
         glPushMatrix();
         if(timeSinceExploded > ringTime) {
            glScaled(scaleSize / divider, scaleSize, scaleSize / divider);
         } else {
            glScaled(xzScale, scaleSize, xzScale);
         }
         glRotated(spin, rx, ry, rz);
         gluSphere(quadric, .6, 20, 20);
         glPopMatrix();

      glPopMatrix();
      glUseProgram(0);
      glEnable(GL_CULL_FACE);
      glEnable(GL_LIGHTING);
   glPopMatrix();
}

void TimedBombShot::update(double timeDiff) {
   spin += 200 * timeDiff;
   if(timeSinceExploded > 0) {
      if (timeSinceExploded < .6) {
         secondScale += 100 * timeDiff;
      }
      addSize = timeDiff * 140 * timeSinceExploded / (1.6 - timeSinceExploded);
   }
   

   addSize += timeDiff;
   if(!isExploded){
      up->rotate(rotationSpeed * timeDiff, axis);
      right->rotate(rotationSpeed * timeDiff, axis);
      forward->rotate(rotationSpeed * timeDiff, axis);
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
   // If more time has passed than the bomb's timeToExplode, blow it up.
   if ((shouldExplode || doubleTime() - timeFired > timeToExplode) && !isExploded) {
      timeSinceExploded = 1;
      isExploded = true;
      //explode();
      SoundEffect::playSoundEffect("TimedBombExplosion", position, false, 255);
   }
   
   timeSinceExploded -= timeDiff;
}

void TimedBombShot::hitWall(BoundingWall* wall) {
   // Check to see if we're actually within exploderadius.
   minX = minY = minZ = -collisionRadius;
   maxX = maxY = maxZ = collisionRadius;
   updateBoundingBox();

   ExplosiveShot::hitWall(wall);
   
   if (wall->actuallyHit) {
      shouldExplode = true;   
   }

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
