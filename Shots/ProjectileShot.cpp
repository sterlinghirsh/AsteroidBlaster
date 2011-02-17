/**
 * ProjectileShot.cpp
 * Blaster Shots!
 * Pew pew pew!
 */

#include "Shots/ProjectileShot.h"
#include "Graphics/BlasterShotParticle.h"

double ProjectileShot::frequency = 10;

ProjectileShot::ProjectileShot(Point3D& posIn, Vector3D dirIn,
 AsteroidShip* const ownerIn ) : Shot(posIn, dirIn, ownerIn) {
   persist = false;
   minX = minY = minZ = -0.1;
   maxX = maxY = maxZ = 0.1;
   updateBoundingBox();
}

void ProjectileShot::draw() {
   Vector3D zVector(0, 0, -1);
   Vector3D axis = zVector.cross(*velocity);
   glPushMatrix();
   glDisable(GL_LIGHTING);
   glColor3f(1, 0, 0);
   setMaterial(ShotMaterial);
   position->glTranslate();
   glRotatef(zVector.getAngleInDegrees(*velocity), 
      axis.xMag, axis.yMag, axis.zMag);
   //glutSolidSphere(0.05, 30, 10);
   //drawCylinder(0.02, 1);
   //glutSolidCone(0.02, 0.2, 8, 8);
   //gluCylinder(quadric,0.04f,0.0f,0.2f,8,8);
   gluCylinder(quadric,0.08f,0.0f,0.8f,8,8);
   glEnable(GL_LIGHTING);

   glPopMatrix();
}

// Spray particles.
void ProjectileShot::update(double timeDiff) {
   Shot::update(timeDiff);
   const int particlesPerSecond = 10;
   
   static Vector3D particleVariation;
   
   for (int i = 0; i <= timeDiff * particlesPerSecond; ++i) {
      particleVariation.randomMagnitude();
      particleVariation.setLength(0.1);
      BlasterShotParticle::Add(new Point3D(*position), 
       new Vector3D(particleVariation));
   }
}

void ProjectileShot::handleCollision(Object3D* other) {
   const int particlesToEmit = 20;
   
   static Vector3D particleVariation;
   static Vector3D positionDifference;
   const double particleSpeed = 5;

   Asteroid3D* asteroid;
   // If we hit an asteroid.
   if ((asteroid = dynamic_cast<Asteroid3D*>(other)) != NULL) {
      // Make some particles!
      positionDifference.updateMagnitude(*asteroid->position, *position);
      positionDifference.setLength(particleSpeed);
      for (int i = 0; i <= particlesToEmit; ++i) {
         particleVariation.randomMagnitude();
         particleVariation.setLength(particleSpeed);
         particleVariation.addUpdate(positionDifference);
         BlasterShotParticle::Add(new Point3D(*position), 
          new Vector3D(particleVariation));
      }
   }
   Shot::handleCollision(other);
}
