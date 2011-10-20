/**
 * ElectricityShot.cpp
 * Incredibly attractive shots.
 * Vwahhhh
 * @author Sterling Hirsh (shirsh@calpoly.edu)
 * @date Valentine's Day
 * <3
 */

#include "Items/Shard.h"
#include "Shots/ElectricityShot.h"
#include <math.h>

#ifdef WIN32
#include "Utility/WindowsMathLib.h"
#endif

static float flashiness = 0;

ElectricityShot::ElectricityShot(Point3D& posIn, Vector3D dirIn, int _weaponIndex,
 AsteroidShip* const ownerIn, double strengthOfShot, const GameState* _gameState) : 
 Shot(posIn, dirIn, _weaponIndex, ownerIn, _gameState) {
   type = TYPE_ELECTRICITYSHOT;
   persist = true;
   angle = M_PI / 360; // Radians from the center
   length = 80;
   farRadius = length * tan(angle);
   framesAlive = 0;
   forward = new Vector3D(*velocity); // Goofy.
   up = new Vector3D(0, 1, 0);
   Point3D endPoint1(*position);
   Point3D endPoint2(*position);
   // Set endPoint2 100 units away.
   velocity->setLength(length);
   velocity->movePoint(endPoint2);
   velocity->normalize();
   // Correct for position when calculating endpoint1 and 2.
   Vector3D positionVector(*position);
   positionVector = positionVector.scalarMultiply(-1);
   positionVector.movePoint(endPoint1);
   positionVector.movePoint(endPoint2);
   // Now set min/max xyz
   minX = std::min(endPoint1.x, endPoint2.x);
   maxX = std::max(endPoint1.x, endPoint2.x);
   minY = std::min(endPoint1.y, endPoint2.y);
   maxY = std::max(endPoint1.y, endPoint2.y);
   minZ = std::min(endPoint1.z, endPoint2.z);
   maxZ = std::max(endPoint1.z, endPoint2.z);

   // Expand the bounding box to make sure it contains the whole cone.
   minX -= farRadius;
   minY -= farRadius;
   minZ -= farRadius;
   maxX += farRadius;
   maxY += farRadius;
   maxZ += farRadius;

   timeFired = gameState->getGameTime();
   shouldConstrain = false;
   /* Make sure tractor beam shots aren't culled from the view frustum (necessary to make them appear)
   */
   shouldBeCulled = false;
   updateBoundingBox();
   const double damageBase = 3;
   damage = damageBase * strengthOfShot; // We might want to change this.
   collisionType = new CollisionRay(length, *velocity, *position);
   hitYet = false;
}

/**
 * Kill this after a certain number of frames.
 */
void ElectricityShot::update(double timeDiff) {
   if (++framesAlive >= 2)
      shouldRemove = true;
}

void ElectricityShot::drawGlow() {
   drawShot(true);
}

void ElectricityShot::draw() {
   drawShot(false);
}

/**
 * We want to remove this after drawing it once.
 */
void ElectricityShot::drawShot(bool isGlow) {
   if (!isGlow)
      glUseProgram(elecShader);
   glPushMatrix();

   //floats used in loop iteration
   GLint loc1;
   float j;
   float k;
   float x;
   float y;
   float z;

   //multipliers for randomness in lightning
   float rot;
   float srot;

   //width of inidvidual lightning lines
   float thickness = 5.0;
   if (isGlow) {
      thickness = 10.0;
   }

   //density of the lightning in the beam
   float density = 2;

   //Width of the lightning shot
   int elecWidth = 25;

   //how fast you want the lighting flashing from blue to white. Higher number == faster flash
   float flash = .7f;

   Point3D start(*position);
   velocity->movePoint(start, length);
   start.glTranslate();

   glRotate();
   flashiness = flashiness + flash;
   //if (flashiness >= 360 ) {
   if (flashiness >= 2 * M_PI) {
      flashiness = 0;
   }
   if (!isGlow) {
      loc1 = glGetUniformLocation(elecShader,"flash");
      glUniform1f(loc1,flashiness);
      setMaterial(GreenShiny);
   } else {
      glEnable(GL_COLOR_MATERIAL);
      glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
      glColor3f(0.5, 0.5, 1.0);
   }

   glLineWidth(thickness);

   glBegin(GL_LINES);

   //Creates the twisted lines whose vertices are sent to the shader to modify
   for(k = -density/2; k < density/2; k = k+1){
      x = 0;
      y = 0;
      z = (float) length;
      for(j = 0; j < length ; j = j+(float)(rand() % 10)){
         glVertex3f(x, y, z);

         rot = (float) (rand() % elecWidth - elecWidth/2);
         srot = (float) (rand() % elecWidth - elecWidth/2);
         glVertex3d(srot * (1 / length), rot  * (1 / length), length - j - 2);
         x = (float) (srot * (1 / length));
         y = (float) (rot * (1 / length));
         z = (float) (length - j - 2);
      }
   }
   glEnd();


   glLineWidth(1.0);
   glPopMatrix();
   if (!isGlow) {
      glUseProgram(0);
   } else {
      glDisable(GL_COLOR_MATERIAL);
   }

}

