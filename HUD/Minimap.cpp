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
#define MAX_ZOOMLEVEL 140
#define MIN_ZOOMLEVEL 15

Minimap::Minimap(AsteroidShip* _ship) :
 ship(_ship), displaySize(DEFAULT_DISPLAYSIZE), zoomLevel(DEFAULT_ZOOMLEVEL) {
   adjustZoomDirection = adjustDisplaySizeDirection = 0;
   targetDisplaySize = DEFAULT_DISPLAYSIZE;
   targetZoomLevel = DEFAULT_ZOOMLEVEL;
   oldDisplaySize = DEFAULT_DISPLAYSIZE;
   hidden = false;
   autoZoom = true;
   itemsDisplayed = 1;
   totalItems = 1;
   furthestItemDistance = 0;
}

void Minimap::drawLines(std::list<Drawable*>* objects) {
   // Now draw the lines.
   // Load just the rotation matrix.
   static Matrix4 modelViewMatrix;
   const float ringWidth = (zoomLevel / 80) / displaySize;
   const float scaleFactor = 1 / zoomLevel;
   std::list<Drawable*>::iterator listIter;
   static Point3D objectPosition;
   static Vector3D positionVector;
   positionVector.updateMagnitude(*ship->position);

   glPushMatrix();
      glLoadIdentity(); // We don't want the camera transform.
      ship->glRotate(true);
      modelViewMatrix.loadModelviewMatrix();
   glPopMatrix();

   float ringDiameter;

   glPushMatrix();
      glScalef(scaleFactor, scaleFactor, scaleFactor);

      glDisable(GL_LIGHTING);
      float distanceFromEdge;
      float alpha;
      glPushMatrix();
         glRotatef(-90.0,1.0f,0.0f,0.0f);   // Rotate By 0 On The X-Axis
         for (ringDiameter = 16; ringDiameter <= zoomLevel; ringDiameter *= 2) {
            distanceFromEdge = zoomLevel - ringDiameter;
            alpha = (float) clamp(distanceFromEdge / 5, 0, 0.5);
            glColor4f(1, 1, 0, alpha);
            gluDisk(quadric, ringDiameter - (ringWidth / 2), ringDiameter + (ringWidth / 2) ,16,2);
         }
      glPopMatrix();
      glEnable(GL_LIGHTING);

      for (listIter = objects->begin(); listIter != objects->end(); ++listIter) {
         // Make sure it's not null, & then draw it in the minimap
         if (*listIter != NULL && (*listIter)->shouldDrawInMinimap && *listIter != ship) {
            objectPosition = *(*listIter)->position; // Get the position.
            positionVector.movePoint(objectPosition, -1); // Center on the ship
            objectPosition = modelViewMatrix * (objectPosition); // Rotate about the ship

            if ((*listIter)->type == TYPE_ASTEROID3D) {
               glColor3f(1, 0, 0);
            } else if ((*listIter)->type == TYPE_SHARD) {
               glColor3f(0, 1, 0);
            } else if ((*listIter)->type == TYPE_ASTEROIDSHIP) {
               glColor3f(0, 0, 1);
            }

            glDisable(GL_LIGHTING);
            glBegin(GL_LINES);
               // Draw a line from the ship to the object's position on the ship's plane.
               glVertex3f(0, 0, 0);
               glVertex3d(objectPosition.x, 0, objectPosition.z);
               
               // Now draw a point from the plane up to the object's position in 3d.
               // Draw a point at the object's position.
               glVertex3d(objectPosition.x, objectPosition.y, objectPosition.z);
               // Draw the other point at the object's position without the Y component.
               glVertex3d(objectPosition.x, 0, objectPosition.z);
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
   std::list<Drawable*>* objects = ship->getRadar()->getMinimapReading(zoomLevel, totalItems);
   static Vector3D positionVector;
   const float scaleFactor = 1 / zoomLevel;
   double radius3D; // Radius from the ship in 3D space.
   // Used when looping over Object3Ds in drawMinimap
   std::list<Drawable*>::iterator listIter;
 
   // Draw the minimap
   glPushMatrix();
      /* Set the viewport to 3/4 of the way across the screen on the bottom.
       * It should take up the bottom right corner of the screen.
       */

      GLint w0;
      GLint h0 = 0;
      GLsizei w;
      GLsizei h;
      float eyeOffset = 0;

      if (drawStereo_enabled) {
         float displaySizeFactor = 2;
         if (displaySize > 0.5) {
            targetDisplaySize = displaySize = 0.5;
         }
         w = (GLsizei) ((float) gameSettings->GW * displaySize * displaySizeFactor) / 2;
         h = (GLsizei) ((float) gameSettings->GH * displaySize * displaySizeFactor) / 2;
         if (stereo_eye_left) {
            w0 = (gameSettings->GW / 2) + 
               (GLint) ((gameSettings->GW / 2.0) * (1.0f - displaySize * displaySizeFactor));
            eyeOffset = -0.3;
         } else {
            w0 = (GLint) ((gameSettings->GW / 2.0) * (1.0f - displaySize * displaySizeFactor));
            eyeOffset = 0.3;
         }
      } else {
         h = (GLsizei) ((float) gameSettings->GH * displaySize);
         w0 = (GLint) ((float) gameSettings->GW * (1.0f - displaySize));
         w = (GLsizei) ((float) gameSettings->GW * displaySize);
      }

      glViewport (w0, h0, w, h);
      glMatrixMode (GL_PROJECTION);      /* Select The Projection Matrix */
      glLoadIdentity ();                     /* Reset The Projection Matrix */

      // Keep our aspect ratio relative to the global width and height
      gluPerspective(VERT_FOV, (double)gameSettings->GW/gameSettings->GH, 0.5, 20);

      glMatrixMode (GL_MODELVIEW);      /* Select The Projection Matrix */
      glLoadIdentity ();                        /* Reset The Modelview Matrix */

      glClear (GL_DEPTH_BUFFER_BIT);      /* Clear Depth Buffer */

      setMaterial(WhiteSolid);

      // eye, lookAt, and up vectors
      gluLookAt(eyeOffset, 2, 5, eyeOffset, 0, 0,  0, 1, 0);

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

         itemsDisplayed = (int) objects->size();
         furthestItemDistance = 0;
         // For each item that needs to be drawn in the minimap
         for (listIter = objects->begin(); listIter != objects->end(); ++listIter) {

            // Make sure it's not null, & then draw it in the minimap
            // TODO: Make this not copy code from below.
            if (*listIter != NULL) {
               radius3D = ship->position->distanceFrom(*(*listIter)->position);

               // Keep track of the furthest item we're actually drawing.
               if (radius3D > furthestItemDistance) {
                  furthestItemDistance = radius3D;
               }

               (*listIter)->drawInMinimap();
            }
         }
      glPopMatrix();
      drawLines(objects);

      glEnable(GL_CULL_FACE);
   glPopMatrix();
   delete objects;
}

/**
 * Set the display size to a new value. This value should be from
 * 0 (not drawn) to 1 (full screen)
 */
void Minimap::setDisplaySize(float newDisplaySize) {
   displaySize = (float) clamp(newDisplaySize, 0, 1);
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
   const double zoomScale = 80;
   const double changeScale = 0.5;

   const int minItemsToDisplay = 4;
   const int maxItemsToDisplay = 5;
   const float extraSpaceAfterFurthestItem = 8;
   
   if (autoZoom) {
      if ((totalItems > itemsDisplayed && itemsDisplayed < minItemsToDisplay) ||
       (totalItems < maxItemsToDisplay && (zoomLevel - furthestItemDistance) < extraSpaceAfterFurthestItem)) {
         // If there are too many items or we are too zoomed in, zoom out.
         adjustZoomDirection = 1;
      } else if (itemsDisplayed > maxItemsToDisplay ||
       (itemsDisplayed == totalItems &&
        (zoomLevel - furthestItemDistance) > 1.5 * extraSpaceAfterFurthestItem)) {
         // If too many items are shown, zoom in.
         adjustZoomDirection = -1;
      } else {
         adjustZoomDirection = 0;
      }
   }

   adjustZoom(timeDiff * zoomScale * adjustZoomDirection);
   adjustDisplaySize(timeDiff * changeScale * adjustDisplaySizeDirection);

   double zoomIncrement = (targetZoomLevel - zoomLevel) * timeDiff * 2;
   double sizeIncrement = (targetDisplaySize - displaySize) * timeDiff * 10;
   zoomLevel += (float) zoomIncrement;
   displaySize += (float) sizeIncrement;
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

bool Minimap::isEnabled() {
   return (!hidden);
}

void Minimap::toggleAutoZoom() {
   autoZoom = !autoZoom;
}

void Minimap::disableAutoZoom() {
   autoZoom = false;
}
