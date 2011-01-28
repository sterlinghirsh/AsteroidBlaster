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
#include "Utility/Custodian.h"
#include <list>

class GameState {
   public:
      AsteroidShip* ship;
      /* All of the text objects to be drawn each frame. If you want more text drawn, declare it here,
       * update it in updateText(), and make it draw in drawAllText().
       */
      BitmapTextDisplay *FPSText, 
                        *numAsteroidsText, 
                        *scoreText, 
                        *healthText,
                        *gameOverText,
                        *winText;
      // Used when looping over Object3Ds
      std::vector<Object3D*>::iterator item;
      Custodian custodian;

      GameState(double worldSize);
      virtual ~GameState();
      void draw();
      //void drawAsteroids();
      void drawAllText();
      void updateText();
      void checkCollisions();
      void update(double timeDiff);
      void initAsteroids();
      void setCurFPS(double fpsIn);
      bool isGameRunning();
   private:
      double curFPS, worldSize;
      bool gameIsRunning;

      Skybox* skybox;
      Camera* camera;
      BoundingSpace* cube;

      std::list<Object3D*> objects;
      std::list<Asteroid3D*> asteroids;
};

#endif
