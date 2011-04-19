/**
 * Sterling Hirsh
 * Asteroid3D.cpp
 * A randomly generated asteroid object.
 */

#include "Utility/GlobalUtility.h"
#include "Utility/Texture.h"
#include "Items/Asteroid3D.h"
#include "Items/AsteroidShip.h"
#include "Shots/Shot.h"
#include "Shots/ProjectileShot.h"
#include "Shots/BeamShot.h"
#include "Shots/TractorBeamShot.h"
#include "Shots/ElectricityShot.h"
#include "Shots/LawnMowerShot.h"
#include "Shots/RamShot.h"
#include "Shots/AntiInertiaShot.h"
#include "Shots/EnergyShot.h"
#include "Shots/TimedBombShot.h"

#include <time.h>
#include "Utility/SoundEffect.h"
#include <algorithm>
#define _USE_MATH_DEFINES
#include <math.h>
#include "Particles/ElectricityImpactParticle.h"

using namespace std;

static double timediff = (double) clock();
static double spinTime = 0;
double randomR = 0;
double randomG = 0;
double randomB = 0;

Asteroid3D::Asteroid3D(double r, double worldSizeIn, const GameState* _gameState, bool isFirst) :
   Object3D(_gameState),
   worldSize(worldSizeIn) {
      shouldDrawInMinimap = true;

      initH = (int)(r * r * 1.0 / 5.0);
      if (initH < 2.0) {
         initH = 2.0;
      }

      mesh.drawAnim = isFirst;

      isExploding = false;

      health = initH;
      newRandomPosition();
      InitAsteroid(r, worldSizeIn);
      hitAsteroid = false;
   }

Asteroid3D::~Asteroid3D() {

}

/**
 * This gives the asteroid a new random position. Useful when
 * initializing them, and not much else.
 */
void Asteroid3D::newRandomPosition() {
   position->x = randdouble() * worldSize - worldSize / 2;
   position->y = randdouble() * worldSize - worldSize / 2;
   position->z = randdouble() * worldSize - worldSize / 2;
   updateBoundingBox();
}

void Asteroid3D::InitAsteroid(double r, double worldSizeIn) {
   angle = 0;
   //radius = r;
   worldSize = worldSizeIn;

   scalex = scaley = scalez = 1;

   rotationSpeed = randdouble() * 100; // Degrees per sec.
   axis = new Vector3D(0, 1, 0);
   axis->randomMagnitude();

   velocity = new Vector3D(0, 0, 0);
   velocity->randomMagnitude();
   velocity->setLength(randdouble() * 3); // Units per sec.

   getBrightColor(randdouble(), cyan, magenta, yellow);

   Ring last;
   double a = randdouble() * 0.5 + 0.75;
   double b = randdouble() * 0.5 + 0.75;

   int npts = (int)((r + 3.0) / 1.5) * 2;

   if (npts == 4) {
      npts += 2;
   }

   for (int j = npts / 2; j >= 0; j--) {
      double tmpangle = (M_PI * 2 / (double)npts * (double)j);
      double tmpRad = r * sin(tmpangle);
      double tmpH = r * cos(tmpangle);
      int tmpPts = (int) (npts * (tmpRad / r));
      if (tmpPts < 0) {
         tmpPts *= -1;
      } else if (tmpPts == 0) {
         tmpPts = 1;
      }

      Ring thisRing;
      thisRing = Ring(tmpPts, a * tmpRad, b * tmpH, &mesh);
      _rList.push_back(thisRing);

      last = thisRing;
   }

   minX = _rList[0].minX();
   maxX = _rList[0].maxX();
   minY = _rList[0].minY();
   maxY = _rList[0].maxY();
   minZ = _rList[0].minZ();
   maxZ = _rList[0].maxZ();

   for (unsigned i = 0; i < _rList.size(); i++) {
      if (_rList[i].minX() < minX) {
         minX = _rList[i].minX();
      }
      if (_rList[i].maxX() > maxX) {
         maxX = _rList[i].maxX();
      }
      if (_rList[i].minY() < minY) {
         minY = _rList[i].minY();
      }
      if (_rList[i].maxY() > maxY) {
         maxY = _rList[i].maxY();
      }
      if (_rList[i].minZ() < minZ) {
         minZ = _rList[i].minZ();
      }
      if (_rList[i].maxZ() > maxZ) {
         maxZ = _rList[i].maxZ();
      }
   }

   for (unsigned i = 1; i <= _rList.size() / 2; i++) {
      makeStrip(_rList[i - 1], _rList[i]);
   }

   for (size_t i = _rList.size() / 2; i < _rList.size() - 1; i++) {
      makeStrip(_rList[i], _rList[i + 1]);
   }

   sizeX = maxX - minX;
   sizeY = maxY - minY;
   sizeZ = maxZ - minZ;

   radius = -minX;
   if (maxX > radius) {
      radius = maxX;
   }
   if (-minY > radius) {
      radius = -minY;
   }
   if (maxY > radius) {
      radius = maxY;
   }
   if (-minZ > radius) {
      radius = -minZ;
   }
   if (maxZ > radius) {
      radius = maxZ;
   }

   // Tweak the radius to (hopefully) encompass all points.
   cullRadius = radius + r * 0.10;

   collisionRadius = radius;
   updateBoundingBox();
}

