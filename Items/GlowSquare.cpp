/**
 * GlowSquare is a glowing square on the side.
 * @author Sterling Hirsh
 * @date 2/24/2011
 */

#include "Items/GlowSquare.h"
#include <algorithm>
#include <math.h>

float GlowSquare::lifetime = 2;

GlowSquare::GlowSquare(Color* _color,
 float size, float _x, float _y, float _z, BoundingWall* _wall,
 int _xIndex, int _yIndex) : 
 color(_color), wall(_wall), x(_xIndex), y(_yIndex) {
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
}

void GlowSquare::draw() {
   double timeDiff;
   const double fadeTime = 1;
   const double shwobbleAmplitude = 0.5;
   const int numShwobbles = 1;
   const double fadeScale = 1 / fadeTime;
   timeDiff = (doubleTime() - timeLastHit) / fadeTime;
   
   while (flashTimes.size() > 0 && flashTimes.top() < doubleTime()) {
      timeLastHit = flashTimes.top();
      flashTimes.pop();
      timeDiff = (doubleTime() - timeLastHit) / fadeTime;
   }

   // If the timeLastHit is after the current time, consider it not hit yet.
   alpha = timeDiff < 0 ? 0 : fadeScale * (fadeTime - clamp(timeDiff, 0, fadeTime));

   color->setColorWithAlpha(1);
   glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
   glUseProgram(fadeShader);
   glBegin(GL_QUADS);
   p1.draw();
   p2.draw();
   p3.draw();
   p4.draw();
   glEnd();

   if (alpha != 0) {
      glUseProgram(0);
      glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
      color->setColorWithAlpha(alpha * 0.5);
      glPushMatrix();
      normal.glTranslate(shwobbleAmplitude * sin(numShwobbles * 2 * M_PI * alpha * alpha));
      glBegin(GL_QUADS);
      p1.draw();
      p2.draw();
      p3.draw();
      p4.draw();
      glEnd();
      glPopMatrix();
   }

}

void GlowSquare::update(double timeDiff) {
}

/**
 * This is what happens when you something bounces off a glow square.
 * Delay is added to the timeLastHit so that the ripple effect works.
 */
void GlowSquare::hit(int distanceLimit, double delay) {
   timeLastHit = doubleTime();

   // Now trigger neighbors
   std::vector<GlowSquare*>::iterator iter = wall->squares.begin();

   delay = clamp(delay, 0.08, 0.2);

   int distance;
   //const double delay = 0.1;
   int xDist, yDist;
   for (; iter != wall->squares.end(); ++iter) {
      xDist = abs((*iter)->x - x);
      yDist = abs((*iter)->y - y);
      // Weird
      //distance = sqrt(xDist * xDist * xDist + yDist * yDist * yDist);
      // Circle
      distance = sqrt(xDist * xDist + yDist * yDist);
      // Box
      //distance = std::max(xDist, yDist);
      // Diamond
      // distance = xDist + yDist;
      if (distance <= distanceLimit) {
         (*iter)->flashTimes.push(timeLastHit + (distance * delay));
      }
   }

}
