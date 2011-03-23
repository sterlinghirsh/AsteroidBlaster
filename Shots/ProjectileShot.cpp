/**
 * ProjectileShot.cpp
 * Blaster Shots!
 * Pew pew pew!
 */

#include "Shots/ProjectileShot.h"
#include "Particles/BlasterShotParticle.h"
#include "Particles/BlasterImpactParticle.h"
#include "Utility/SoundEffect.h"
   
const int particleCycle = 100;

ProjectileShot::ProjectileShot(Point3D& posIn, Vector3D dirIn,
 AsteroidShip* const ownerIn ) : Shot(posIn, dirIn, ownerIn) {
   persist = false;
   minX = minY = minZ = -0.1;
   maxX = maxY = maxZ = 0.1;
   lifetime = 5;
   updateBoundingBox();
   static int currentStartingParticleCycle = 0;
   particleNum = currentStartingParticleCycle;
   currentStartingParticleCycle = (currentStartingParticleCycle + 7) % particleCycle;
   particleDirection = velocity->getNormalVector();
   
   Vector3D normalizedVelocity(*velocity);
   normalizedVelocity.normalize();

   particleDirection.rotate(randdouble() * 2 * M_PI, normalizedVelocity);
}

void ProjectileShot::draw() {
   Vector3D zVector(0, 0, -1);
   Vector3D axis = zVector.cross(*velocity);
   glPushMatrix();
   glDisable(GL_LIGHTING);
   glColor3f(1, 0, 0);
   setMaterial(ShotMaterial);
   position->glTranslate();
   glRotated(zVector.getAngleInDegrees(*velocity), 
      axis.xMag, axis.yMag, axis.zMag);
   gluCylinder(quadric,0.08f,0.0f,0.8f,6,6);
   glEnable(GL_LIGHTING);

   glPopMatrix();
}

// Spray particles.
void ProjectileShot::update(double timeDiff) {
   Shot::update(timeDiff);
   const int particlesPerSecond = 4;
   
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
      particleDirection.setLength(0.1);
      
      // And this is the helix way.
      BlasterShotParticle::AddRainbow(new Point3D(*position), 
       new Vector3D(particleDirection), particleNum, particleCycle);
      // Reflect and Duplicate the above for a double helix.
   }
}

void ProjectileShot::handleCollision(Drawable* other) {
   const int particlesToEmit = 10;
   
   static Vector3D particleVariation;
   static Vector3D positionDifference;
   const double particleSpeed = 15;
   // This should probably be moved to the Asteroid's code.

   Asteroid3D* asteroid;
   // If we hit an asteroid.
   if ((asteroid = dynamic_cast<Asteroid3D*>(other)) != NULL) {
      SoundEffect::playSoundEffect("BlasterHit.wav");
      // Make some particles!
      positionDifference.updateMagnitude(*asteroid->position, *position);
      positionDifference.setLength(particleSpeed);
      for (int i = 0; i <= particlesToEmit; ++i) {
         particleVariation.randomMagnitude();
         particleVariation.setLength(particleSpeed);
         particleVariation.addUpdate(positionDifference);
         BlasterImpactParticle::Add(new Point3D(*position), 
          new Vector3D(particleVariation));
      }
   }
   Shot::handleCollision(other);
}

void ProjectileShot::hitWall(BoundingWall* wall) {
   //particleDirection.reflect(wall->normal);
}
