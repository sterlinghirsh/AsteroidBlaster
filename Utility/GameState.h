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
#include "Items/BoundingSphere.h"
#include "Items/Asteroid3D.h"
#include "Graphics/Skybox.h"
#include "Graphics/Sprite.h"
#include "Particles/Particle.h"
#include "Graphics/Camera.h"
#include "Utility/Text.h"
#include "Utility/Custodian.h"
#include "Utility/InputManager.h"
#include "HUD/ProgressBar.h"
#include "HUD/Minimap.h"
#include <list>
#include <sstream>

class AsteroidShip;
class Minimap;

class GameState : public InputReceiver {
   static std::ostringstream sstream2;

   public:
      AsteroidShip* ship;
      /* All of the text objects to be drawn each frame. If you want more text drawn, declare it here,
       * update it in updateText(), and make it draw in drawAllText().
       */
      Text *FPSText,
                        *numAsteroidsText,
                        *numShardsText,
                        *scoreText,
                        *shardText,
                        *healthText,
                        *gameOverText,
                        *winText,
                        *weaponText,
                        *ammoText,
                        *curLevelText;
      // Used when looping over Drawable objects
      std::vector<Drawable*>::iterator item;
      // Used when looping over Drawable objects in drawMinimap
      std::list<Drawable*>::iterator listIter;
      Custodian custodian;

      std::list<Drawable*>* viewFrustumObjects;

      GameState(double worldSize);
      virtual ~GameState();
      void draw();
      void hBlur();
      void vBlur();
      void drawBlur();
      void drawBloom();
      void drawGlow();
      void drawMinimap();

      void drawAllText();
      void drawHud();
      void updateText();
      void checkCollisions();
      void update(double timeDiff);
      void initAsteroids();
      void setCurFPS(double fpsIn);
      bool isGameRunning();
      bool getMenuMode();
      
      double getWallxMin();
      double getWallxMax();
      double getWallyMin();
      double getWallyMax();
      double getWallzMin();
      double getWallzMax();
      
      double getMouseX();
      double getMouseY();
      Camera* getCamera();

      void menuFunc();
      void nextLevel();
      void reset();

      virtual void keyUp(int key);
      virtual void keyDown(int key);
      virtual void mouseDown(int button);
      virtual void mouseMove(int dx, int dy, int x, int y);
      virtual void mouseUp(int button);

      // Bloom
      int xSize, ySize;
      GLuint hTexture;
      GLuint vTexture;
      GLuint blurTex;
      char* colorBits;
      float aspect;

      bool bloom;
      bool bloom1;
      bool reg;
      bool godMode;
   
      ProgressBar* weaponReadyBar;
      ProgressBar* healthBar;
   private:
      //void reset();
      double curFPS, worldSize;
      bool gameIsRunning;
      bool doYaw;
      double mouseX, mouseY;
      int scoreToWin;
      int numAsteroidsToSpawn;
      bool isW, isA, isS, isD;
      int curLevel;

      Skybox* skybox;
      Camera* camera;
      BoundingSpace* cube;
      BoundingSphere* sphere;
      Minimap* minimap;

      std::list<Object3D*> objects;
};

#endif
