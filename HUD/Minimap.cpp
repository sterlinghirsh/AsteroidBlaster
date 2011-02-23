/**
 * Minimap.h
 * A minimap that displays in the corner of the screen.
 * @author Sterling Hirsh
 * @date 2/23/2011
 */

#include "HUD/Minimap.h"
#include "Utility/GlobalUtility.h"
#include "Items/Object3D.h"

#define DEFAULT_DISPLAYSIZE 0.5
#define DEFAULT_ZOOMLEVEL 20

Minimap::Minimap(AsteroidShip* _ship) :
 ship(_ship), displaySize(DEFAULT_DISPLAYSIZE), zoomLevel(DEFAULT_ZOOMLEVEL) {
   // Do nothing.
}

void Minimap::drawLines(std::list<Object3D*>* objects) {
   // Now draw the lines.
   // Load just the rotation matrix.
   static Matrix4 modelViewMatrix;
   const float ringWidth = 1;
   double radius2D; // Radius when an object is projected onto the forward-right plane of the ship.
   double radius3D; // Radius from the ship in 3D space.
   const float scaleFactor = 1 / zoomLevel;
   std::list<Object3D*>::iterator listIter;
   static Point3D objectPosition;
   static Vector3D positionVector;

   glPushMatrix();
   glLoadIdentity(); // We don't want the camera transform.
   ship->glRotate(true);
   modelViewMatrix.loadModelviewMatrix();
   glPopMatrix();
   glPushMatrix();
   glScalef(scaleFactor, scaleFactor, scaleFactor);
   setMaterial(WhiteSolid);
   for (listIter = objects->begin(); listIter != objects->end(); ++listIter) {
      // Make sure it's not null, & then draw it in the minimap
      if (*listIter != NULL && (*listIter)->shouldDrawInMinimap && *listIter != ship) {
         objectPosition = *(*listIter)->position; // Get the position.
         positionVector.movePoint(objectPosition, -1); // Center on the ship
         objectPosition = modelViewMatrix * (objectPosition); // Rotate about the ship
         radius2D = distance2D(objectPosition.x, objectPosition.z);
         radius3D = distance3D(objectPosition.x, objectPosition.y, objectPosition.z);
         if (radius3D < 20) {
            glColor3f(1, 0, 0);
            setMaterial(RedTransparent);
         } else if (radius3D < 30) {
            glColor3f(1, 1, 0);
            setMaterial(YellowTransparent);
         } else {
            glColor3f(0, 1, 0);
            setMaterial(GreenTransparent);
         }

         // Draw a disc.
         // This is the way that one game did it with the circles on a plane.
         glPushMatrix();
         glEnable(GL_LIGHTING);
         glRotatef(-90.0,1.0f,0.0f,0.0f);   // Rotate By 0 On The X-Axis
         gluDisk(quadric, radius2D - (ringWidth / 2), radius2D + (ringWidth / 2) ,16,2);
         glDisable(GL_LIGHTING);
         glPopMatrix();
         //glDisable(GL_LIGHTING);
         glBegin(GL_LINES);
         // Draw a point at the object's position.
         glVertex3f(objectPosition.x, objectPosition.y, objectPosition.z);
         // Draw the other point at the object's position without the Y component.
         glVertex3f(objectPosition.x, 0, objectPosition.z);
         glEnd();
      }
   }
   glPopMatrix();

}

void Minimap::draw() {
   // Get a reading of all the nearby Object3Ds from the Radar
   std::list<Object3D*>* objects = ship->getRadar()->getNearbyReading();
   static Vector3D positionVector;
   const float scaleFactor = 1 / zoomLevel;
   double radius3D; // Radius from the ship in 3D space.
   // Used when looping over Object3Ds in drawMinimap
   std::list<Object3D*>::iterator listIter;
 
   // Draw the minimap
   glPushMatrix();
   /* Set the viewport to 3/4 of the way across the screen on the bottom.
    * It should take up the bottom right corner of the screen.
    */

   glViewport (GW * (1 - displaySize), 0, GW * displaySize, GH * displaySize);
   glMatrixMode (GL_PROJECTION);      /* Select The Projection Matrix */
   glLoadIdentity ();                     /* Reset The Projection Matrix */

   // Keep our aspect ratio relative to the global width and height
   gluPerspective(VERT_FOV, (double)GW/GH, 0.5, 20);

   glMatrixMode (GL_MODELVIEW);      /* Select The Projection Matrix */
   glLoadIdentity ();                        /* Reset The Modelview Matrix */

   glClear (GL_DEPTH_BUFFER_BIT);      /* Clear Depth Buffer */

   setMaterial(WhiteSolid);

   // eye, lookAt, and up vectors
   gluLookAt(0, 2, 5, 0, 0, 0,  0, 1, 0);

   positionVector.updateMagnitude(*ship->position);

   glDisable(GL_LIGHT0); // turn off the normal light, put a light at the center.
   glEnable(GL_LIGHT1);

   glPushMatrix();
   ship->glRotate(false);
   Vector3D oppositeOfPosition(*(ship->position));

   // Translate everything so that the ship is at 0, 0 and everything is centered there.
   oppositeOfPosition.updateMagnitude(oppositeOfPosition);
   glScalef(scaleFactor, scaleFactor, scaleFactor);
   //glRotatef(180, 0, 1, 0);
   oppositeOfPosition.glTranslate(-1);
   glDisable(GL_COLOR_MATERIAL);

   // For each item that needs to be drawn in the minimap
   for (listIter = objects->begin(); listIter != objects->end(); ++listIter) {

      // Make sure it's not null, & then draw it in the minimap
      // TODO: Make this not copy code from below.
      if (*listIter != NULL && ((*listIter)->shouldDrawInMinimap || *listIter == ship)) {
         radius3D = ship->position->distanceFrom(*(*listIter)->position);
         if (radius3D < 20) {
            glColor3f(1, 0, 0);
            setMaterial(RedFlat);
         } else if (radius3D < 30) {
            glColor3f(1, 1, 0);
            setMaterial(YellowFlat);
         } else {
            glColor3f(0, 1, 0);
            setMaterial(GreenTransparent);
         }
         (*listIter)->drawInMinimap();
      }
   }
   glPopMatrix();
   drawLines(objects);

   // Draw the sphere around it.
   glEnable(GL_LIGHTING);
   glDisable(GL_LIGHT1); // Reenable the normal light.
   glEnable(GL_LIGHT0);
   setMaterial(GrayTransparent);
   //glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
   glDisable(GL_CULL_FACE);
   gluSphere(quadric, 2, 20, 20);
   glEnable(GL_CULL_FACE);
  glPopMatrix();
}

/**
 * Set the display size to a new value. This value should be from
 * 0 (not drawn) to 1 (full screen)
 */
void Minimap::setDisplaySize(float newDisplaySize) {
   displaySize = clamp(newDisplaySize, 0, 1);
}

/**
 * Set the zoom level to a new value.
 */
void Minimap::setZoomLevel(float newZoomLevel) {
   zoomLevel = newZoomLevel;
}
