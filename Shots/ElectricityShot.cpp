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

static float spin = 0;

ElectricityShot::ElectricityShot(Point3D& posIn, Vector3D dirIn, 
 AsteroidShip* const ownerIn) : Shot(posIn, dirIn, ownerIn) {
   persist = true;
   angle = M_PI / 20; // Radians from the center
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

   timeFired = doubleTime();
   shouldConstrain = false;
   /* Make sure tractor beam shots aren't culled from the view frustum (necessary to make them appear)
    */
   shouldBeCulled = false;
   updateBoundingBox();
}

/**
 * Kill this after a certain number of frames.
 */
void ElectricityShot::update(double timeDiff) {
   if (++framesAlive >= 2)
      shouldRemove = true;
}

/**
 * We want to remove this after drawing it once.
 */
void ElectricityShot::draw() {
   
   glUseProgram(elecShader);
   glPushMatrix();
   
      //floats used in loop iteration
      GLint loc1;
      float j;
      float k;
      float x = 0;
      float y = 0;
      float z = 0;
      int l = 0;
      
      //rotation for the Y value of the vertex using cosine
      float rot;
      
      //rotation for the X value of the vertex using sine
      float srot;
      
      //Number of lines you want generated in the rotating tractor beam
      float numCurves = 2;
      
      //used as a multiplier to create the correct gap in between the lines
      float gap = length / numCurves;
      
      //must be 2 for the cone to be complete, less than 2 for an incomplete cone, more 
      //than 2 for spacing to be irregular between the lines
      float sharpness = 2;
      
      float lpos[4] = {1.0, 0.5, 1.0, 0.0};	// light postion
      //glLightfv(GL_LIGHT0, GL_POSITION, lpos);
      Point3D start(*position);
      velocity->movePoint(start);
      start.glTranslate();
      
      glRotate();
      //glRotatef(-spin++,0,0,1);
      spin = spin + .2;
      if (spin >= 360 ) {
         spin = 0;
      }
      loc1 = glGetUniformLocation(elecShader,"spin");
      glUniform1f(loc1,spin);

      setMaterial(GreenShiny);
      glLineWidth(.50);
      glBegin(GL_LINES);
      z = length;
      //Creates the twisted lines whose vertices are sent to the shader to modify
      for(k = -numCurves/2; k < numCurves/2; k = k+1){
            for(j = 0; j < length ; j = j+.1){
                  //rot = cos(rand());
                  //srot = sin(rand());
                  glVertex3f(x, y, z);
                  
                  rot = cos(rand()) * 3;
                  srot = sin(rand()) * 3;
                  glVertex3f(srot * (l % 4) *  (1 / length), rot  * (l % 4) *  (1 / length), length - j - .1);
                  glVertex3f(-x, -y, z);
                  x = srot * (l % 4) *  (1 / length);
                  y = rot * (l % 4) *  (1 / length);
                  z = length - j - .1;
                  glVertex3f(-x, -y, z);
                  l++;
            }
      }
      glEnd();

   glLineWidth(1.0);
   glPopMatrix();
   glUseProgram(0);
}

/**
 * Is the object in the cone?
 */
bool ElectricityShot::detectCollision(Object3D* other, bool checkOther) {
   if (other == owner)
      return false;
   
   if (gameState->godMode) {
      return true;
   }

   // This is how far it is for position to other->position.
   Vector3D shotToTarget(*position, *other->position);
   
   Vector3D tempDirection(*velocity);
   tempDirection.normalize();

   double forwardDistance = tempDirection.dot(shotToTarget);
   // Is it behind me?
   if (forwardDistance < 0) {
      return false;
   }

   if (forwardDistance > length) {
      return false;
   }

   double requiredDistance = forwardDistance * tan(angle);
   Point3D closestPoint(*position);
   tempDirection.movePoint(closestPoint, forwardDistance);
   return (other->position->distanceFrom(closestPoint) <=
    (other->radius + requiredDistance));
}

void ElectricityShot::handleCollision(Object3D* other) {
   if (other == owner)
      return;
}
