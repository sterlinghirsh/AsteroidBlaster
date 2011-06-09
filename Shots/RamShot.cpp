/**
 * RamShot.cpp
 * Incredibly attractive shots.
 * Vwahhhh
 * @author Sterling Hirsh (shirsh@calpoly.edu)
 * @date Valentine's Day
 * <3
 */

#include "Items/Shard.h"
#include "Shots/RamShot.h"
#include <math.h>

#ifdef WIN32
#include "Utility/WindowsMathLib.h"
#endif

static float flashiness = 0;
static float tracker = 0;
static int rando = 1;

RamShot::RamShot(Point3D& posIn, Vector3D dirIn, int _weaponIndex, 
 AsteroidShip* const ownerIn, const GameState* _gameState) : Shot(posIn, dirIn, _weaponIndex, ownerIn, _gameState) {
   persist = true;
   angle = M_PI / 360; // Radians from the center
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
   
   forward = new Vector3D(*(owner->forward));
   up = new Vector3D(*(owner->up));
   right = new Vector3D(*(owner->right));

   timeFired = doubleTime();
   shouldConstrain = false;
   /* Make sure tractor beam shots aren't culled from the view frustum (necessary to make them appear)
    */
   shouldBeCulled = false;
   updateBoundingBox();

   damage = 100;
}

/**
 * Kill this after a certain number of frames.
 */
void RamShot::update(double timeDiff) {
   if (++framesAlive >= 2)
      shouldRemove = true;
}

/**
 * We want to remove this after drawing it once.
 */
void RamShot::draw() {
   return;
   if (gameSettings->drawDeferred) {
      glDrawBuffer(NOLIGHT_BUFFER);
   } 
   glUseProgram(ramShader);

   glPushMatrix();
   glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
      
      //floats used in loop iteration
      GLint loc1;
      float x;
      float y;
      float z;
      
      //multipliers for randomness in lightning
      float length = 80;
      //width of inidvidual lightning lines
      float thickness = 5.0;
      
      //density of the lightning in the beam
      float density = 2;
      
      //Width of the lightning shot
      int elecWidth = 100;
      
      //how fast you want the lighting flashing from blue to white. Higher number == faster flash
      float flash = .000000000001f;
      
      float lpos[4] = {1.0, 0.5, 1.0, 0.0};	// light postion
      //glLightfv(GL_LIGHT0, GL_POSITION, lpos);
      Point3D start(*position);
      //velocity->movePoint(start);
      start.glTranslate();
      //glRotatef(180, 1, 0, 0);
      glRotate();
      glScaled(5, 5, 5);

      //flashiness = flashiness + flash;
      //int rando;
      tracker++;
      if (flashiness >= 360 ) {
         flashiness = 0;
      }
      if (tracker > 75) {
            tracker = 0;
            rando = rand();
      }
       
      flashiness = flashiness + (float)(rando % 50) / 10.0f;
      loc1 = glGetUniformLocation(ramShader,"poop");
      glUniform1f(loc1,flashiness);
      glTranslatef(0, 0, -.2f);
      setMaterial(GreenShiny);
      glLineWidth(thickness);
      glDisable(GL_CULL_FACE);
      glEnable(GL_LIGHTING);
      glBegin(GL_TRIANGLE_FAN);

    // Center of fan is at the origin
    glColor4f(1, 0, 0, 1);
    glVertex3f(0.0f, 0.0f, 0.0);
    glVertex3f(.5, .2, 1.5);
    glVertex3f(-.5, .2, 1.5);
    glVertex3f(-.5, -.2, 1.5);
    glVertex3f(.5, -.2, 1.5);
    glVertex3f(.5, .2, 1.5);

    glEnd();
    
    glBegin(GL_QUADS);
    glVertex3f(.5, .2, 1.5);
    glVertex3f(-.5, .2, 1.5);
    glVertex3f(-.5, -.2, 1.5);
    glVertex3f(.5, -.2, 1.5);
    glEnd();
      
      glEnable(GL_CULL_FACE);
   glLineWidth(1.0);
   glPopMatrix();
   glUseProgram(0);
   

}
