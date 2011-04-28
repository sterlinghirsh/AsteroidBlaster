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
#include "Items/Ball.h"
#include "Items/Spring.h"
#include "Graphics/Skybox.h"
#include "Graphics/Sprite.h"
#include "Particles/Particle.h"
#include "Graphics/Camera.h"
#include "Text/Text.h"
#include "Utility/Custodian.h"
#include "Utility/InputManager.h"
#include "Utility/GlobalUtility.h"
#include "HUD/ProgressBar.h"
#include "HUD/Minimap.h"
#include "HUD/Screen.h"
#include <list>
#include <sstream>

class AsteroidShip;
class Minimap;

class GameState : public InputReceiver {
   static std::ostringstream sstream2;

   public:
      AsteroidShip* ship;
      Ball* ball;
      Spring* spring;
      
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
                        *curLevelText,
                        *timerText;
      // Used when looping over Drawable objects
      std::vector<Drawable*>::iterator item;
      // Used when looping over Drawable objects in drawMinimap
      std::list<Drawable*>::iterator listIter;
      Custodian custodian;

      std::list<Drawable*>* viewFrustumObjects;
      std::list<Drawable*>* targetableViewFrustumObjects;

      GameState(double worldSize, bool _inMenu);
      virtual ~GameState();
      void draw();
      void hBlur();
      void vBlur();
      void drawBlur();
      void drawBloom();
      void drawGlow();
      void drawMinimap();
      void drawScreens();
      void addScreens();

      bool minimapOn();
      void toggleMinimap();

      void drawAllText();
      void drawHud();
      void updateText();
      int minutesRemaining(double secondsRemaining);
      int secondsRemainder(double secondsRemaining);

      void update(double timeDiff);
      void initAsteroids();
      void setCurFPS(double fpsIn);
      bool getMenuMode();
      
      double getWallxMin() const;
      double getWallxMax() const;
      double getWallyMin() const;
      double getWallyMax() const;
      double getWallzMin() const;
      double getWallzMax() const;
      
      double getMouseX();
      double getMouseY();
      Camera* getCamera();

      void menuFunc();
      void nextLevel();
      void reset();

      void addAIPlayer();

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

      bool godMode;
      bool gameIsRunning;
      bool inMenu;
   
      ProgressBar* weaponReadyBar;
      ProgressBar* healthBar;
      double curFPS, worldSize;
      double lastDrawTime;
      
      Camera* camera;
   private:
      //void reset();
      
      bool doYaw;
      double mouseX, mouseY;
      double shipControlX, shipControlY;
      int scoreToWin;
      int numAsteroidsToSpawn;
      bool isW, isA, isS, isD;
      int curLevel;
      // Used for a countdown at the end of each level.
      double countDown;


      // Timer to control how long the level lasts.
      Timer levelTimer;

      // The number of seconds this level will last.
      double levelDuration;

      Skybox* skybox;
      //Camera* camera;
      BoundingSpace* cube;
      BoundingSphere* sphere;
      Minimap* minimap;
      Screen* bloomScreen;
      Screen* rawScreen;
      Screen* fboScreen;

      std::list<Object3D*> objects;
      void addLevelMessage();
      void debugPosition();
};

#endif