void Asteroid3D::drawGlow() {
   glDisable(GL_LIGHTING);
   // Call the display list if it has one.
   Object3D::draw();
   glEnable(GL_COLOR_MATERIAL);
   glDisable(GL_CULL_FACE);
   glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
   glPushMatrix();
   glTranslated(position->x, position->y, position->z);
   glRotated(angle, axis->x, axis->y, axis->z);
   glScaled(scalex, scaley, scalez);
   glDepthFunc(GL_LEQUAL);

   glColor3f(0.0, 0.0, 0.0);
   glPolygonOffset(1.0f, 1.0f);
   glEnable(GL_POLYGON_OFFSET_FILL);
   glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
   mesh.draw(false);
   //mesh.drawTextured(false, 0);
   glDisable(GL_POLYGON_OFFSET_FILL);
   glEnable(GL_CULL_FACE);

   /*
   double step = (initH - health + 1);
   double stepR = .004 / initH * step;
   double stepG = .388 / initH * step;
   double stepB = 1.0 / initH * step;
   glColor3f(0.996 + stepR, 0.612 + stepG, 0.0 + stepB);
   */
   
   double step = (initH - health) / initH;
   double stepR = cyan * step;
   double stepG = magenta * step;
   double stepB = yellow * step;
   //glColor3d(1 - stepR / 2.0, 1 - stepG / 2.0, 1 - stepB / 2.0);
   mesh.setLineColor((float)(1 - stepR / 2.0),
         (float)(1 - stepG / 2.0),
         (float)(1 - stepB / 2.0));

   double shipDist = position->distanceFrom(*gameState->ship->position);
   double lineW = (worldSize / shipDist * ASTEROID3D_LINE_W + 1.0) / 2;
   //printf("distance: %f, line width: %f\n", shipDist, lineW);
   //glLineWidth(ASTEROID3D_LINE_W);
   glLineWidth((GLfloat) lineW);
   glEnable(GL_POLYGON_OFFSET_LINE);
   glPolygonOffset(-1.0f, -1.0f);
   glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
   mesh.drawLines(false);
   glDisable(GL_POLYGON_OFFSET_LINE);
   glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
   glLineWidth(1);

   glDisable(GL_COLOR_MATERIAL);
   glEnable(GL_LIGHTING);
   //glDepthFunc(GL_LEQUAL);
   glPopMatrix();
}

