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
#include "Utility/InputManager.h"
#include <list>

class AsteroidShip;

class GameState : public InputReceiver {
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
      // Used when looping over Object3Ds in drawInMinimap
      std::list<Object3D*>::iterator listIter;
      Custodian custodian;

      GameState(double worldSize);
      virtual ~GameState();
      void draw();
      void drawInMinimap();
      
      void drawAllText();
      void updateText();
      void checkCollisions();
      void update(double timeDiff);
      void initAsteroids();
      void setCurFPS(double fpsIn);
      bool isGameRunning();

      double getMouseX();
      double getMouseY();

      virtual void keyUp(int key);
      virtual void keyDown(int key);
      virtual void mouseDown(int button);
      virtual void mouseMove(int dx, int dy, int x, int y);
      virtual void mouseUp(int button);
   private:
      void reset();
      double curFPS, worldSize;
      bool gameIsRunning;
      bool doYaw;
      double mouseX, mouseY;
      int scoreToWin;
      bool thirdPerson;

      Skybox* skybox;
      Camera* camera;
      BoundingSpace* cube;

      std::list<Object3D*> objects;
};

#endif
