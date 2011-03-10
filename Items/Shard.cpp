/**
 * Chris Brenton
 * Shard.cpp
 * A randomly generated shard object.
 */

#define _USE_MATH_DEFINES
#include "Utility/GlobalUtility.h"
#include "Graphics/Sprite.h"
#include "Items/Shard.h"
#include "Items/Asteroid3D.h"
#include "Items/AsteroidShip.h"
#include "Shots/Shot.h"
#include "Shots/BeamShot.h"
#include "Shots/TractorBeamShot.h"
#include "Shots/ElectricityShot.h"
#include <algorithm>
#include <math.h>
#include "Utility/SoundEffect.h"
#include "Particles/TractorAttractionParticle.h"

using namespace std;

/*
 * Basic constructor.
 */
Shard::Shard(double r, double worldSizeIn) :
   Object3D(0, 0, 0, 0) {
      shouldDrawInMinimap = true;
      worldSize = worldSizeIn;
      orbiters = glGenLists(1);
      genOrbiters();

      /*
      cubeList = glGenLists(1);
      glNewList(cubeList, GL_COMPILE);
      glutSolidCube (0.6);
      glEndList();
      */
      transparentZ = -8.0;

      InitShard(r, worldSizeIn);
   }

/*
 * Virtual destructor.
 */
Shard::~Shard() {

}

/*
 * Create a display list for the shard's orbiters.
 */
void Shard::genOrbiters() {
   glNewList(orbiters, GL_COMPILE);
   glDisable(GL_LIGHTING);
   GLUquadricObj *q;
   q = gluNewQuadric();
   gluSphere(q, 1.0, 4, 4);
   glEnable(GL_LIGHTING);
   glEndList();
}

/*
 * Initialize the shard.
 */
