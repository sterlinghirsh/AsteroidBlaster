/**
 * HomingMissileShot.cpp
 * This bomb seeks shit out.
 */

#include "Shots/HomingMissileShot.h"
#include "Shots/ExplosiveShot.h"
#include "Utility/SoundEffect.h"
#include "Items/BoundingWall.h"
#include "Particles/BlasterShotParticle.h"

#ifdef WIN32
#include "Utility/WindowsMathLib.h"
#endif

#include "Network/gamestate.pb.h"

bool shouldChangeY = true;
bool shouldChangeZ = false;
const int particleCycle = 100;

HomingMissileShot::HomingMissileShot(Point3D& posIn, Vector3D dirIn, int _weaponIndex, AsteroidShip* const ownerIn, const GameState* _gameState) : ExplosiveShot(posIn, dirIn, _weaponIndex, ownerIn, _gameState) {
   type = TYPE_HOMINGMISSILESHOT;
   seekRadius = 80.0;
   radius = seekRadius;
   minX = minY = minZ = -radius;
   maxX = maxY = maxZ = radius;
   updateBoundingBox();

   // Blow up 15 seconds after it's fired.
   timeToExplode = 20;
   scaleSize = 0;
   secondScale = 0;
   explodeRadius = 1;
   addSize = 0;
   spin = 90;
   
   particleColor = randdouble();
   
   targetID = -1;
   
   hasTarget = false;
   
   hasDamaged = false;
   
   //rotationSpeed = randdouble() * 10; // Degrees per sec.
   //axis = new Vector3D(0, 1, 0);
   //axis->randomMagnitude();
   //axis->normalize();
   
   rx = 0;
   ry = 0;
   rz = 0;
   
   static int currentStartingParticleCycle = 0;
   particleNum = currentStartingParticleCycle;
   currentStartingParticleCycle = (currentStartingParticleCycle + 7) % particleCycle;
   particleDirection = velocity->getNormalVector();
   
   Vector3D normalizedVelocity(*velocity);
   normalizedVelocity.normalize();

   particleDirection.rotate(randdouble() * 2 * M_PI, normalizedVelocity);
   
   rollSpeed = randdouble() * 50 - 25;
   pitchSpeed = 0;
   yawSpeed = 0;
   
   forward = new Vector3D((owner->shotDirection));
   up = new Vector3D(*(owner->up));
   right = new Vector3D(*(owner->right));
   
   
   timeSinceExploded = -1;
   
   damage = 3;
   slowDownPerSecond = 40.0;
   collisionRadius = 0.5;
   collisionSphere->updateRadius(seekRadius);

   soundHandle = SoundEffect::playSoundEffect("MissileLaunch", position, velocity, false);
}

// Needed to avoid obscure vtable compiler error.
HomingMissileShot::~HomingMissileShot() {
   // Don't stop the sound.
}

