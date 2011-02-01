/**
 * Sterling Hirsh
 * Asteroid3D.cpp
 * A randomly generated asteroid object.
 */

#include "Graphics/GlutUtility.h"
#include "Graphics/Sprite.h"
#include "Items/Asteroid3D.h"
#include "Items/AsteroidShip.h"
#include "Shots/AsteroidShot.h"
#include <algorithm>
#include <math.h>

using namespace std;

Asteroid3D::Asteroid3D(double r, double worldSizeIn) : 
 Object3D(0, 0, 0, 0) {
   worldSize = worldSizeIn;
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
   double tempRad;
   double x, y, z;
   const double pi = 3.141592;
   const int hbands = 10 + (rand() % 20);
   const int vbands = 10 + (rand() % 20);
   double theta, phi;
   //FIXME so apperantly hbands and vbands cause serious error when
   //      compiling in VS C++ 2008. Might need optimization.
   const int maxhbands = 30;
   const int maxvbands = 30;
   MeshPoint* points  [maxhbands][maxvbands];
   int pointNums [maxhbands][maxvbands];

   angle = 0;
   radius = r;
   worldSize = worldSizeIn;

   //scalex = .75 + randdouble() * .5;
   //scaley = .6 + randdouble() * .8;
   //scalez = .3 + randdouble() * 1.4;
   scalex = scaley = scalez = 1;

   int topIndex = mesh.addPoint(0, randRadius(r), 0); // Top
   int bottomIndex = mesh.addPoint(0, -randRadius(r), 0); // Bottom

   rotationSpeed = randdouble() * 100; // Degrees per sec.
   axis = new Vector3D(0, 1, 0);
   axis->randomMagnitude();

   velocity = new Vector3D(0, 0, 0);
   velocity->randomMagnitude();
   velocity->setLength(randdouble() * 3); // Units per sec.

   for (int h = 0; h < hbands; ++h) {
         phi = (pi / 2) - 
            (pi * ((double)(1 + h) / (hbands)));
      for (int v = 0; v < vbands; ++v) {
         theta = 2 * pi * ((double)(1 + v) / (vbands));
         tempRad = randRadius(r);
         x = tempRad * cos(phi) * cos(theta);
         y = tempRad * sin(phi);
         z = tempRad * cos(phi) * cos((pi / 2) - theta);
         points[h][v] = new MeshPoint(x, y, z);
         pointNums[h][v] = mesh.addPoint(points[h][v]);
          
      }
   }

   for (int v = 0; v < vbands; ++v) {
      mesh.addFace(topIndex, 
       pointNums[0][(v + 1) % vbands],
            pointNums[0][v]
       );
      for (int h = 0; h < hbands - 1; ++h) {
         mesh.addFace(pointNums[h][v],
          pointNums[h][(v + 1) % vbands],
          pointNums[h + 1][v]
          );

         mesh.addFace(
          pointNums[h][(v + 1) % vbands],
          pointNums[h + 1][(v + 1) % vbands],
          pointNums[h + 1][v]);
      }

      mesh.addFace(bottomIndex, 
       pointNums[hbands - 1][(v + 1) % vbands],
            pointNums[hbands - 1][v]);
   }

   for (int h = 0; h < hbands; ++h) {
      for (int v = 0; v < vbands; ++v) {
         delete points[h][v];
      }
   }

   sizeX = max<double>(mesh.xMax, fabs(mesh.xMin)) * scalex;
   sizeY = max<double>(mesh.yMax, fabs(mesh.yMin)) * scaley;
   sizeZ = max<double>(mesh.zMax, fabs(mesh.zMin)) * scalez;
   collisionRadius = radius;
   minX = mesh.xMin;
   minY = mesh.yMin;
   minZ = mesh.zMin;
   maxX = mesh.xMax;
   maxY = mesh.yMax;
   maxZ = mesh.zMax;
   updateBoundingBox();
}

void Asteroid3D::draw() {
   glColor3f(1, 1, 1);
   materials(Rock);
   Object3D::draw();
   glPushMatrix();
   glTranslatef(position->x, position->y, position->z);
   glRotatef(angle, axis->xMag, axis->yMag, axis->zMag);
   glScalef(scalex, scaley, scalez);
   mesh.draw(false);
   glPopMatrix();
}

void Asteroid3D::update(double timeDiff) {
   Object3D::update(timeDiff);
   angle += rotationSpeed * timeDiff;
}

void Asteroid3D::handleCollision(Object3D* other) {
   Asteroid3D* otherAsteroid;
   AsteroidShip* ship;
   AsteroidShot* shot;
   if ((otherAsteroid = dynamic_cast<Asteroid3D*>(other)) != NULL) {
      double speed = velocity->getLength();

      velocity->updateMagnitude(*(otherAsteroid->position), *position);
      velocity->setLength(speed);
   } else if ((ship = dynamic_cast<AsteroidShip*>(other)) != NULL) {
      shouldRemove = true;
      if (radius > 2) {
         custodian->add(makeChild());
         custodian->add(makeChild());
      }
   } else if ((shot = dynamic_cast<AsteroidShot*>(other)) != NULL) {
      shouldRemove = true;
      const int explosionFactor = 3;
      Sprite::sprites.push_back(
         new Sprite("Images/SkybusterExplosion.bmp", 4, 5, 20, 
          *position, radius * explosionFactor, radius * explosionFactor));
      if (radius > 2) {
         custodian->add(makeChild());
         custodian->add(makeChild());
      }
   }
}

bool Asteroid3D::handleHit(list<Asteroid3D*>& asteroids) {

   if (radius > 1) {
      for (int i = 0; i < 3; ++i) {
         asteroids.push_back(makeChild());
      }
   }
   return true;
}

Asteroid3D* Asteroid3D::makeChild() {
   Asteroid3D* asteroid = new Asteroid3D(radius/2, worldSize);
   //asteroid->velocity = asteroid->velocity->scalarMultiply(2);
   // Make this scalarMultiplyUpdate(2);
   asteroid->velocity->addUpdate(*asteroid->velocity);
   asteroid->velocity->addUpdate(*velocity);
   asteroid->position->clone(position);

   return asteroid;
}

double Asteroid3D::randRadius(double r) {
   return (3 * (r / 4)) + ((r / 4) * randdouble());
}

void Asteroid3D::debug() {
   printf("Asteroid3D::debug(): \n");
   minPosition->print();
   maxPosition->print();
   velocity->print();
   printf("--\n");
}
