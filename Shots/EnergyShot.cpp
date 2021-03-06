/**
 * EnergyShot.cpp
 * Blaster Shots!
 * Pew pew pew!
 */

#include "Shots/EnergyShot.h"
#include "Particles/BlasterShotParticle.h"
#include "Particles/BlasterImpactParticle.h"
#include "Utility/SoundEffect.h"
#include "Network/gamestate.pb.h"

#include <algorithm>

#ifdef WIN32
#include "Utility/WindowsMathLib.h"
#endif

const int particleCycle = 100;
static float spinCycle = 0;

const double baseRadius = 0.2;
const double baseDamagePerSecond = 1;

EnergyShot::EnergyShot(Point3D& posIn, Vector3D dirIn, int _weaponIndex,
 AsteroidShip* const ownerIn, const GameState* _gameState) : 
 Shot(posIn, dirIn, _weaponIndex, ownerIn, _gameState) {
   weapon = static_cast<Energy*>(owner->weapons[weaponIndex]);
   type = TYPE_ENERGYSHOT;
   persist = false;
   radius = baseRadius;
   minX = minY = minZ = -radius;
   maxX = maxY = maxZ = radius;
   updateBoundingBox();
   lifetime = 20;
   damagePerSecond = baseDamagePerSecond;

   static int currentStartingParticleCycle = 0;
   particleNum = currentStartingParticleCycle;
   currentStartingParticleCycle = (currentStartingParticleCycle + 7) % particleCycle;
   particleDirection = velocity->getNormalVector();
   
   Vector3D normalizedVelocity(*velocity);
   normalizedVelocity.normalize();

   particleDirection.rotate(randdouble() * 2 * M_PI, normalizedVelocity);

   chargeTime = 0;
   damage = 0; // Base damage. But we really use damagePerSecond.
   collisionType = collisionSphere = new CollisionSphere(radius, *position);
}

void EnergyShot::draw() {
   double random, posOrNeg;
   double x, y, z;
   Vector3D zVector(0, 0, -1);
   Vector3D axis = zVector.cross(*velocity);
   glPushMatrix();
      glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
      glDisable(GL_LIGHTING);

      //setMaterial(WhiteSolid);
      position->glTranslate();
      glRotated(zVector.getAngleInDegrees(*velocity), 
         axis.x, axis.y, axis.z);
      
      glScaled(radius, radius, radius);
      
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
void EnergyShot::update(double timeDiff) {
   Shot::update(timeDiff);
   
   if (shouldRemove) {
      if (weapon->chargingShotid == id)
         weapon->resetChargingShot();
   }
}

void EnergyShot::hitWall(BoundingWall* wall) {
   shouldRemove = true;
   if (weapon->chargingShotid == id)
      weapon->resetChargingShot();
}

void EnergyShot::updateChargeTime(double newChargeTime) {
   // Charge time limit of 5 seconds.
   chargeTime = newChargeTime;
   float cappedSquaredChargeTime = std::min(chargeTime * chargeTime, 5.0f);
   radius = baseRadius + baseRadius * cappedSquaredChargeTime;

   collisionSphere->updateRadius(radius);

   minX = minY = minZ = -radius;
   maxX = maxY = maxZ = radius;
   updateBoundingBox();

   damagePerSecond = baseDamagePerSecond + baseDamagePerSecond * cappedSquaredChargeTime;
}

bool EnergyShot::saveDiff(const ast::Entity& old, ast::Entity* ent) {
   bool changed = Shot::saveDiff(old, ent);
   if (chargeTime != old.chargetime()) {
      ent->set_chargetime(chargeTime);
      changed = true;
   }
   
   if (damagePerSecond != old.damagepersecond()) {
      ent->set_damagepersecond(damagePerSecond);
      changed = true;
   }
   
   if (radius != old.radius()) {
      ent->set_radius(radius);
      changed = true;
   }
   return changed;
}

void EnergyShot::save(ast::Entity* ent) {
   Shot::save(ent);
   ent->set_chargetime(chargeTime);
   ent->set_damagepersecond(damagePerSecond);
   ent->set_radius(radius);
}

void EnergyShot::load(const ast::Entity& ent) {
   Shot::load(ent);

   if (ent.has_chargetime())
      chargeTime = ent.chargetime();
   
   if (ent.has_damagepersecond())
      damagePerSecond = ent.damagepersecond();

   if (ent.has_radius())
      radius = ent.radius();
}

