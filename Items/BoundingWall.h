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
#include "Items/Drawable.h"
#include <vector>

class Object3D;
class GlowSquare;
class GameState;

class BoundingWall {
   public:
      void constrain(Drawable* item);
      Color* wallColor;
      int wallID;
      int wallSize;
      std::vector<GlowSquare*> squares;
      int numSquares;
      int squareSize;
      int squaresPerSide;

      BoundingWall(int _squareSize, int _wallSize, Color* _wallColor, int _wallID, const GameState* _gameState);
      virtual ~BoundingWall();
      void draw();
      void drawGlow();
      void update(double timeDiff);
      int getSquareX(int squareID);
      int getSquareY(int squareID);
      int getSquareID(int squareX, int squareY);
      void getSquareCoordsFromPoint(Point3D& item, int& squareXIndex, int& squareYIndex);
      GlowSquare* getSquareByID(unsigned index);
      GlowSquare* getSquareByCoords(int x, int y);
      virtual void initDisplayList(); // Set up the displayList.
      Vector3D normal;
      bool actuallyHit;
   private:
      int linesDisplayList;
      const GameState* gameState;
};

#endif
