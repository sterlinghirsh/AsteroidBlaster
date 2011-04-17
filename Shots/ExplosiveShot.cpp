/**
 * ExplosiveShot.cpp
 * All bombs, missiles, etc will extend this in some way.
 */

#include "Shots/ExplosiveShot.h"
#include "Particles/BlasterShotParticle.h"
#include "Particles/BlasterImpactParticle.h"
#include "Utility/SoundEffect.h"

#ifdef WIN32
#include "Utility/WindowsMathLib.h"
#endif

//const int particleCycle = 100;

ExplosiveShot::ExplosiveShot(Point3D& posIn, Vector3D dirIn,
 AsteroidShip* const ownerIn, const GameState* _gameState) : Shot(posIn, dirIn, ownerIn, _gameState) {
   persist = false;
   minX = minY = minZ = -0.5;
   maxX = maxY = maxZ = 0.5;
   /* When isExploded becomes true, we check for collisions within the
      explodeRadius. */
   explodeRadius = 15;
   lifetime = 30;
   // All ExplosiveShots should start off not yet exploded.
   isExploded = false;
   updateBoundingBox();
   /*
   static int currentStartingParticleCycle = 0;
   particleNum = currentStartingParticleCycle;
   currentStartingParticleCycle = (currentStartingParticleCycle + 7) % particleCycle;
   particleDirection = velocity->getNormalVector();
   
   Vector3D normalizedVelocity(*velocity);
   normalizedVelocity.normalize();

   particleDirection.rotate(randdouble() * 2 * M_PI, normalizedVelocity);
   */
}

/**
 * This should be overridden by the shot subclasses that extend 
 * ExplosiveShot, since an ExplosiveShot itself will never be drawn.
 */
void ExplosiveShot::draw() {
   /*
   Vector3D zVector(0, 0, -1);
   Vector3D axis = zVector.cross(*velocity);
   glPushMatrix();
      glDisable(GL_LIGHTING);
      glColor3f(1, 0, 0);
      setMaterial(ShotMaterial);
      position->glTranslate();
      glRotated(zVector.getAngleInDegrees(*velocity), 
         axis.x, axis.y, axis.z);
      gluCylinder(quadric,0.08f,0.0f,0.8f,6,6);
      glEnable(GL_LIGHTING);

   glPopMatrix();
   */
}

/**
 * update should do all generic shot updating, but then should be
 * overridden, just like draw, by all classes that extend ExplosiveShot.
 */
void ExplosiveShot::update(double timeDiff) {
   Shot::update(timeDiff);
   //const int particlesPerSecond = 4;
   
   //Vector3D normalizedVelocity(*velocity);
   //normalizedVelocity.normalize();
   //const double particleDirectionPeriod = 0.3;
   //const double particleDirectionRotationSpeed = 2 * M_PI / particleDirectionPeriod;
   //particleDirection.rotate(timeDiff * particleDirectionRotationSpeed, normalizedVelocity);
   //particleDirection.normalize();
   
   /*
   for (int i = 0; i <= timeDiff * particlesPerSecond; ++i) {
      particleNum = (particleNum + 1) % particleCycle;
      // This is the random way...
      particleDirection.randomMagnitude();
      particleDirection.setLength(0.1);
      
      BlasterShotParticle::AddRainbow(new Point3D(*position), 
       new Vector3D(particleDirection), particleNum, particleCycle, gameState);
      // Reflect and Duplicate the above for a double helix.
   }
   */
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

   isExploded = true;

   // Record the frame when the bomb exploded.
   frameExploded = doubleTime();
   
   shouldConstrain = false;

   // Generically, the explode() function should:
   // Set shouldConstrain to false, so the ExplosiveShot doesn't collide with walls any more.
}

void ExplosiveShot::handleCollision(Drawable* other) {
   // Apply damage to all targets hit.
   // Apply a force to all targets hit.
    
   /*
   const int particlesToEmit = 10;
   
   static Vector3D particleVariation;
   static Vector3D positionDifference;
   const double particleSpeed = 15;
   // This should probably be moved to the Asteroid's code.
   */
    

   Asteroid3D* asteroid;
   // If we hit an asteroid.
   if ((asteroid = dynamic_cast<Asteroid3D*>(other)) != NULL) {
      SoundEffect::playSoundEffect("BlasterHit.wav");
      /*
      // Make some particles!
      positionDifference.updateMagnitude(*asteroid->position, *position);
      positionDifference.setLength(particleSpeed);
      for (int i = 0; i <= particlesToEmit; ++i) {
         particleVariation.randomMagnitude();
         particleVariation.setLength(particleSpeed);
         particleVariation.addUpdate(positionDifference);
         BlasterImpactParticle::Add(new Point3D(*position), 
          new Vector3D(particleVariation), gameState);
      }
      */
   }
   Shot::handleCollision(other);
}

void ExplosiveShot::hitWall(BoundingWall* wall) {
   //particleDirection.reflect(wall->normal);
}