void Shard::InitShard(double r, double worldSizeIn) {
   const double pi = 3.141592;

   angle = 0;
   radius = r;
   worldSize = worldSizeIn;

   scalex = scaley = scalez = 1;

   rotationSpeed = 360 * SPINS_PER_SEC; // Degrees per sec.
   axis = new Vector3D(0, 1, 0);
   axis->randomMagnitude();

   velocity = new Vector3D(0, 0, 0);

   Ring last;
   double a = randdouble() * 0.5 + 0.75;
   double b = randdouble() * 0.5 + 0.75;

   int npts = 4;

   // Make essentially an asteroid with three levels, containing one, four,
   // and one points around the radius, respectively.
   for (int j = npts / 2; j >= 0; j--) {
      double tmpangle = (M_PI * 2 / (double)npts * (double)j);
      double tmpRad = r * sin(tmpangle);
      double tmpH = r * cos(tmpangle) * 2.0;
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

   // Determine the max and min x, y, and z of this shard.
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

   // Connect the rings from the beginning of _rList to the halfway point.
   for (unsigned i = 1; i <= _rList.size() / 2; i++) {
      makeStrip(_rList[i - 1], _rList[i]);
   }

   // Connect the rings from the halfway point of _rList to the end.
   for (unsigned i = _rList.size() / 2; i < _rList.size() - 1; i++) {
      makeStrip(_rList[i], _rList[i + 1]);
   }

   sizeX = maxX - minX;
   sizeY = maxY - minY;
   sizeZ = maxZ - minZ;
   collisionRadius = radius;
   updateBoundingBox();
}

void drawOtherOrbiters() {
   double curTime = doubleTime();
   const int numBalls = 1;

   // Make it so balls will draw evenly spaced.
   double increment = M_PI * (2.0 / numBalls);
   double t; // parameter for parametric function.
   double dx, dy, dz;
   glPushMatrix();
   int j;
   //glScalef(3, 3, 3);
   setMaterial(WhiteSolid);
   //setMaterial(BlackSolid);
   for (int i = 0; i < numBalls; ++i) {
      t = 5 * curTime + increment * i;

      for (j = 0; j < 3; ++j) {
         dx = 0;
         dy = 0;
         dz = 0;
         if (j % 3 == 0) {
            dx = sin(t);
            dy = cos(t);
         }
         if (j % 3 == 1) {
            dy = sin(t + M_PI * 4/ 3);
            dz = cos(t + M_PI * 4 / 3);
         }
         if (j % 3 == 2) {
            dz = sin(t + M_PI * 2 / 3);
            dx = cos(t + M_PI * 2 / 3);
         }
         glPushMatrix();
         glTranslatef(dx, dy, dz);
         gluSphere(quadric, 0.1f, 16, 16);
         glPopMatrix();
      }
   }
   glPopMatrix();
}

void Shard::drawGlow() {
   glColor3f(0, 0, 0);
   // Call the display list if it has one.
   Object3D::draw();
   // Disable materials.
   //glEnable(GL_COLOR_MATERIAL);
   //glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
   glPushMatrix();
   glTranslatef(position->x, position->y, position->z);
   // Push matrix and draw main shard.
   glPushMatrix();
   glRotatef(angle, axis->xMag, axis->yMag, axis->zMag);
   glScalef(scalex, scaley, scalez);
   glDisable(GL_LIGHTING);

   // Set polygon offset to be behind the lines.
   glPolygonOffset(1.0f, 1.0f);
   glEnable(GL_POLYGON_OFFSET_FILL);
   glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
   // Draw all triangles in the mesh with smoothing turned off.
   mesh.draw(false);
   glDisable(GL_POLYGON_OFFSET_FILL);

   // Set polygon offset to be in front of the faces.
   glPolygonOffset(-1.0f, -1.0f);
   glEnable(GL_POLYGON_OFFSET_LINE);
   glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
   // Draw the lines that border the mesh's triangles.
   mesh.drawLines(false);
   glDisable(GL_POLYGON_OFFSET_LINE);
   glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
   glPopMatrix();

   drawOtherOrbiters();
   // For each ring of orbiters, rotate the proper amount.
   /*for (int i = 0; i < NUM_ORBIT_RINGS; i++) {
     ORBITER_CLR;
     double tilt = 180.0 / (double)NUM_ORBIT_RINGS;
     glRotatef(tilt, axis->xMag, axis->yMag, axis->zMag);
     glPushMatrix();
   // Calculate the current rotation for the current orbiter ring.
   // Alternates between positive and negative.
   double tmpRot = ORBIT_RATE * angle * 2.0 * (1.0 + 0.1 * i);
   for (int j = 0; j < i; j++) {
   tmpRot *= -1;
   }
   glRotatef(tmpRot, 0, 1, 0);
   glScalef(scalex, scaley, scalez);
   //double initAngle = (double)rand() / (double)RAND_MAX
   double initAngle = 360.0 / NUM_ORBIT_RINGS * i;
   for (int k = 0; k < NUM_ORBITERS; k++) {
   //printf("ring %d, orbiter %d, tilt[%d] = %f\n", i, k, i, tilt * i);
   glPushMatrix();
   // Rotate each orbiter by the appropriate amount to form a ring.
   glRotatef(360.0 / NUM_ORBITERS * k + initAngle, 0, 1, 0);
   // Move the orbiter away from the origin to set the ring's radius.
   glTranslatef(RING_RAD, 0.0, 0.0);
   glScalef(ORBITER_RAD, ORBITER_RAD, ORBITER_RAD);
   // Draw an orbiter from the list.
   //glCallList(orbiters);
   glPopMatrix();
   }
   glPopMatrix();
   }
   */

   //glDisable(GL_COLOR_MATERIAL);
   glEnable(GL_LIGHTING);
   glPopMatrix();
}

void Shard::draw() {
   GLfloat mat_specular[] = { 0.1, 0.1, 0.3, 0.75 };
   GLfloat mat_shininess[] = { 100.0 };

   GLfloat mat_transparent[] = { 0.2, 0.2, 0.5, 0.9 };
   GLfloat mat_emission[] = { 0.0, 0.1, 0.3, 0.2 };
   GLfloat mat_no_emission[] = { 0.0, 0.0, 0.0, 1.0 };

   glColor3f(1, 1, 1);
   // Call the display list if it has one.
   Object3D::draw();
   // Disable materials.
   glEnable(GL_COLOR_MATERIAL);
   glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
   glPushMatrix();
   glTranslatef(position->x, position->y, position->z);
   // Push matrix and draw main shard.
   glPushMatrix();
   glRotatef(angle, axis->xMag, axis->yMag, axis->zMag);
   glScalef(scalex, scaley, scalez);

   //setMaterial(Rock);
   glMaterialfv(GL_FRONT, GL_EMISSION, mat_emission);
   glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_transparent);
   glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
   glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
   //glDepthMask(GL_FALSE);
   //glColor4f(0.4, 0.5, 0.7, 0.7);
   // Set polygon offset to be behind the lines.
   glPolygonOffset(1.0f, 1.0f);
   glEnable(GL_POLYGON_OFFSET_FILL);
   glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
   // Draw all triangles in the mesh with smoothing turned off.
   mesh.draw(false);
   glDisable(GL_POLYGON_OFFSET_FILL);
   glMaterialfv(GL_FRONT, GL_EMISSION, mat_no_emission);
   //glDepthMask(GL_TRUE);

   glColor3f(0.325, 0.71, 0.808);
   // Set polygon offset to be in front of the faces.
   glPolygonOffset(-1.0f, -1.0f);
   glEnable(GL_POLYGON_OFFSET_LINE);
   glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
   // Draw the lines that border the mesh's triangles.
   mesh.drawLines(false);
   glDisable(GL_POLYGON_OFFSET_LINE);
   glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
   glPopMatrix();

   drawOtherOrbiters();
   // For each ring of orbiters, rotate the proper amount.
   /*for (int i = 0; i < NUM_ORBIT_RINGS; i++) {
     ORBITER_CLR;
     double tilt = 180.0 / (double)NUM_ORBIT_RINGS;
     glRotatef(tilt, axis->xMag, axis->yMag, axis->zMag);
     glPushMatrix();
   // Calculate the current rotation for the current orbiter ring.
   // Alternates between positive and negative.
   double tmpRot = ORBIT_RATE * angle * 2.0 * (1.0 + 0.1 * i);
   for (int j = 0; j < i; j++) {
   tmpRot *= -1;
   }
   glRotatef(tmpRot, 0, 1, 0);
   glScalef(scalex, scaley, scalez);
   //double initAngle = (double)rand() / (double)RAND_MAX
   double initAngle = 360.0 / NUM_ORBIT_RINGS * i;
   for (int k = 0; k < NUM_ORBITERS; k++) {
   //printf("ring %d, orbiter %d, tilt[%d] = %f\n", i, k, i, tilt * i);
   glPushMatrix();
   // Rotate each orbiter by the appropriate amount to form a ring.
   glRotatef(360.0 / NUM_ORBITERS * k + initAngle, 0, 1, 0);
   // Move the orbiter away from the origin to set the ring's radius.
   glTranslatef(RING_RAD, 0.0, 0.0);
   glScalef(ORBITER_RAD, ORBITER_RAD, ORBITER_RAD);
   // Draw an orbiter from the list.
   //glCallList(orbiters);
   glPopMatrix();
   }
   glPopMatrix();
   }
   */

   //glDisable(GL_COLOR_MATERIAL);
   glPopMatrix();
}

