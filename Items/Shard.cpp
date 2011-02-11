/**
 * Chris Brenton
 * Shard.cpp
 * A randomly generated shard object.
 */

#include "Graphics/GlutUtility.h"
#include "Graphics/Sprite.h"
#include "Items/Shard.h"
#include "Items/Asteroid3D.h"
#include "Items/AsteroidShip.h"
#include "Shots/AsteroidShot.h"
#include <algorithm>
#include <math.h>

using namespace std;

Shard::Shard(double r, double worldSizeIn) :
   Object3D(0, 0, 0, 0) {
      worldSize = worldSizeIn;
      InitShard(r, worldSizeIn);
   }

Shard::~Shard() {

}

void Shard::InitShard(double r, double worldSizeIn) {
   double tempRad;
   double x, y, z;
   const double pi = 3.141592;

   angle = 0;
   radius = r;
   worldSize = worldSizeIn;

   scalex = scaley = scalez = 1;

   //rotationSpeed = randdouble() * 100; // Degrees per sec.
   rotationSpeed = 360 * SPINS_PER_SEC; // Degrees per sec.
   axis = new Vector3D(0, 1, 0);
   axis->randomMagnitude();

   velocity = new Vector3D(0, 0, 0);

   Ring last;
   double a = randdouble() * 0.5 + 0.75;
   double b = randdouble() * 0.5 + 0.75;

   int npts = 4;

   for (int j = npts / 2; j >= 0; j--) {
      double angle = (M_PI * 2 / (double)npts * (double)j);
      double tmpRad = r * sin(angle);
      double tmpH = r * cos(angle);
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
   collisionRadius = radius;
   updateBoundingBox();
}

void Shard::draw() {
   glColor3f(1, 1, 1);
   //materials(Rock);
   // Call the display list if it has one.
   Object3D::draw();
   glEnable(GL_COLOR_MATERIAL);
   glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
   glPushMatrix();
   glTranslatef(position->x, position->y, position->z);
   glRotatef(angle, axis->xMag, axis->yMag, axis->zMag);
   glScalef(scalex, scaley, scalez);

   glColor3f(0.4, 0.5, 0.7);
   glPolygonOffset(1.0f, 1.0f);
   glEnable(GL_POLYGON_OFFSET_FILL);
   glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
   mesh.draw(false);
   glDisable(GL_POLYGON_OFFSET_FILL);

   glColor3f(0.325, 0.71, 0.808);
   //glLineWidth(2);
   glEnable(GL_POLYGON_OFFSET_LINE);
   glPolygonOffset(-1.0f, -1.0f);
   glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
   mesh.drawLines(false);
   glDisable(GL_POLYGON_OFFSET_LINE);
   glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
   //glLineWidth(1);

   glDisable(GL_COLOR_MATERIAL);
   glPopMatrix();
}

void Shard::makeStrip(Ring r1, Ring r2) {
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
   for (unsigned i = 0; i < t2.size(); i++) {
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

void Shard::update(double timeDiff) {
   Object3D::update(timeDiff);
   angle += rotationSpeed * timeDiff;
}

void Shard::handleCollision(Object3D* other) {
   Shard* otherAsteroid;
   AsteroidShip* ship;
   AsteroidShot* shot;
   if ((ship = dynamic_cast<AsteroidShip*>(other)) != NULL) {
      shouldRemove = true;
      //printf("you just sharded\n");
      //ship->score += 69;
      //ship->numShards++;
   }
}

double Shard::randRadius(double r) {
   return (3 * (r / 4)) + ((r / 4) * randdouble());
}

/**
 * Subclasses can extend this, but this draws a sphere on the minimap.
 */
void Shard::drawInMinimap() {
   glPushMatrix();
   position->glTranslate();
   //glDisable(GL_LIGHTING);
   //glColor3f(0.4, 0.5, 0.7);
   materials(WhiteSolid);
   gluSphere(quadric, 3, 4, 2);
   //glEnable(GL_LIGHTING);
   glPopMatrix();
}

void Shard::debug() {
   printf("Shard::debug(): \n");
   minPosition->print();
   maxPosition->print();
   velocity->print();
   printf("--\n");
}
