#ifndef __SPRING_H__
#define __SPRING_H__

#include "Items/Object3D.h"
#include "Graphics/Camera.h"

// The length of the springs when fully contracted (MUST BE NON-ZERO).
#define POS_SPRING_LEN 0.2
#define UP_SPRING_LEN 0.01
// The scaling factor of the forces (higher is stronger).
#define POS_FORCE_SCALE 1.0
#define UP_FORCE_SCALE 3.0

class Spring : public Object3D {
   public:
      Spring(const GameState* _gameState);
      ~Spring() {};
      void attach(AsteroidShip* anchorIn, Camera* itemIn);
      void update(double ms);
      AsteroidShip* anchor;
      Camera* item;
      bool isAttached;
};

#endif
