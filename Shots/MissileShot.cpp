/**
 * MissileShot.cpp
 * Blaster Shots!
 * Pew pew pew!
 */

#include "Shots/MissileShot.h"
#include "Particles/BlasterShotParticle.h"
#include "Particles/BlasterImpactParticle.h"
#include "Utility/SoundEffect.h"

#ifdef WIN32
#include "Utility/WindowsMathLib.h"
#endif

const int particleCycle = 100;
static float spinCycle = 0;

MissileShot::MissileShot(Point3D& posIn, Vector3D dirIn,
 AsteroidShip* const ownerIn, const GameState* _gameState) : Shot(posIn, dirIn, ownerIn, _gameState) {
   persist = false;
   minX = minY = minZ = -0.1;
   maxX = maxY = maxZ = 0.1;
   lifetime = 20;
   updateBoundingBox();
   static int currentStartingParticleCycle = 0;
   particleNum = currentStartingParticleCycle;
   currentStartingParticleCycle = (currentStartingParticleCycle + 7) % particleCycle;
   particleDirection = velocity->getNormalVector();
   
   Vector3D normalizedVelocity(*velocity);
   normalizedVelocity.normalize();

   particleDirection.rotate(randdouble() * 2 * M_PI, normalizedVelocity);
}

void MissileShot::draw() {
   double random, posOrNeg;
   double x, y, z;
   Vector3D zVector(0, 0, -1);
   Vector3D axis = zVector.cross(*velocity);
   glPushMatrix();
      glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
      glDisable(GL_LIGHTING);
      
      setMaterial(WhiteSolid);
      position->glTranslate();
      glRotated(zVector.getAngleInDegrees(*velocity), 
         axis.x, axis.y, axis.z);
         glPushMatrix();
         random = (double) (rand() % 4);
         random /= 10;
         glRotated(spinCycle++, random, random, random);
         glLineWidth(3.0);
         glColor4d(1, 1, 0, 1);
            glBegin(GL_LINE_LOOP);
            for(double j = 0; j < 2; j += .1) {
                  random = (double) (rand() % 4);
                  posOrNeg = (double) (rand() % 2);
                  posOrNeg = 1 - (posOrNeg * 2);
                  random /= 10 * posOrNeg;
                  x = (1.1 * cos(j * M_PI)) + random;
                  random = (double) (rand() % 4);
                  posOrNeg = (double) (rand() % 2);
                  posOrNeg = 1 - (posOrNeg * 2);
                  random /= 10 * posOrNeg;
                  y = (1.1 * sin(j * M_PI)) + random;
                  z = 0;
                  glVertex3d(x, y, z);
            }
            glEnd();
            
            glBegin(GL_LINE_LOOP);
            for(double j = 0; j < 2; j += .1) {
                  random = (double) (rand() % 4);
                  posOrNeg = (double) (rand() % 2);
                  posOrNeg = 1 - (posOrNeg * 2);
                  random /= 10 * posOrNeg;
                  x = (1.1 * cos(j * M_PI)) + random;
                  
                  random = (double) (rand() % 4);
                  posOrNeg = (double) (rand() % 2);
                  posOrNeg = 1 - (posOrNeg * 2);
                  random /= 10 * posOrNeg;
                  z = (1.1 * sin(j * M_PI)) + random;
                  y = 0;
                  glVertex3d(x, y, z);
            }
            glEnd();

            glBegin(GL_LINE_LOOP);
            for(double j = 0; j < 2; j += .1) {
                  random = (double) (rand() % 4);
                  posOrNeg = (double) (rand() % 2);
                  posOrNeg = 1 - (posOrNeg * 2);
                  random /= 10 * posOrNeg;
                  z = (1.1 * cos(j * M_PI)) + random;
                  random = (double) (rand() % 4);
                  posOrNeg = (double) (rand() % 2);
                  posOrNeg = 1 - (posOrNeg * 2);
                  random /= 10 * posOrNeg;
                  y = (1.1 * sin(j * M_PI)) + random;
                  x = 0;
                  glVertex3d(x, y, z);
            }
            glEnd();
            glPopMatrix();
         
      glColor4d(0, 1, 1, .3);
      gluSphere(quadric, .2, 20, 20);
      gluSphere(quadric, .3, 20, 20);
      gluSphere(quadric, .4, 20, 20);
      gluSphere(quadric, .5, 20, 20);
      gluSphere(quadric, .6, 20, 20);
      gluSphere(quadric, .7, 20, 20);
      gluSphere(quadric, .8, 20, 20);
      gluSphere(quadric, .9, 20, 20);
      gluSphere(quadric, 1, 20, 20);
      
      glEnable(GL_LIGHTING);

   glPopMatrix();
}

// Spray particles.
void MissileShot::update(double timeDiff) {
   Shot::update(timeDiff);
   const int particlesPerSecond = 1;
   
   //Vector3D normalizedVelocity(*velocity);
   //normalizedVelocity.normalize();
   //const double particleDirectionPeriod = 0.3;
   //const double particleDirectionRotationSpeed = 2 * M_PI / particleDirectionPeriod;
   //particleDirection.rotate(timeDiff * particleDirectionRotationSpeed, normalizedVelocity);
   //particleDirection.normalize();
   
   for (int i = 0; i <= timeDiff * particlesPerSecond; ++i) {
      particleNum = (particleNum + 1) % particleCycle;
      // This is the random way...
      particleDirection.randomMagnitude();
      particleDirection.setLength(0.01);
      
      /*BlasterShotParticle::AddRainbow(new Point3D(*position), 
       new Vector3D(particleDirection), particleNum, particleCycle, gameState);*/
      // Reflect and Duplicate the above for a double helix.
   }
}

void MissileShot::hitWall(BoundingWall* wall) {
   ExplosiveShot::hitWall(wall);
}
