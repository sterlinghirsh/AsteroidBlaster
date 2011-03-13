/**
 * LawnMowerShot.cpp
 * Incredibly attractive shots.
 * Vwahhhh
 * @author Sterling Hirsh (shirsh@calpoly.edu)
 * @date Valentine's Day
 * <3
 */

#include "Items/Shard.h"
#include "Shots/LawnMowerShot.h"
#include <math.h>

static float spin = 1;
static float adder = .10;
static float spinner = 0;
static float otherSpin = 1.5;
static float otherAdder = .05;
static float spinster = 1.5;
static float adderer = .08;
static float poop = 2.5;
static float bladder = .05;

LawnMowerShot::LawnMowerShot(Point3D& posIn, Vector3D dirIn, 
 AsteroidShip* const ownerIn) : Shot(posIn, dirIn, ownerIn) {
   persist = true;
   angle = M_PI / 40; // Radians from the center
   length = 10;
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
void LawnMowerShot::update(double timeDiff) {
   if (++framesAlive >= 2)
      shouldRemove = true;
}

/**
 * We want to remove this after drawing it once.
 */
void LawnMowerShot::draw() {
   
   glUseProgram(lawnShader);
   glPushMatrix();
   
      //floats used in loop iteration
      float j;
      float k;
      float x, y, z;
      GLint loc1;
      
      //rotation for the Y value of the vertex using cosine
      float rot;
      
      //rotation for the X value of the vertex using sine
      float srot;
      
      //Number of lines you want generated in the rotating tractor beam
      float numCurves = 1;
      
      //used as a multiplier to create the correct gap in between the lines
      float gap = length / numCurves;
      
      //must be 2 for the cone to be complete, less than 2 for an incomplete cone, more 
      //than 2 for spacing to be irregular between the lines
      float sharpness = 2;
      
      float lpos[4] = {1.0, 0.5, 1.0, 0.0};	// light postion
      //glLightfv(GL_LIGHT0, GL_POSITION, lpos);
      //Point3D start(*position);
      //velocity->movePoint(start);
      position->glTranslate();
      
      glRotate();
      spinner += 2;
      glRotatef(-spinner,0,0,1);
      if (spinner >= 360 ) {
         spinner = 0;
      }
      otherSpin += otherAdder;
      spin = spin + adder;
      spinster +=  adderer;
      poop += bladder;
      //printf("Spin variable: %f\n", spin);
      //glRotatef(-spin,1, 0, 0);
      if (spin >= 4 || spin <= 1) {
         
         adder *= -1;
      }
      if (otherSpin >= 2 || otherSpin <= 1) {
         
         otherAdder *= -1;
      }
      if (spinster >= 4 || spinster <= 1) {
         
         adderer *= -1;
      }
      if (poop >= 3 || poop <= 1) {
         
         bladder *= -1;
      }
      loc1 = glGetUniformLocation(lawnShader,"clr");
      glUniform1f(loc1,10);
      
      setMaterial(GreenShiny);
      glLineWidth(2.0);
      glBegin(GL_LINES);
      //Creates the twisted lines whose vertices are sent to the shader to modify
      
      //glVertex3f(0, 0, 0);
      //glVertex3f(0, 0, -length);


            x = y = z = 0; 
            for(j = 0; j < length ; j = j+.5){
            
                  rot = cos((j / length) * M_PI/1.5);

                  glVertex3f(x, y, z);

                  glVertex3f(j/length * spin + rot * (j + 1) * (farRadius / length), j/length,  - j);

                  glVertex3f(-x, y, z);
                  glVertex3f(-j/length * spin + -rot * (j + 1) * (farRadius / length), j/length,  - j);
                  x = j/length*spin + rot * (j + 1) * (farRadius / length);
                  y = j/length;
                  z = - j;

            }
            glEnd();

            glBegin(GL_LINES);


            x = y = z = 0; 
            for(j = 0; j < length ; j = j+.5){
            
                  rot = cos((j / length) * M_PI/1.5);

                  glVertex3f(x, y, z);

                  glVertex3f(j/length * spinster + rot * (j + 1) * (farRadius / length), -j/length,  - j);

                  glVertex3f(-x, y, z);
                  glVertex3f(-j/length * spinster + -rot * (j + 1) * (farRadius / length), -j/length,  - j);
                  x = j/length*spinster + rot * (j + 1) * (farRadius / length);
                  y = -j/length;
                  z = - j;
            }
      
      glEnd();
      glBegin(GL_LINES);

            x = y = z = 0; 
            for(j = 0; j < length ; j = j+.5){
            
                  rot = cos((j / length) * M_PI/1.5);

                  glVertex3f(x, y, z);

                  glVertex3f(j/length * otherSpin + rot * (j + 1) * (farRadius / length), 2*j/length,  - j);

                  glVertex3f(-x, y, z);
                  glVertex3f(-j/length * otherSpin + -rot * (j + 1) * (farRadius / length), 2*j/length,  - j);
                  x = j/length*otherSpin + rot * (j + 1) * (farRadius / length);
                  y = 2*j/length;
                  z = - j;

            }
            glEnd();
            
            glBegin(GL_LINES);
      //Creates the twisted lines whose vertices are sent to the shader to modify


            x = y = z = 0; 
            for(j = 0; j < length ; j = j+.5){
            
                  rot = cos((j / length) * M_PI/1.5);

                  glVertex3f(x, y, z);

                  glVertex3f(j/length * poop + rot * (j + 1) * (farRadius / length), -2*j/length,  - j);

                  glVertex3f(-x, y, z);
                  glVertex3f(-j/length * poop + -rot * (j + 1) * (farRadius / length), -2*j/length,  - j);
                  x = j/length*poop + rot * (j + 1) * (farRadius / length);
                  y = -2*j/length;
                  z = - j;

            }
      
      glEnd();

   glLineWidth(1.0);
   glPopMatrix();
   glUseProgram(0);
}

/**
 * Is the object in the cone?
 */
bool LawnMowerShot::detectCollision(Drawable* other, bool checkOther) {
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

   if (forwardDistance > other->radius + length) {
      return false;
   }

   double requiredDistance = forwardDistance * tan(angle);
   Point3D closestPoint(*position);
   tempDirection.movePoint(closestPoint, forwardDistance);
   return (other->position->distanceFrom(closestPoint) <=
    (other->radius + requiredDistance));
}

void LawnMowerShot::handleCollision(Drawable* other) {
   if (other == owner)
      return;
}

