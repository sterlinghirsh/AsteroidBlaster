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
   timeToExplode = 100;
   scaleSize = 0;
   secondScale = 0;
   explodeRadius = 8;
   addSize = 0;
   spin = 90;
   //isExploded = true;
   
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
      glDisable(GL_CULL_FACE);
      glDisable(GL_LIGHTING);
      glEnable(GL_NORMALIZE);
      //glDisable(GL_BLEND);
      glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
      glDisable(GL_POLYGON_OFFSET_LINE);

      /*double amountDone = (doubleTime() - timeFired) / timeToExplode;
      if (fmod(amountDone * amountDone * 80, 2) < 1) {
         glColor3d(0.4, 0.8, 0.4);
      } else {
         glColor3d(1.0, 0.4, 0.4);
      }*/
      //glDisable(GL_
      glColor4d(1, 0, 0, 1);
      //setMaterial(BlackSolid);
      position->glTranslate();
      glRotate();
      
      glPushMatrix();
      gluSphere(quadric, 0.3, 30, 30);
      
      glPopMatrix();
      
      glPushMatrix();
      glColor4d(.0, 0, .0, 1);
      gluCylinder(quadric, .31, .31, 1.5, 30, 30);
      glPopMatrix();
      
      glPushMatrix();
      glTranslated(0, 0, 1.5);
      gluDisk(quadric, 0, .3, 15, 15);
      
      glPopMatrix();
      
      glLineWidth(2);
      glPushMatrix();
      glBegin(GL_LINES);
      glColor4d(1, 0, 0, 1);
      for(double z = 0; z <= 1.5; z += .1) {
         glVertex3d(.33 * sin((z / 1.5) * M_PI * 2), .33 * cos((z / 1.5) * M_PI * 2), z);
         glVertex3d(.33 * sin(((z + .1) / 1.5) * M_PI * 2), .33 * cos(((z + .1) / 1.5) * M_PI * 2), z + .1);
         
         glVertex3d(-.33 * sin((z / 1.5) * M_PI * 2), -.33 * cos((z / 1.5) * M_PI * 2), z);
         glVertex3d(-.33 * sin(((z + .1) / 1.5) * M_PI * 2), -.33 * cos(((z + .1) / 1.5) * M_PI * 2), z + .1);
      }
      glEnd();
      glPopMatrix();
      
      glLineWidth(1.0);
      glEnable(GL_BLEND);
      glEnable(GL_CULL_FACE);
      glEnable(GL_LIGHTING);
   glPopMatrix();
}

void TimedBombShot::drawExplosion() {
   double xzScale;;
   glPushMatrix();
      GLint loc1;
      glDisable(GL_LIGHTING);
      glDisable(GL_CULL_FACE);
      setMaterial(ShotMaterial);
      position->glTranslate();
      glRotate();

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
      //printf("Time since exploded: %f\n", helper);
      glColor4d(1, .6, 0, timeSinceExploded - .1);

      //addSize = timeSinceExploded / (2.5 - timeSinceExploded);
      if (scaleSize > 15) {
         scaleSize += addSize / 15;
      } else {
         scaleSize += addSize;
      }
      
      xzScale = scaleSize * (2.3 - timeSinceExploded) * (2.3 - timeSinceExploded) / 2.5, scaleSize;
      
      loc1 = glGetUniformLocation(ringShader,"ratio");
      glUniform1f(loc1,helper);
      //scaleSize = 20;
      //secondScale = 30;
      
      /*if (scaleSize > secondScale) {
         glPushMatrix();
         glRotated(spin, rx, ry, rz);
         glScaled(scaleSize, scaleSize, scaleSize);
         
         gluSphere(quadric, .6, 20, 20);
         glPopMatrix();
      }*/
      //glRotated(spin, rx, ry, rz);
      if(timeSinceExploded < 1.3) {
         glUseProgram(ringShader);
         glPushMatrix();/*
         glColor4d(1, 0, 0, timeSinceExploded * 5);
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
         glPopMatrix();*/
         glColor4d(0, .2, 1, timeSinceExploded*5);
         glPushMatrix();
         //glColor4d(0, 0, 1, 1);//timeSinceExploded * .3);
         glRotated(90, 1, 0, 0);
         glScaled(secondScale, secondScale, 1);
         glPushMatrix();
         glTranslated(0, 0, -.3);
         gluDisk(quadric, .1, .6, 20, 20);
         glPopMatrix();
         glPushMatrix();
         glTranslated(0, 0, .3);
         gluDisk(quadric, .1, .6, 20, 20);
         glPopMatrix();
         glPushMatrix();
         glTranslated(0, 0, -.3);
         gluCylinder(quadric, .6, .6, .6, 20, 20);
         glPopMatrix();
         glPopMatrix();
         /*
         glPushMatrix();
         //glColor4d(0, 0, 1, 1);//timeSinceExploded * .3);
         glScaled(1, secondScale, secondScale);
         glRotated(90, 0, 1, 0);
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
         glPopMatrix();*/
         glPopMatrix();
         glUseProgram(explosionShader);
      }
      //if (scaleSize <= secondScale) {
      glColor4d(1, .6, 0, timeSinceExploded - .1);
         glPushMatrix();
         if(timeSinceExploded > 1.3) {
            glScaled(scaleSize / 2.5, scaleSize, scaleSize / 2.5);
         } else {
            glScaled(xzScale, scaleSize, xzScale);
         }
         //glRotated(20, 1, 0, 0);
         glRotated(spin, rx, ry, rz);
         gluSphere(quadric, .6, 20, 20);
         glPopMatrix();
      //}

      glPopMatrix();
      glUseProgram(0);
      glEnable(GL_CULL_FACE);
      glEnable(GL_LIGHTING);
   glPopMatrix();
}

void TimedBombShot::update(double timeDiff) {
   if(timeSinceExploded > 0) {
      if (timeSinceExploded < 1.3) {
         secondScale += 50 * timeDiff;
      }
      addSize = timeDiff * 70 * timeSinceExploded / (2.5 - timeSinceExploded);
   }
   

   addSize += timeDiff;
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
      timeSinceExploded = 1.5;
      isExploded = true;
      //explode();
   }
   //printf("Time since exploded: %f\n", doubleTime() - timeFired);
   // If more time has passed than the bomb's timeToExplode, blow it up.
   if (!isExploded && (doubleTime() - timeFired > timeToExplode)) {
      timeSinceExploded = 1.5;
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
