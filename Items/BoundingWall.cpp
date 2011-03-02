/**
 * Bounding Wall: It's a wall!
 *
 * @author Sterling Hirsh
 * @date 3/1/20599/2
 */

#include "Items/BoundingWall.h"
#include <math.h>

BoundingWall::BoundingWall(double _squareSize, int _wallSize, Color* _wallColor, int _wallID) :
 squareSize(_squareSize), wallSize(_wallSize), wallColor(_wallColor), wallID(_wallID) {
   // Initialize Wall Squares
   numSquares = round(2 * wallSize / squareSize);
   squares.reserve(numSquares); 

   /* These three variables dictate where this will be positioned relative
    * to the plane. The X and Y are the coords in the plane.
    * The Z will be the constant value for this plane.
    */
   float squareX, squareY, squareZ;
   float minX, minY;

   // If wall is top, front, or right
   squareZ = wallID <= WALL_RIGHT ? wallSize : -wallSize;
   
   // These will probably always be the same.
   minX = minY = -wallSize;
   // Create the squares.
   for (int y = 0; y < numSquares; ++y) {
      for (int x = 0; x < numSquares; ++x) {
         squareX = minX + x * squareSize;
         squareY = minY + y * squareSize;

         if (wallID % 3 == 0) {
            // If wall is top or bottom.
            squares.push_back(new GlowSquare(wallColor, squareSize, 
             squareX, squareZ, squareY, wallID));    
         } else if (wallID % 3 == 1) {
            // If wall is Front or Back.
            squares.push_back(new GlowSquare(wallColor, squareSize, 
             squareX, squareY, squareZ, wallID));    
         } else {
            // If wall is Left or Right.
            squares.push_back(new GlowSquare(wallColor, squareSize, 
             squareZ, squareX, squareY, wallID));    
         }
      }
   }
   
}

int BoundingWall::getSquareX(int squareID) {
   return squareID % wallSize;
}

int BoundingWall::getSquareY(int squareID) {
   return squareID / wallSize;
}

int BoundingWall::getSquareID(int squareX, int squareY) {
   return (squareY * wallSize) + squareX;
}

int BoundingWall::getSquareIDFromObject(Object3D* item) {
   float squareX, squareY;
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

   int squareXIndex = floor((squareX + wallSize) / squareSize);
   int squareYIndex = floor((squareY + wallSize) / squareSize);
   return getSquareID(squareXIndex, squareYIndex);
}

GlowSquare* BoundingWall::getSquareByID(int index) {
   if (index < 0 || index >= squares.size())
      return NULL;
   return squares[index];
}

void BoundingWall::constrain(Object3D* item) {
   /*
   glowingSquares.push_back(new GlowSquare(wallColor, squareSize, 
    xMax, item->position->y, item->position->z));
    */
   int index = getSquareIDFromObject(item);
   GlowSquare* square = getSquareByID(index);
   if (square == NULL) {
      return;
   }

   square->hit();
}

void BoundingWall::draw() {
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
