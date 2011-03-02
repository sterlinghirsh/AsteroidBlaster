/**
 * Bounding Wall: It's a wall!
 *
 * @author Sterling Hirsh
 * @date 3/1/20599/2
 */

#ifndef __BOUNDINGWALL_H__
#define __BOUNDINGWALL_H__

#define WALL_TOP 0
#define WALL_FRONT 1
#define WALL_RIGHT 2
#define WALL_BOTTOM 3
#define WALL_BACK 4
#define WALL_LEFT 5


#include "Items/GlowSquare.h"
#include "Items/Object3D.h"
#include <vector>

class Object3D;

class BoundingWall {
   public:
      void constrain(Object3D* item);
      Color* wallColor;
      int wallID;
      int wallSize;
      std::vector<GlowSquare*> squares;
      int numSquares;
      double squareSize;

      BoundingWall(double _squareSize, int _wallSize, Color* _wallColor, int _wallID);
      void draw();
      void update(double timeDiff);
      int getSquareX(int squareID);
      int getSquareY(int squareID);
      int getSquareID(int squareX, int squareY);
      int getSquareIDFromObject(Object3D* item);
      GlowSquare* getSquareByID(int index);
};

#endif
