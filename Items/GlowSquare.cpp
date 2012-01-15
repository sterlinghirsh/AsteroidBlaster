/**
 * GlowSquare is a glowing square on the side.
 * @author Sterling Hirsh
 * @date 2/24/2011
 */

#include "Items/GlowSquare.h"
#include <algorithm>
#include <math.h>

#ifdef WIN32
#include "Utility/WindowsMathLib.h"
#endif
   
const double fadeTime = 0.75;
const double shwobbleAmplitude = 0.5;
const int numShwobbles = 1;
const double fadeScale = 1 / fadeTime;

GlowSquare::GlowSquare(Color* _color,
 float size, float _x, float _y, float _z, BoundingWall* _wall,
 int _xIndex, int _yIndex) : 
 color(_color), wall(_wall), x(_xIndex), y(_yIndex) {
   timeLastHit = 0;
   active = false;
   timeSinceLastHit = 999;
   if (wall->wallID % 3 == 0) {
      // Top or bottom
      p1.update(_x, _y, _z);
      p2.update(_x + size, _y, _z);
      p3.update(_x + size, _y, _z + size);
      p4.update(_x, _y, _z + size);
   } else if (wall->wallID % 3 == 1) {
      // Front or back
      p1.update(_x, _y, _z);
      p2.update(_x + size, _y, _z);
      p3.update(_x + size, _y + size, _z);
      p4.update(_x, _y + size, _z);
   } else {
      // Left or right
      p1.update(_x, _y, _z);
      p2.update(_x, _y + size, _z);
      p3.update(_x, _y + size, _z + size);
      p4.update(_x, _y, _z + size);
   }

   switch(wall->wallID) {
      case WALL_TOP: normal.updateMagnitude(0, -1, 0); break;
      case WALL_BOTTOM: normal.updateMagnitude(0, 1, 0); break;
      case WALL_FRONT: normal.updateMagnitude(0, 0, -1); break;
      case WALL_BACK: normal.updateMagnitude(0, 0, 1); break;
      case WALL_LEFT: normal.updateMagnitude(1, 0, 0); break;
      case WALL_RIGHT: normal.updateMagnitude(-1, 0, 0); break;
   }

   midpoint1.midpoint(p1, p2);
   midpoint2.midpoint(p2, p3);
   midpoint3.midpoint(p3, p4);
   midpoint4.midpoint(p4, p1);

   initDisplayList();
}

GlowSquare::~GlowSquare() {
   // Don't delete color, since it's a shared pointer.
}

void GlowSquare::drawLines() {
   /*
   // This draws the empty box. Let's do something more interesting.
   glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
   glBegin(GL_QUADS);
   p1.draw();
   p2.draw();
   p3.draw();
   p4.draw();
   glEnd();
   */

   /*
   p1.draw();
   midpoint1.draw();
   p3.draw();
   midpoint2.draw();
   */
   /*
   midpoint1.draw();
   midpoint2.draw();
   midpoint3.draw();
   midpoint4.draw();
   */
   midpoint1.draw();
   p1.draw();
   midpoint4.draw();
}

void GlowSquare::drawGlowingPart() {
   glBegin(GL_QUADS);
   /*
   midpoint1.draw();
   midpoint2.draw();
   midpoint3.draw();
   midpoint4.draw();
   */
   /*
   p1.draw();
   p2.draw();
   p3.draw();
   p4.draw();
   */

   p1.draw();
   midpoint1.draw();
   p3.draw();
   midpoint3.draw();

   glEnd();
}

void GlowSquare::draw() {
   // This draws the glowing filled in square.
   if (timeSinceLastHit < fadeTime) {
      glPushMatrix();
      // If the timeLastHit is after the current time, consider it not hit yet.
      alpha = (fadeScale * (fadeTime - clamp(timeSinceLastHit, 0, fadeTime)));
      //glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
      // Max alpha is 0.5
      color->setColorWithAlpha((float) (alpha * 0.5));
      normal.glTranslate(shwobbleAmplitude * sin(numShwobbles * 2 * M_PI * alpha * alpha));
      glCallList(displayList);
      glPopMatrix();
   }

}

void GlowSquare::drawGlow() {
   draw();
}

/**
 * Update method for glowsquare.
 * TODO: Make a priority queue of time, square pairs.
 * Store the appropriate times in a texture or something.
 * Handle the glow squares in a shader, and only update the texture when a new
 * value is the next timeLastHit.
 */
void GlowSquare::update(double timeDiff) {
   // Set the timeLastHit to the latest time that is <= the current time.
   
   timeSinceLastHit = (doubleTime() - timeLastHit) / fadeTime;
   while (!flashTimes.empty() && flashTimes.top() <= doubleTime()) {
      timeLastHit = flashTimes.top();
      flashTimes.pop();
      timeSinceLastHit = (doubleTime() - timeLastHit) / fadeTime;
   }

   if (flashTimes.empty() && timeSinceLastHit > fadeTime) {
      // Deactive the square.
      active = false;
   }
}

/**
 * This is what happens when you something bounces off a glow square.
 * Delay is added to the timeLastHit so that the ripple effect works.
 */
void GlowSquare::hit(int distanceLimit, double delay) {
   timeLastHit = doubleTime();

   // Now trigger neighbors
   GlowSquare* otherSquare;

   delay = clamp(delay, 0.1, 0.2);

   int distance;
   int xDist, yDist;
   
   int maxX = x + distanceLimit;
   int maxY = y + distanceLimit;

   // Iterate over just the squares we need so we don't wate time.
   for (int curX = x - distanceLimit; curX <= maxX; ++curX) {
      if (curX < 0)
         continue;
      if (curX > wall->squaresPerSide)
         break;

      for (int curY = y - distanceLimit; curY <= maxY; ++curY) {
         if (curY < 0)
            continue;
         if (curY > wall->squaresPerSide)
            break;

         otherSquare = wall->getSquareByCoords(curX, curY);
         if (otherSquare == NULL) {
            continue;
         }
         xDist = abs(otherSquare->x - x);
         yDist = abs(otherSquare->y - y);
         // Weird
         //distance = sqrt(xDist * xDist * xDist + yDist * yDist * yDist);
         // Circle
         distance = (int) sqrt((double)(xDist * xDist + yDist * yDist));
         // Box
         //distance = std::max(xDist, yDist);
         // Diamond
         // distance = xDist + yDist;
         if (distance <= distanceLimit) {
            otherSquare->flashTimes.push(timeLastHit + (distance * delay));

            // Activate the square.
            if (!otherSquare->active) {
               otherSquare->active = true;
               wall->activeSquares.push_back(otherSquare);
            }
         }
      }
   }
}

void GlowSquare::initDisplayList() {
   displayList = glGenLists(1);
   glNewList(displayList, GL_COMPILE);
   drawGlowingPart();
   glEndList();
}
