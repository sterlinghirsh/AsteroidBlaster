/**
 * Sterling Hirsh
 * Asteroid3D.cpp
 * A randomly generated asteroid object.
 */

#include "Utility/GlobalUtility.h"
#include "Graphics/Sprite.h"
#include "Items/Asteroid3D.h"
#include "Items/AsteroidShip.h"
#include "Shots/Shot.h"
#include "Shots/ProjectileShot.h"
#include "Shots/BeamShot.h"
#include "Shots/TractorBeamShot.h"
#include "Shots/ElectricityShot.h"
#include "Utility/SoundEffect.h"
#include <algorithm>
#define _USE_MATH_DEFINES
#include <math.h>

using namespace std;

Asteroid3D::Asteroid3D(double r, double worldSizeIn) :
   Object3D(0, 0, 0, 0) {
      shouldDrawInMinimap = true;
      worldSize = worldSizeIn;
      //health = initH;
      initH = (int)(r * r * 1.0 / 5.0);
      if (initH < 2.0) {
         initH = 2.0;
      }
      health = initH;
      newRandomPosition();
      InitAsteroid(r, worldSizeIn);
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
      int tmpPts = npts * (tmpRad / r);
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

   for (unsigned i = _rList.size() / 2; i < _rList.size() - 1; i++) {
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
   glColor3f(1, 1, 1);
   glDisable(GL_LIGHTING);
   // Call the display list if it has one.
   Object3D::draw();
   glEnable(GL_COLOR_MATERIAL);
   glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
   glPushMatrix();
   glTranslatef(position->x, position->y, position->z);
   glRotatef(angle, axis->xMag, axis->yMag, axis->zMag);
   glScalef(scalex, scaley, scalez);

   glColor3f(0.0, 0.0, 0.0);
   glPolygonOffset(1.0f, 1.0f);
   glEnable(GL_POLYGON_OFFSET_FILL);
   glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
   mesh.draw(false);
   glDisable(GL_POLYGON_OFFSET_FILL);

   double step = (initH - health + 1);
   double stepR = .004 / initH * step;
   double stepG = .388 / initH * step;
   double stepB = 1.0 / initH * step;
   glColor3f(0.996 + stepR, 0.612 + stepG, 0.0 + stepB);

   glLineWidth(ASTEROID3D_LINE_W);
   glEnable(GL_POLYGON_OFFSET_LINE);
   glPolygonOffset(-1.0f, -1.0f);
   glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
   mesh.drawLines(false);
   glDisable(GL_POLYGON_OFFSET_LINE);
   glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
   glLineWidth(1);

   glDisable(GL_COLOR_MATERIAL);
   glEnable(GL_LIGHTING);
   glPopMatrix();
}

void Asteroid3D::draw() {
   glColor3f(1, 1, 1);
   setMaterial(Rock);
   // Call the display list if it has one.
   Object3D::draw();
   glEnable(GL_COLOR_MATERIAL);
   glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
   glPushMatrix();
   glTranslatef(position->x, position->y, position->z);
   glRotatef(angle, axis->xMag, axis->yMag, axis->zMag);
   glScalef(scalex, scaley, scalez);

   if (isTargeted()) {
      glColor3f(1.0f, 0.0f, 0.0f);
   } else {
      glColor3f(0.0, 0.0, 0.0);
   }

   glPolygonOffset(1.0f, 1.0f);
   glEnable(GL_POLYGON_OFFSET_FILL);
   glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
   mesh.draw(false);
   glDisable(GL_POLYGON_OFFSET_FILL);

   //if (health == 1) {
   //glColor3f(0.996, 0.312, 0.1);
   //} else if (health > 1) {
   double step = (initH - health + 1);
   double stepR = .004 / initH * step;
   double stepG = .388 / initH * step;
   double stepB = 1.0 / initH * step;
   glColor3f(0.996 + stepR, 0.612 + stepG, 0.0 + stepB);
   //}
   //glColor3f(0.996, 0.612, 0.0);

   glLineWidth(ASTEROID3D_LINE_W);
   glEnable(GL_POLYGON_OFFSET_LINE);
   glPolygonOffset(-1.0f, -1.0f);
   glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
   mesh.drawLines(false);
   glDisable(GL_POLYGON_OFFSET_LINE);
   glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
   glLineWidth(1);

   glDisable(GL_COLOR_MATERIAL);
   glPopMatrix();
   setTargeted(false);
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

void Asteroid3D::update(double timeDiff) {
   Object3D::update(timeDiff);
   angle += rotationSpeed * timeDiff;
}

void Asteroid3D::handleCollision(Object3D* other) {
   Asteroid3D* otherAsteroid;
   AsteroidShip* ship;
   Shot* shot;
   if ((otherAsteroid = dynamic_cast<Asteroid3D*>(other)) != NULL) {
      double d = (*(otherAsteroid->position)).distanceFrom(*position);
      double combinedRad = otherAsteroid->radius + radius;
      double maxR = max(radius, otherAsteroid->radius);
      //printf("distance between stroids: %f (%f)\n", d, combinedRad);
      if (d <=  maxR) {
         //printf("stroid is stuck\n");
         shouldRemove = true;
         if (radius > 2) {
            int dimension = rand() % 3;
            custodian->add(makeChild(0, dimension));
            custodian->add(makeChild(1, dimension));
         }
      }
      double speed = velocity->getLength();

      velocity->updateMagnitude(*(otherAsteroid->position), *position);
      velocity->setLength(speed);
   } else if ((ship = dynamic_cast<AsteroidShip*>(other)) != NULL) {
      shouldRemove = true;
      if (radius > 2) {
         int dimension = rand() % 3;
         custodian->add(makeChild(0, dimension));
         custodian->add(makeChild(1, dimension));
      }
   } else if ((shot = dynamic_cast<Shot*>(other)) != NULL) {
      if (health > 0) {
         if (dynamic_cast<BeamShot*>(other) != NULL) {
            health = 0;
            velocity->addUpdate(shot->velocity->scalarMultiply(10));
         } else if (dynamic_cast<TractorBeamShot*>(other) != NULL) {
            // Do nothing.
         } else if (dynamic_cast<ElectricityShot*>(other) != NULL) {
            health = health - .05;
         } else {
            if (gameState->godMode) {
               health = 0;
            } else {
               health--;
            }
            double speed = velocity->getLength();
            speed += 5.0 / radius;
            velocity->addUpdate(*shot->velocity);
            velocity->setLength(speed);
         }
      }
      if (health <= 0) {
         SoundEffect::playSoundEffect("Explosion1.wav");
         shouldRemove = true;
         const int explosionFactor = 3;
         Sprite::sprites.push_back(
               new Sprite("Images/SkybusterExplosion.bmp", 4, 5, 20,
                  *position, radius * explosionFactor,
                  radius * explosionFactor));
         if (radius > 2) {
            shot->owner->score += (int) radius * 10;
            int dimension = rand() % 3;
            custodian->add(makeChild(0, dimension));
            custodian->add(makeChild(1, dimension));
         } else {
            custodian->add(makeShard(0));
            //printf("YOU JUST SHARDED\n");
         }
      }
   }
}

Shard* Asteroid3D::makeShard(int num) {
   Shard* shard;
   shard = new Shard(0.5, worldSize);
   shard->velocity->update(0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
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
   asteroid = new Asteroid3D(radius/2, worldSize);
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