void Asteroid3D::draw() {
   glColor3f(1, 1, 1);
   glDisable(GL_CULL_FACE);
   setMaterial(Rock);
   // Call the display list if it has one.
   Object3D::draw();
   glEnable(GL_COLOR_MATERIAL);
   glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
   glPushMatrix();
   glTranslated(position->x, position->y, position->z);
   glRotated(angle, axis->x, axis->y, axis->z);
   glScaled(scalex, scaley, scalez);

   if (isTargeted()) {
      glColor3f(1.0f, 0.0f, 0.0f);
   } else {
      //glColor3f(0.0, 0.0, 0.0);
      glColor3f(1.0, 1.0, 1.0);
   }

   glPolygonOffset(1.0f, 1.0f);
   glEnable(GL_POLYGON_OFFSET_FILL);
   glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
   //mesh.draw(false);
   if (gameState->godMode) {
      mesh.drawTextured(false, Texture::getTexture("ZoeRedEyes"));
   } else {
      mesh.drawTextured(false, Texture::getTexture("AsteroidSurface"));
   }
   glDisable(GL_POLYGON_OFFSET_FILL);

   //if (health == 1) {
   //glColor3f(0.996, 0.312, 0.1);
   //} else if (health > 1) {
   double step = (initH - health) / initH;
   double stepR = cyan * step;
   double stepG = magenta * step;
   double stepB = yellow * step;
   //glColor3d(1 - stepR, 1 - stepG, 1 - stepB);
   mesh.setLineColor((float)(1 - stepR),
         (float)(1 - stepG),
         (float)(1 - stepB));
   //}
   //glColor3f(0.996, 0.612, 0.0);

   double shipDist = position->distanceFrom(*gameState->ship->position);
   double lineW = worldSize / shipDist * ASTEROID3D_LINE_W + 1.0;
   //glLineWidth(ASTEROID3D_LINE_W);
   glLineWidth((GLfloat)lineW);
   glEnable(GL_POLYGON_OFFSET_LINE);
   glPolygonOffset(-1.0f, -1.0f);
   glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
   mesh.drawLines(false);
   glDisable(GL_POLYGON_OFFSET_LINE);
   glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
   glLineWidth(1);

   glDisable(GL_COLOR_MATERIAL);
   if (hitAsteroid) {
      if (doubleTime() - timeHit > 5) {
         hitAsteroid = false;
         velocity = new Vector3D(*newVelocity);
         acceleration = new Vector3D(*newAcceleration);         
      }
      
      drawEnergyEffect();
   }
   glPopMatrix();
   setTargeted(false);
   glEnable(GL_CULL_FACE);
}

void Asteroid3D::makeStrip(Ring r1, Ring r2) {
   double count = 0.0;
   int last = 0;
   Ring t1, t2;
   if (r1.size() < r2.size()) {
      t1 = r1;
      t2 = r2;
   } else {
      t1 = r2;
      t2 = r1;
   }
   double step = (double)t1.size() / (double)t2.size();
   //for (int i = 0; i < t2.size() - (t2.size() % 2); i++) {
   for (int i = 0; i < t2.size(); i++) {
      int p1, p2, p3, p4;
      p1 = t2._nList[i];
      p2 = t1._nList[(int)count % t1.size()];
      p3 = t2._nList[(i + 1) % t2.size()];
      if (r1.size() < r2.size()) {
         mesh.addFace(p1, p3, p2);
      } else {
         mesh.addFace(p1, p2, p3);
      }

      p4 = t1._nList[((int)count - 1 + t1.size()) % t1.size()];
      if (last != (int)count || i == 0) {
      //if (last != (int)count) {
         if (r1.size() < r2.size()) {
            mesh.addFace(p1, p2, p4);
         } else {
            mesh.addFace(p1, p4, p2);
         }
      }

      last = (int)count;
      count += step;
   }
}