/*
 * Connect two Ring objects by making triangles from the points and adding them to
 * this shard's mesh.
 */
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

/*
 * Update the shard's rotation and position based on elapsed time.
 */
void Shard::update(double timeDiff) {
   Object3D::update(timeDiff);
   angle += rotationSpeed * timeDiff;
   double speed = velocity->getLength();
   if (speed != 0.0) {
      speed *= (1.0 - DECEL_RATE * timeDiff);
      velocity->setLength(speed);
   }
}

/*
 * Handle a collision with another Object3D.
 */
void Shard::handleCollision(Object3D* other) {
   AsteroidShip* ship;
   Asteroid3D* asteroid;
   Shot* shot;
   ElectricityShot* Eshot;
   TractorBeamShot* TBshot; // Not tuberculosis
   if ((ship = dynamic_cast<AsteroidShip*>(other)) != NULL) {
      shouldRemove = true;
      SoundEffect::playSoundEffect("CrystalCollect.wav");
   } else if ((asteroid = dynamic_cast<Asteroid3D*>(other)) != NULL) {
      // Set speed to the speed of the asteroid.
      double speed = asteroid->velocity->getLength();
      velocity->updateMagnitude(*(asteroid->position), *position);
      velocity->setLength(speed);
   } else if ((shot = dynamic_cast<Shot*>(other)) != NULL) {
      double speed;
      if (dynamic_cast<BeamShot*>(other) != NULL) {
         speed = 80; // High speed from hard-hitting railgun.
      } else if((TBshot = dynamic_cast<TractorBeamShot*>(other)) != NULL) {
         // Pull the shot in.
         const int numParticles = 1;
         speed = position->distanceFrom(*TBshot->position) - TBshot->length;
         Vector3D random;
         for (int i = 0; i < numParticles; ++i) {
            Point3D* particlePosition = new Point3D(*position);
            random.randomMagnitude();
            random.movePoint(*particlePosition);
            Vector3D* particleVelocity = new Vector3D(*particlePosition, *TBshot->position);
            particleVelocity->setLength(10); // Speed of the particle.
            particleVelocity->addUpdate(*velocity);
            // Make this go toward the ship.
            TractorAttractionParticle::Add(particlePosition, particleVelocity, TBshot->owner->position); 
         }
      } else if((Eshot = dynamic_cast<ElectricityShot*>(other)) != NULL) {
         // Pull the shot in.
         //speed = position->distanceFrom(*TBshot->position) - TBshot->length;
      } else {
         // Set speed to between the speed of the shot and the current speed.
         speed = shot->velocity->getLength();
         speed += velocity->getLength() * 2;
         speed /= 3;
      }
      velocity->updateMagnitude(*(shot->position), *position);
      velocity->setLength(speed);
   }
}

/*
 * Generate a random radius.
 */
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
   setMaterial(WhiteSolid);
   gluSphere(quadric, 1, 4, 2);
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
