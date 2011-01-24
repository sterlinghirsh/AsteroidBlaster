/**
 * GameState - Stores the state of the game.
 * Lists of objects, etc.
 * @author Sterling Hirsh
 * @date 1-23-11
 * CPE 476
 */

#ifndef __GAMESTATE_H__
#define __GAMESTATE_H__

#include "Items/AsteroidShip.h"
#include "Items/BoundingSpace.h"
#include "Items/Asteroid3D.h"
#include "Graphics/Skybox.h"
#include "Graphics/Sprite.h"
#include "Graphics/Camera.h"
#include "Utility/BitmapTextDisplay.h"
#include <list>

class GameState {
   public:
      double curFPS, worldSize;
      AsteroidShip* ship;
      BoundingSpace* cube;
      Camera* camera;
      Skybox* skybox;
      std::list<Object3D*> objects;
      std::list<Asteroid3D*> asteroids;
      BitmapTextDisplay *FPStext, 
                        *objectsText, 
                        *objectsCollidedText, 
                        *scoreText, 
                        *timeText, 
                        *gameOverText;
      std::list<Asteroid3D*>::iterator asteroid;

      GameState(double worldSize);
      virtual ~GameState();
      void draw();
      void drawAsteroids();
      void drawAllText();
      void updateText();
      void checkCollisions();
      void update(double timeDiff);
      void initAsteroids();

};

#endif