void Asteroid3D::drawEnergyEffect() {
   double random, posOrNeg;
   double x, y, z;
   double div = 2;
   //double randomR = 0, randomG = 0, randomB = 0;
   
   if (randomR > 1) randomR = .2;
   if (randomG > 1) randomG = .2;
   if (randomB > 1) randomB = .2;
   
   random = (double) (rand() % 4);
   random /= 10;
   randomR += (double) (rand() % 50) / 1000;
   //randomR /= 10;
   randomG += (double) (rand() % 30) / 1000;
   //randomG /= 10;
   randomB += (double) (rand() % 20) / 200;
   //randomB /= 10;
   glPushMatrix();
   glDisable(GL_LIGHTING);

   glColor4d(randomR, randomG, 1, 1);
   glLineWidth(5);

   glRotated(-angle, axis->x, axis->y, axis->z);
   for(double rad = -radius + radius/2; rad < radius; rad += (radius) / 2) {
      glBegin(GL_LINE_LOOP);
      for(double j = 0; j < 2; j += .1) {
            random = (double) (rand() % 4);
            posOrNeg = (double) (rand() % 2);
            posOrNeg = 1 - (posOrNeg * 2);
            random /= 10 * posOrNeg;
            z = ((radius - fabs(rad) + 1 + fabs(rad/div)) * cos(j * M_PI) + random);
            random = (double) (rand() % 4);
            posOrNeg = (double) (rand() % 2);
            posOrNeg = 1 - (posOrNeg * 2);
            random /= 10 * posOrNeg;
            y = ((radius - fabs(rad) + 1 + fabs(rad/div)) * sin(j * M_PI) + random);
            x = rad + random;
            glVertex3d(x, y, z);
      }
      glEnd();
   }
   
   for(double rad = -radius + radius/2; rad < radius; rad += (radius) / 2) {
      glBegin(GL_LINE_LOOP);
      for(double j = 0; j < 2; j += .1) {
            random = (double) (rand() % 4);
            posOrNeg = (double) (rand() % 2);
            posOrNeg = 1 - (posOrNeg * 2);
            random /= 10 * posOrNeg;
            x = ((radius - fabs(rad) + 1 + fabs(rad/div)) * cos(j * M_PI) + random);
            random = (double) (rand() % 4);
            posOrNeg = (double) (rand() % 2);
            posOrNeg = 1 - (posOrNeg * 2);
            random /= 10 * posOrNeg;
            y = ((radius - fabs(rad) + 1 + fabs(rad/div)) * sin(j * M_PI) + random);
            z = rad + random;
            glVertex3d(x, y, z);
      }
      glEnd();
   }
   //for(double rad = -radius; rad <= radius + 1; rad += (radius) / 2) {
   for(double rad = -radius + radius/2; rad < radius; rad += (radius) / 2) {
      glBegin(GL_LINE_LOOP);
      for(double j = 0; j < 2; j += .1) {
            random = (double) (rand() % 4);
            posOrNeg = (double) (rand() % 2);
            posOrNeg = 1 - (posOrNeg * 2);
            random /= 10 * posOrNeg;
            x = ((radius - fabs(rad) + 1 + fabs(rad/div)) * cos(j * M_PI) + random);
            random = (double) (rand() % 4);
            posOrNeg = (double) (rand() % 2);
            posOrNeg = 1 - (posOrNeg * 2);
            random /= 10 * posOrNeg;
            z = ((radius - fabs(rad) + 1 + fabs(rad/div)) * sin(j * M_PI) + random);
            y = rad + random;
            glVertex3d(x, y, z);
      }
      glEnd();
   }
   
   glLineWidth(1);

   glEnable(GL_LIGHTING);
   glPopMatrix();
}

void Asteroid3D::update(double timeDiff) {
   if (isExploding) {
      timeSinceExplode += timeDiff;
      for (int i = 0; i < mesh.faces.size(); i++) {
         mesh.faces[i].offsetBy(timeSinceExplode);
         // Offset normals.
      }
      if (timeSinceExplode >= 1.0) {
         shouldRemove = true;
      }
   }
   Object3D::update(timeDiff);
   mesh.tick(timeDiff);
   if (velocity->getComparisonLength() > 40 * 40) {
      // Set a speed limit of 40 u/s.
      velocity->setLength(40);
   }
   angle += rotationSpeed * timeDiff;
   if(hitAsteroid) health -= timeDiff;
}

