/**
 * Cube constrains objects' motion.
 * Sterling Hirsh / Taylor Arnicar
 * 1-20-11
 * CPE 476
 */

#ifndef __BOUNDINGSPACE_H__
#define __BOUNDINGSPACE_H__

#include "Utility/Point3D.h"
#include "Items/Object3D.h"
#include "Items/BoundingWall.h"
#include <list>
#include <vector>

class GameState;

class BoundingSpace {
   public:
      double extent; // Only one var instead of min/max x, y, z
      double xMax, xMin, yMax, yMin, zMax, zMin;

      BoundingSpace(double extentIn, double x, double y, double z, const GameState* _gameState);
      virtual ~BoundingSpace();
      virtual void constrain(Drawable* item);
      void draw();
      void drawGlow();
      void update(double timeDiff);
      int getNumSquares();
   private:
      std::list<GlowSquare*> glowingSquares;
      std::vector<GlowSquare*> squares;
      float squareSize;
      Color* wallColors[6];
      BoundingWall* walls[6];
};

#endif
