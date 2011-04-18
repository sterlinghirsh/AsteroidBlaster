#ifndef __SPRING_H__
#define __SPRING_H__

#include "Items/Object3D.h"
#include "Graphics/Camera.h"

// The length of the spring when fully contracted (MUST BE NON-ZERO).
#define SPRING_LEN 0.2
// The scaling factor of the force (higher is stronger).
#define FORCE_SCALE 1.0

class Spring : public Object3D {
   public:
      Spring(const GameState* _gameState);
      ~Spring() {};
      void attach(AsteroidShip* anchorIn, Camera* itemIn);
      void update(double ms);
      void draw();
      AsteroidShip* anchor;
      Camera* item;
      bool isAttached;
};

#endif