void Asteroid3D::handleCollision(Drawable* other) {
   Asteroid3D* otherAsteroid;
   AsteroidShip* ship;
   Shot* shot;
   BeamShot* beamShot;
   LawnMowerShot* lawnMowerShot;
   ElectricityShot* elecShot;
   TractorBeamShot* TBshot; // Not tuberculosis
   TimedBombShot* TBshot2;
   if ((otherAsteroid = dynamic_cast<Asteroid3D*>(other)) != NULL) {
      if (isExploding || otherAsteroid->isExploding) { return; }
      double d = (*(otherAsteroid->position)).distanceFrom(*position);
      double combinedRad = otherAsteroid->radius + radius;
      double maxR = max(radius, otherAsteroid->radius);
      Vector3D* push = new Vector3D(*otherAsteroid->position, *position);

      if (d <=  maxR) {
         if (d == 0) {
            //printf("stroid is stuck\n");
            push->randomMagnitude();
         }
         push->setLength(0.1); // Units per sec per sec.
         addAcceleration(push);
      }

      double speed = velocity->getLength();

      Vector3D reflectionAxis(*otherAsteroid->position, *position);
      reflectionAxis.normalize();

      double m1 = radius; // This Asteroid's Mass.
      double m2 = otherAsteroid->radius; // Other Asteroid's Mass.

      Vector3D* newVelocity = new Vector3D(*velocity);
      newVelocity->scalarMultiplyUpdate(m1 - m2);
      newVelocity->addUpdate(otherAsteroid->velocity->scalarMultiply(2 * m2));
      newVelocity->scalarMultiplyUpdate(1 / (m1 + m2 + 0.2)); // Added the 0.2 to make stuff lose some energy.
      
      // We want to only add acceleration away from the other item.
      if (newVelocity->dot(*push) < 0) {
         newVelocity->scalarMultiplyUpdate(-1);
      }
      
      addInstantAcceleration(newVelocity);

      Vector3D* reverseVelocity = new Vector3D(*velocity);
      reverseVelocity->scalarMultiplyUpdate(-1);
      addInstantAcceleration(reverseVelocity);

   } else if ((ship = dynamic_cast<AsteroidShip*>(other)) != NULL) {
      if (isExploding) { return; }
      //shouldRemove = true;
      timeSinceExplode = 0.0;
      isExploding = true;
      if (radius > 4) {
         int dimension = rand() % 3;
         custodian->add(makeChild(0, dimension));
         custodian->add(makeChild(1, dimension));
      }
   } else if ((shot = dynamic_cast<Shot*>(other)) != NULL) {
      if (isExploding) { return; }
      if (health > 0) {
         if ((beamShot = dynamic_cast<BeamShot*>(other)) != NULL) {
            if (((!beamShot->hitYet) || this == beamShot->hitItem) && (curFrame - 1) <= beamShot->firstFrame) {
               health = 0;
               velocity->addUpdate(shot->velocity->scalarMultiply(10));
            }
         } else if ((TBshot = dynamic_cast<TractorBeamShot*>(other)) != NULL) {
            velocity->updateMagnitude(TBshot->owner->velocity);
            
            // Do nothing.
         } else if ((lawnMowerShot = dynamic_cast<LawnMowerShot*>(other)) != NULL) {
            health -= fabs((health / 8) + 0.05); // Fabsulous!!
            velocity->updateMagnitude(*lawnMowerShot->position, *position);
         } else if ((elecShot = dynamic_cast<ElectricityShot*>(other)) != NULL) {
            const int numElecParticles = 1;
            
            double hitDistance = 0;
            // TODO: Refactor this so we do all collision detection elsewhere.
            Point3D* closestPoint = sphereCollideWithRay(*shot->position, *shot->velocity, &hitDistance);

            // Sometimes, we decide that it's not a real hit. No big deal.
            if (closestPoint != NULL) {
               elecShot->length = hitDistance;
               elecShot->velocity->setLength(hitDistance);
               health -= elecShot->strength * .05;
               Vector3D centerToImpactPoint(*position, *closestPoint);
               centerToImpactPoint.setLength(5);

               for (int i = 0; i < numElecParticles; ++i) {
                  Point3D* particleStartPoint = new Point3D(*closestPoint);
                  Vector3D* particleDirection = new Vector3D();
                  particleDirection->randomMagnitude();
                  particleDirection->setLength(3);
                  particleDirection->addUpdate(centerToImpactPoint);
                  ElectricityImpactParticle::Add(particleStartPoint, particleDirection, gameState);
               }

               delete closestPoint;
            }
         } else if (dynamic_cast<RamShot*>(other) != NULL) {
            health = 0;
         } else if (dynamic_cast<AntiInertiaShot*>(other) != NULL) {
            Vector3D* newVelocity = new Vector3D(*velocity);
            newVelocity->scalarMultiplyUpdate(-0.1);
            addInstantAcceleration(newVelocity);
            if (rotationSpeed >= 0.5) {
               rotationSpeed -= 0.5;
            } else {
               rotationSpeed = 0;
            } 
         } else if (dynamic_cast<EnergyShot*>(other) != NULL) {
            if (!hitAsteroid) {
               hitAsteroid = true;
               timeHit = doubleTime();
               newVelocity = new Vector3D(*velocity);
               newAcceleration = new Vector3D(*acceleration);
            }
            
            //newVelocity->setLength(-1.0);
            //addInstantAcceleration(newVelocity);
            velocity = new Vector3D(0, 0, 0);
            acceleration = new Vector3D(0, 0, 0);
            /*if (rotationSpeed >= 0.5) {
               rotationSpeed = 0;
            } else {
               rotationSpeed = 0;
            }*/
         } else if (dynamic_cast<ProjectileShot*>(other) != NULL) {
            if (gameState->godMode) {
               health = 0;
            } else {
               health--;
            }
            double speed = 10 / sqrt(radius);
            Vector3D* newAcceleration = new Vector3D(*(shot->position), *position);
            newAcceleration->setLength(speed);
            addInstantAcceleration(newAcceleration);
         } else if ((TBshot2 = dynamic_cast<TimedBombShot*>(other)) != NULL) {
            if (TBshot2->isExploded)
               health = 0;
            // remove health from this asteroid based on its distance to the bomb.
            // change this bomb's motion vector based on its distance to the bomb.
         }
      }
      if (health <= 0) {
         SoundEffect::playSoundEffect("Explosion1.wav");
         //shouldRemove = true;
         timeSinceExplode = 0.0;
         isExploding = true;
         const int explosionFactor = 3;
         if (radius > 2) {
            shot->owner->score += (int) radius * 10;
            int dimension = rand() % 3;
            custodian->add(makeChild(0, dimension));
            custodian->add(makeChild(1, dimension));
         }
         dropRandomItem();
      }
   }
}

