/**
 * Minimap.h
 * A minimap that displays in the corner of the screen.
 * @author Sterling Hirsh
 * @date 2/23/2011
 */

#include "HUD/Minimap.h"
#include "Utility/GlobalUtility.h"
#include "Items/Object3D.h"

#define DEFAULT_DISPLAYSIZE 0.25
#define MAX_DISPLAYSIZE 1.0
#define MIN_DISPLAYSIZE 0

#define DEFAULT_ZOOMLEVEL 80
#define MAX_ZOOMLEVEL 80
#define MIN_ZOOMLEVEL 20

Minimap::Minimap(AsteroidShip* _ship) :
 ship(_ship), displaySize(DEFAULT_DISPLAYSIZE), zoomLevel(DEFAULT_ZOOMLEVEL) {
   adjustZoomDirection = adjustDisplaySizeDirection = 0;
   targetDisplaySize = DEFAULT_DISPLAYSIZE;
   targetZoomLevel = DEFAULT_ZOOMLEVEL;
   oldDisplaySize = DEFAULT_DISPLAYSIZE;
   hidden = false;
}

void Minimap::drawLines(std::list<Object3D*>* objects) {
   // Now draw the lines.
   // Load just the rotation matrix.
   static Matrix4 modelViewMatrix;
   const float ringWidth = (zoomLevel / 80) / displaySize;
   double radius2D; // Radius when an object is projected onto the forward-right plane of the ship.
   double radius3D; // Radius from the ship in 3D space.
   const float scaleFactor = 1 / zoomLevel;
   std::list<Object3D*>::iterator listIter;
   static Point3D objectPosition;
   static Vector3D positionVector;
   positionVector.updateMagnitude(*ship->position);

   glPushMatrix();
      glLoadIdentity(); // We don't want the camera transform.
      ship->glRotate(true);
      modelViewMatrix.loadModelviewMatrix();
   glPopMatrix();

   const int ringsToDraw = 4;
   const float ringIncrement = zoomLevel / ringsToDraw;
   float ringDiameter;
   
   // Find the highest power of 2 under the zoom level.
   for (ringDiameter = 8; (ringDiameter * 2) <= zoomLevel; ringDiameter *= 2) {
      // Do nothing;
   }

   glPushMatrix();
      glScalef(scaleFactor, scaleFactor, scaleFactor);

      glDisable(GL_LIGHTING);
      glColor4f(1, 1, 1, 1);
      glPushMatrix();
         glRotatef(-90.0,1.0f,0.0f,0.0f);   // Rotate By 0 On The X-Axis
         for (int i = 1; i <= ringsToDraw; i *= 2) {
            gluDisk(quadric, ringDiameter - (ringWidth / 2), ringDiameter + (ringWidth / 2) ,16,2);
            ringDiameter /= 2;
         }
      glPopMatrix();
      glEnable(GL_LIGHTING);

      for (listIter = objects->begin(); listIter != objects->end(); ++listIter) {
         // Make sure it's not null, & then draw it in the minimap
         if (*listIter != NULL && (*listIter)->shouldDrawInMinimap && *listIter != ship) {
            objectPosition = *(*listIter)->position; // Get the position.
            positionVector.movePoint(objectPosition, -1); // Center on the ship
            objectPosition = modelViewMatrix * (objectPosition); // Rotate about the ship
            radius2D = distance2D(objectPosition.x, objectPosition.z);
            radius3D = distance3D(objectPosition.x, objectPosition.y, objectPosition.z);

            if (radius3D < 0.5 * zoomLevel) {
               glColor3f(1, 0, 0);
               setMaterial(RedFlat);
            } else if (radius3D < 0.75 * zoomLevel) {
               glColor3f(1, 1, 0);
               setMaterial(YellowFlat);
            } else {
               glColor3f(0, 1, 0);
               setMaterial(GreenTransparent);
            }

            // Draw a disc.
            // This is the way that one game did it with the circles on a plane.
            /*
            glPushMatrix();
               glEnable(GL_LIGHTING);
               glRotatef(-90.0,1.0f,0.0f,0.0f);   // Rotate By 0 On The X-Axis
               gluDisk(quadric, radius2D - (ringWidth / 2), radius2D + (ringWidth / 2) ,16,2);
               glDisable(GL_LIGHTING);
            glPopMatrix();
            */

            glDisable(GL_LIGHTING);
            glBegin(GL_LINES);
               // Draw a line from the ship to the object's position on the ship's plane.
               glVertex3f(0, 0, 0);
               glVertex3f(objectPosition.x, 0, objectPosition.z);
               
               // Now draw a point from the plane up to the object's position in 3d.
               // Draw a point at the object's position.
               glVertex3f(objectPosition.x, objectPosition.y, objectPosition.z);
               // Draw the other point at the object's position without the Y component.
               glVertex3f(objectPosition.x, 0, objectPosition.z);
            glEnd();
         }
      }
   glPopMatrix();
}

/**
 * This draws the minimap.
 * If displaySize is 0, the minimap is not shown.
 * Otherwise displaySize is how big the map is drawn.
 *    1 is full screen, .25 is quarter screen, etc.
 * zoomLevel is the maximum distance that should be drawn.
 *    40 draws items within 40 units of the ship.
 *    5 draws items within 5 units of the ship.
 */
void Minimap::draw() {
   if (displaySize == 0)
      return;

   // Get a reading of all the nearby Object3Ds from the Radar
   std::list<Object3D*>* objects = ship->getRadar()->getNearbyReading(zoomLevel);
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
      glScalef(2, 2, 2);

      glPushMatrix();
         ship->glRotate(false);
         Vector3D oppositeOfPosition(*(ship->position));

         // Translate everything so that the ship is at 0, 0 and everything is centered there.
         //oppositeOfPosition.updateMagnitude(oppositeOfPosition);
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
               if (radius3D < 0.5 * zoomLevel) {
                  glColor3f(1, 0, 0);
                  setMaterial(RedFlat);
               } else if (radius3D < 0.75 * zoomLevel) {
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
      ship->glRotate(false);
      glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
      glDisable(GL_COLOR_MATERIAL);

      gluSphere(quadric, 1, 10, 10);
      glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
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

/**
 * Adjust the zoom level;
 */
void Minimap::adjustZoom(double changeAmount) {
   targetZoomLevel += changeAmount;
   targetZoomLevel = clamp(targetZoomLevel, MIN_ZOOMLEVEL, MAX_ZOOMLEVEL);
}

/**
 * Adjust the display size.
 */
void Minimap::adjustDisplaySize(double changeAmount) {
   targetDisplaySize += changeAmount;
   targetDisplaySize = clamp(targetDisplaySize, MIN_DISPLAYSIZE, MAX_DISPLAYSIZE);
}

/**
 * Update aspects of the minimap.
 */
void Minimap::update(double timeDiff) {
   const double zoomScale = 40;
   const double changeScale = 0.5;
   adjustZoom(timeDiff * zoomScale * adjustZoomDirection);
   adjustDisplaySize(timeDiff * changeScale * adjustDisplaySizeDirection);

   double zoomIncrement = (targetZoomLevel - zoomLevel) * timeDiff * 2;
   double sizeIncrement = (targetDisplaySize - displaySize) * timeDiff * 10;
   zoomLevel += zoomIncrement;
   displaySize += sizeIncrement;
}

void Minimap::toggle() {
   if (hidden) {
      targetDisplaySize = oldDisplaySize;
      hidden = false;
   } else {
      oldDisplaySize = targetDisplaySize;
      targetDisplaySize = 0;
      hidden = true;
   }
}
