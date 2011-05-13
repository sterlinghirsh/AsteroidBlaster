/**
 * A spring. Connects a camera to a ship, but it can be restructured to
 * use two Object3D's, or really pretty much any two objects.
 * @author Chris Brenton
 * @date 4/19/11
 */
#ifndef __SPRING_H__
#define __SPRING_H__

#include "Items/Object3D.h"
#include "Graphics/Camera.h"

class AsteroidShip;

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