Shard* Asteroid3D::makeShard(int num) {
   Shard* shard;
   shard = new Shard(0.5, worldSize, gameState);
   shard->velocity->updateMagnitude(0.0, 0.0, 0.0);
   shard->position->clone(position);
   shard->position->x += num == 0 ? radius/2 : -radius/2;
   return shard;
}

/**
 * Make a smaller asteroid as the result of an explosion or something.
 * num is the number of asteroid that is produced.
 * This helps with making sure that two asteroids don't intersect.
 */
Asteroid3D* Asteroid3D::makeChild(int num, int dimension) {
   Asteroid3D* asteroid;
   asteroid = new Asteroid3D(radius/2, worldSize, gameState);
   asteroid->velocity->scalarMultiplyUpdate(4);
   asteroid->velocity->addUpdate(*velocity);
   asteroid->position->clone(position);
   if (dimension % 3 == 0)
      asteroid->position->x += num == 0 ? radius/2 : -radius/2;
   else if (dimension % 3 == 1)
      asteroid->position->y += num == 0 ? radius/2 : -radius/2;
   else
      asteroid->position->z += num == 0 ? radius/2 : -radius/2;

   return asteroid;
}

double Asteroid3D::randRadius(double r) {
   return (3 * (r / 4)) + ((r / 4) * randdouble());
}

/**
 * Subclasses can extend this, but this draws a sphere on the minimap.
 */
void Asteroid3D::drawInMinimap() {
   glPushMatrix();
   position->glTranslate();
   gluSphere(quadric, 0.5 * collisionRadius, 5, 5);
   glPopMatrix();
}

void Asteroid3D::debug() {
   printf("Asteroid3D::debug(): \n");
   minPosition->print();
   maxPosition->print();
   velocity->print();
   printf("--\n");
}

void Asteroid3D::dropRandomItem() {
   double whichItem = randdouble();
   if (whichItem < 0.4) {
      // Do nothing!
   } else if (whichItem < 0.8) { 
      custodian->add(makeShard(0));
      SoundEffect::playSoundEffect("CrystalRelease");
      //printf("YOU JUST SHARDED\n");
   } else {
      // Don't play the sound effect twice on top of itself.
      SoundEffect::playSoundEffect("DoubleCrystalRelease");
      custodian->add(makeShard(0));
      custodian->add(makeShard(0));
   }
}
