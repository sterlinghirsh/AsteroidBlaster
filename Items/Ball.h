#ifndef __BALL_H__
#define __BALL_H__

#include "Items/Object3D.h"

class Ball : public Object3D {
   public:
      Ball(const GameState* _gameState);
      Ball(double x, double y, double z, const GameState* _gameState);
      ~Ball() {};
      void attach(Object3D* anchorIn);
      void draw();
      Object3D* anchor;
};

#endif