void HomingMissileShot::draw() {
   /*
   float rando = (float)(rand() % 420) / 70 - 3;
   //printf("Random number: %f\n", rando);
   GLint loc1;
   GLint loc2;
   GLint loc3;
   GLint loc4;
   glPushMatrix();
   position->glTranslate();
   glRotate();
   
   glUseProgram(blackHoleShader);
   loc1 = glGetUniformLocation(blackHoleShader,"randomColor");
   glUniform1f(loc1,(float)rando);
   //loc2= glGetUniformLocation(blackHoleShader,"swirlX");
   //glUniform1f(loc2,(float)rx);
   loc3 = glGetUniformLocation(blackHoleShader,"swirlY");
   glUniform1f(loc3,(float)ry);
   loc4 = glGetUniformLocation(blackHoleShader,"swirlZ");
   glUniform1f(loc4,(float)rz);
   
   glPushMatrix();
   glDisable(GL_CULL_FACE);
   glDisable(GL_LIGHTING);
   glEnable(GL_NORMALIZE);
   
   glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
   glDisable(GL_POLYGON_OFFSET_LINE);

   
   gluSphere(quadric, 3, 20, 20);
   
   
   glPopMatrix();
   glUseProgram(0);
   glPopMatrix();*/
   
   
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
      
      //glTranslated(0, 0, -.75);
      //glRotated(spin, 0, 0, 1);
      glRotated(180, 0, 1, 0);
      
      /*glPushMatrix();
      
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
      
      glPopMatrix();*/
      
      /*glLineWidth(2);
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
      glPopMatrix();*/
      
      glPushMatrix();
      glPushMatrix();
      /*if (targetID == -1) {
         glColor4d(0, 1, 0, 1);
      } else {
         glColor4d(1, 0, 0, 1);
      }*/
      glColor4d(0, 0, 0, 1);
      glPushMatrix();
      glTranslated(0, .2, 0);
      glBegin(GL_TRIANGLES);
         glVertex3d(0, 0, 0);
         glVertex3d(0, 0, 1);
         glVertex3d(0, .3, 0);
      glEnd();
      glPopMatrix();
      
      glPushMatrix();
      glRotated(90, 0, 0, 1);
      glTranslated(0, .2, 0);
      glBegin(GL_TRIANGLES);
         glVertex3d(0, 0, 0);
         glVertex3d(0, 0, 1);
         glVertex3d(0, .3, 0);
      glEnd();
      glPopMatrix();
      
      glPushMatrix();
      glRotated(180, 0, 0, 1);
      glTranslated(0, .2, 0);
      glBegin(GL_TRIANGLES);
         glVertex3d(0, 0, 0);
         glVertex3d(0, 0, 1);
         glVertex3d(0, .3, 0);
      glEnd();
      glPopMatrix();
      
      glPushMatrix();
      glRotated(270, 0, 0, 1);
      glTranslated(0, .2, 0);
      glBegin(GL_TRIANGLES);
         glVertex3d(0, 0, 0);
         glVertex3d(0, 0, 1);
         glVertex3d(0, .3, 0);
      glEnd();
      glPopMatrix();
      
      glPushMatrix();
         glTranslated(0, 0, 1.5);
         gluCylinder(quadric, .2, 0, .5, 30, 30);
      glPopMatrix();
      
      glPopMatrix();
      if (targetID == -1) {
         glColor4d(0, 1, 0, 1);
      } else {
         glColor4d(1, 0, 0, 1);
      }
      gluCylinder(quadric, .2, .2, 1.5, 30, 30);
      gluDisk(quadric, 0, .2, 30, 30);
      glPopMatrix();

      glLineWidth(1.0);
      glEnable(GL_BLEND);
      glEnable(GL_CULL_FACE);
      glEnable(GL_LIGHTING);
   glPopMatrix();
}

void HomingMissileShot::drawGlow(){
   draw();
}

double HomingMissileShot::getDamage(Object3D* other) {
   if (hasDamaged) {
      return 0;
   } else {
      return damage;
   }
}

