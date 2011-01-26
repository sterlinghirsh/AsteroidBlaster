/**
 * Sterling Hirsh
 * Asteroid3D.cpp
 * A randomly generated asteroid object.
 */

#include "Graphics/GlutUtility.h"
#include "Graphics/Sprite.h"
#include "Items/Asteroid3D.h"
#include <algorithm>
#include <math.h>

using namespace std;

Asteroid3D::Asteroid3D(double r, double worldSizeIn) : 
 Object3D(0, 0, 0, 0) {
   position->x = randdouble() * worldSizeIn - worldSizeIn / 2;  
   position->y = randdouble() * worldSizeIn - worldSizeIn / 2;  
   position->z = randdouble() * worldSizeIn - worldSizeIn / 2;  
   InitAsteroid(r, worldSizeIn);
}

Asteroid3D::~Asteroid3D() {
   
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

   rotationAmount = 0;
   radius = r;
   worldSize = worldSizeIn;

   scalex = .75 + randdouble() * .5;
   scaley = .6 + randdouble() * .8;
   scalez = .3 + randdouble() * 1.4;

   int topIndex = mesh.addPoint(0, randRadius(r), 0); // Top
   int bottomIndex = mesh.addPoint(0, -randRadius(r), 0); // Bottom

   rotationSpeed = randdouble() * 100; // Degrees per sec.
   rotationVector.randomMagnitude();

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
   collisionRadius = distance3D(sizeX, sizeY, sizeZ);
}

void Asteroid3D::draw(bool drawSmooth = true) {
   glPushMatrix();
   glTranslatef(position->x, position->y, position->z);
   glRotatef(rotationAmount, rotationVector.xMag,
         rotationVector.yMag, rotationVector.zMag);
   glScalef(scalex, scaley, scalez);
   mesh.draw(drawSmooth);
   glPopMatrix();
}

void Asteroid3D::updatePosition(double timeDiff) {
   position->x += velocity->xMag * timeDiff;
   position->y += velocity->yMag * timeDiff;
   position->z += velocity->zMag * timeDiff;
   rotationAmount += rotationSpeed * timeDiff;
   
   // reflect
   if (position->x + collisionRadius > worldSize / 2)  velocity->xMag = -fabs(velocity->xMag);
   if (position->x - collisionRadius < -worldSize / 2) velocity->xMag =  fabs(velocity->xMag);
   if (position->y + collisionRadius > worldSize / 2)  velocity->yMag = -fabs(velocity->yMag);
   if (position->y - collisionRadius < -worldSize / 2) velocity->yMag =  fabs(velocity->yMag);
   if (position->z + collisionRadius > worldSize / 2)  velocity->zMag = -fabs(velocity->zMag);
   if (position->z - collisionRadius < -worldSize / 2) velocity->zMag =  fabs(velocity->zMag);
}

bool Asteroid3D::handleHit(list<Asteroid3D*>& asteroids) {
   const int explosionFactor = 3;
   Sprite::sprites.push_back(
      new Sprite("Images/SkybusterExplosion.bmp", 4, 5, 20, 
       *position, radius * explosionFactor, radius * explosionFactor));

   if (radius > 0.5) {
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
