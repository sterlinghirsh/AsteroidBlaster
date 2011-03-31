/**
 * Bounding Wall: It's a wall!
 *
 * @author Sterling Hirsh
 * @date 3/1/20599/2
 */

#include "Items/BoundingWall.h"
#include "Items/Object3D.h"
#include "Utility/GameState.h"
#include <math.h>

#ifdef WIN32
#include "Utility/WindowsMathLib.h"
#endif

BoundingWall::BoundingWall(int _squareSize, int _wallSize, Color* _wallColor, int _wallID) :
 squareSize(_squareSize), wallSize(_wallSize), wallColor(_wallColor), wallID(_wallID) {
   // Initialize Wall Squares
   squaresPerSide = (int) round(2 * wallSize / squareSize);
   numSquares = squaresPerSide * squaresPerSide;
   squares.reserve(numSquares); 

   /* These three variables dictate where this will be positioned relative
    * to the plane. The X and Y are the coords in the plane.
    * The Z will be the constant value for this plane.
    */
   float squareX, squareY, squareZ;
   double minX, minY;

   // If wall is top, front, or right
   squareZ = (float) (wallID <= WALL_RIGHT ? wallSize : -wallSize);
   
   // These will probably always be the same.
   minX = minY = -wallSize;
   // Create the squares.
   for (int y = 0; y < squaresPerSide; ++y) {
      for (int x = 0; x < squaresPerSide; ++x) {
         squareX = (float) (minX + (x * squareSize));
         squareY = (float) (minY + (y * squareSize));

         if (wallID % 3 == 0) {
            // If wall is top or bottom.
            squares.push_back(new GlowSquare(wallColor, (float) squareSize, 
             squareX, squareZ, squareY, this, x, y));
         } else if (wallID % 3 == 1) {
            // If wall is Front or Back.
            squares.push_back(new GlowSquare(wallColor, (float) squareSize, 
             squareX, squareY, squareZ, this, x, y));
         } else {
            // If wall is Left or Right.
            squares.push_back(new GlowSquare(wallColor, (float) squareSize, 
             squareZ, squareX, squareY, this, x, y));
         }
      }
   }
   initDisplayList();
   switch(wallID) {
      case WALL_TOP: normal.updateMagnitude(0, -1, 0); break;
      case WALL_BOTTOM: normal.updateMagnitude(0, 1, 0); break;
      case WALL_FRONT: normal.updateMagnitude(0, 0, -1); break;
      case WALL_BACK: normal.updateMagnitude(0, 0, 1); break;
      case WALL_LEFT: normal.updateMagnitude(1, 0, 0); break;
      case WALL_RIGHT: normal.updateMagnitude(-1, 0, 0); break;
   }
}

BoundingWall::~BoundingWall() {
   for (int i = 0; i < squares.size(); ++i) {
      delete squares[i];
   }
   
}

int BoundingWall::getSquareX(int squareID) {
   return squareID % squaresPerSide;
}

int BoundingWall::getSquareY(int squareID) {
   return squareID / squaresPerSide;
}

int BoundingWall::getSquareID(int squareX, int squareY) {
   return (squareY * squaresPerSide) + squareX;
}

GlowSquare* BoundingWall::getSquareByCoords(int x, int y) {
   return getSquareByID(getSquareID(x, y));
}

void BoundingWall::getSquareCoordsFromObject(Drawable* item, int& squareXIndex, int& squareYIndex) {
   double squareX, squareY;
   if (wallID == WALL_TOP || wallID == WALL_BOTTOM) {
      squareX = item->position->x;
      squareY = item->position->z;
   } else if (wallID == WALL_FRONT || wallID == WALL_BACK) {
      squareX = item->position->x;
      squareY = item->position->y;
   } else {
      squareX = item->position->y;
      squareY = item->position->z;
   }

   squareXIndex = (int) floor((squareX + wallSize) / squareSize);
   squareYIndex = (int) floor((squareY + wallSize) / squareSize);
}

GlowSquare* BoundingWall::getSquareByID(unsigned index) {
   if (index < 0 || index >= squares.size())
      return NULL;
   return squares[index];
}

void BoundingWall::constrain(Drawable* item) {
   int x, y;
   getSquareCoordsFromObject(item, x, y);
   GlowSquare* square = getSquareByCoords(x, y);
   if (square == NULL) {
      return;
   }

   item->hitWall(this);

   double speed = item->velocity->getLength();
   // How far out will the ripple effect go?
   int distanceLimit = (int) (item->radius * item->radius * 0.3) + 4;
   // How much time will there be between stages of the ripple effect?
   double delay = 1 / (4 * speed);

   square->hit(distanceLimit, delay);

}

void BoundingWall::draw() {
   Point3D cameraPosition(*gameState->ship->position);
   gameState->ship->getCameraOffset()->movePoint(cameraPosition);
   switch (wallID) {
      case WALL_TOP:
         if (cameraPosition.y > wallSize) return; break;
      case WALL_BOTTOM:
         if (-cameraPosition.y > wallSize) return; break;
      case WALL_LEFT:
         if (-cameraPosition.x > wallSize) return; break;
      case WALL_RIGHT:
         if (cameraPosition.x > wallSize) return; break;
      case WALL_FRONT:
         if (cameraPosition.z > wallSize) return; break;
      case WALL_BACK:
         if (-cameraPosition.z > wallSize) return; break;
   }
   
   glCallList(linesDisplayList);
   std::vector<GlowSquare*>::iterator square;
   for (square = squares.begin(); square != squares.end();
    ++square) {
      (*square)->draw();   
   }
}

void BoundingWall::drawGlow() {
   std::vector<GlowSquare*>::iterator square;
   for (square = squares.begin(); square != squares.end();
    ++square) {
      (*square)->draw();   
   }
}

void BoundingWall::update(double timeDiff) {
   std::vector<GlowSquare*>::iterator square;
   for (square = squares.begin(); square != squares.end();
    ++square) {
      (*square)->update(timeDiff);
   }
}

void BoundingWall::initDisplayList() {
   linesDisplayList = glGenLists(1);
   glNewList(linesDisplayList, GL_COMPILE);
   // Draw the lines here.
   glUseProgram(fadeShader);
   wallColor->setColorWithAlpha(1);

   std::vector<GlowSquare*>::iterator square;
   for (square = squares.begin(); square != squares.end();
    ++square) {
      glBegin(GL_LINE_STRIP);
      (*square)->drawLines();
      glEnd();
   }

   glUseProgram(0);
   glEndList();
}