void HomingMissileShot::drawExplosion() {
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

void HomingMissileShot::update(double timeDiff) {
   SoundEffect::updateSource(soundHandle, position, velocity);

   const int particlesPerSecond = 4;
   Vector3D* shotAccel;
   
   if (!isExploded) {
      roll(rollSpeed * timeDiff);
      pitch(pitchSpeed * timeDiff);
      yaw(yawSpeed * timeDiff);
   }
   
   shotAccel = new Vector3D(*forward);
   
   shotAccel->setLength(15+ (owner->weapons[HOMINGMISSILE_WEAPON_INDEX]->level*3));
   addAcceleration(shotAccel);
   
   double speed = velocity->getLength();
   if (gameState->getGameTime() - timeFired > .3) {
      velocity->updateMagnitude(forward->scalarMultiply(speed));
   } else {
      shotAccel = new Vector3D(*forward);
   
      shotAccel->setLength(30);
      addAcceleration(shotAccel);
   }
   if(timeSinceExploded > 0) {
      if (timeSinceExploded < .6) {
         secondScale += 30 * timeDiff;
      }
      addSize = timeDiff * 14 * timeSinceExploded / (1.6 - timeSinceExploded);
   }
   
   addSize += timeDiff;
   if(!isExploded){
      if (targetID != -1) {
         Object3D* target = gameState->custodian[targetID];
         if (target != NULL) {
            Vector3D positionToAsteroid(*position, *target->position);
            Vector3D toAsteroidNorm(positionToAsteroid);
            toAsteroidNorm.normalize();
            Point3D futurePosition(*(target->position));
      
            Vector3D toFuturePositionNorm(*position, futurePosition);
            toFuturePositionNorm.normalize();
            
            
            double upAmount;
            double rightAmount;
            double forwardAmount;
            
            upAmount = up->dot(toFuturePositionNorm);
            rightAmount = right->dot(toFuturePositionNorm);
            forwardAmount = forward->dot(toFuturePositionNorm);
            
            if (forwardAmount > .75) {
          
               setPitchSpeed(-15 * upAmount);
            
               setYawSpeed(-15 * rightAmount);
               if (forwardAmount > .98) {
                  Vector3D* shotAccel;
                  shotAccel = new Vector3D(*forward);
                  shotAccel->setLength(100);
                  addAcceleration(shotAccel);
               } else if (targetID != -1) {
                  addAcceleration(new Vector3D(velocity->scalarMultiply(-0.2)));
               }
            } else {
               //rollSpeed = 0;
               yawSpeed = 0;
               pitchSpeed = 0;
               targetID = -1;
            }
         } else {
            targetID = -1;
         }
      }
      
      
      for (int i = 0; i <= timeDiff * particlesPerSecond; ++i) {
         particleNum = (particleNum + 1) % particleCycle;
         // This is the random way...
         particleDirection.randomMagnitude();
         particleDirection.setLength(0.1);
         
         BlasterShotParticle::AddColor(new Point3D(*position), 
          new Vector3D(particleDirection), particleColor, gameState);
         // Reflect and Duplicate the above for a double helix.
      }
      
      Vector3D* shotAccel;
      shotAccel = new Vector3D(*forward);
      
      shotAccel->setLength(15);
      addAcceleration(shotAccel);

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
   if ((shouldExplode || gameState->getGameTime() - timeFired > timeToExplode) && !isExploded) {
      timeSinceExploded = 1;
      isExploded = true;
      //explode();
      //SoundEffect::playSoundEffect("HomingMissileExplosion", position, false, 255);
   }
   
   timeSinceExploded -= timeDiff;
}

void HomingMissileShot::hitWall(BoundingWall* wall) {
   // Check to see if we're actually within exploderadius.
   
   if (shouldExplode || isExploded) {
      wall->actuallyHit = false;
   }
   
   if (!isExploded) {
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
   
}

/**
 * Called when the bomb should be exploding.
 * Make the size of the bomb much bigger so that it will collide with
 * everything it should, and auto-handle the collisions.
 */
void HomingMissileShot::explode() {
   // Do all the generic exploding actions that every bomb should do.
   ExplosiveShot::explode();

   // Play a sound
   SoundEffect::playSoundEffect("Explosion1.wav", position, NULL);

   // set it as not collidable
}

bool HomingMissileShot::saveDiff(const ast::Entity& old, ast::Entity* ent) {
   bool changed = Shot::saveDiff(old, ent);
   
   // Maybe make it calculate one from the others?
   if (!up->saveDiff(old.up(), ent->mutable_up()))
      ent->clear_up();
   else
      changed = true;
   
   if (!forward->saveDiff(old.forward(), ent->mutable_forward()))
      ent->clear_forward();
   else
      changed = true;
   
   /*
   if (!right->saveDiff(old.right(), ent->mutable_right()))
      ent->clear_right();
   else
      changed = true;
      */


   return changed;
}

void HomingMissileShot::save(ast::Entity* ent) {
   Shot::save(ent);
   
   up->save(ent->mutable_up());
   forward->save(ent->mutable_forward());
   // right->save(ent->mutable_right());
}

void HomingMissileShot::load(const ast::Entity& ent) {
   Shot::load(ent);

   if (ent.has_forward())
      forward->load(ent.forward());
   /*
   if (ent.has_right())
      right->load(ent.right());
      */
   if (ent.has_up())
      up->load(ent.up());
   *right = forward->cross(*up);
}
