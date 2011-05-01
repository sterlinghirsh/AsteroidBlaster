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
#include "HUD/WeaponDisplay.h"
#include "HUD/Minimap.h"
#include "HUD/Screen.h"
#include <list>
#include <sstream>

class AsteroidShip;
class Minimap;

class GameState : public InputReceiver {
   //public variables------------------------------
   public:
      static std::ostringstream sstream2;
      
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
                        
                        
                        
      Custodian custodian;
      AsteroidShip* ship;
      Ball* ball;
      Spring* spring;
      // Used when looping over Drawable objects
      std::vector<Drawable*>::iterator item;
      // Used when looping over Drawable objects in drawMinimap
      std::list<Drawable*>::iterator listIter;

      std::list<Drawable*>* viewFrustumObjects;
      std::list<Drawable*>* targetableViewFrustumObjects;
      
      // Bloom
      float aspect;
      
      bool godMode;
      bool gameIsRunning;
      bool inMenu;
      bool levelOver;
   
      ProgressBar* weaponReadyBar;
      ProgressBar* healthBar;
      WeaponDisplay* weaponBar;
      double curFPS, worldSize;
      double lastDrawTime;
      
   //private variables------------------------------
   private:
   
      bool usingShipCamera;
      Camera* shipCamera;
      Camera* spectatorCamera;
      double spectatorSpeed;
      double spectatorRadius;
      
      bool doYaw;
      double mouseX, mouseY;
      double shipControlX, shipControlY;
      int numAsteroidsToSpawn;
      bool isW, isA, isS, isD;
      int curLevel;
      // Used for a countdown at the end of each level.
      double countDown;
      // Used to tell whether or not we already have a countdown on screen.
      bool isCountingDown;

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
   
   
   //public functions------------------------------
   public:
      GameState(double worldSize, bool _inMenu);
      virtual ~GameState();
      
      // virtual functions required by InputReciever
      virtual void keyUp(int key);
      virtual void keyDown(int key);
      virtual void mouseDown(int button);
      virtual void mouseMove(int dx, int dy, int x, int y);
      virtual void mouseUp(int button);
      
      void draw(bool drawGlow = false);
      void hBlur();
      void vBlur();
      void drawBlur();
      void drawBloom();
      
      void drawMinimap();
      void drawScreens();
      void addScreens();

      bool minimapOn();
      void toggleMinimap();
      void pauseLevelTimer();
      void resumeLevelTimer();

      void drawAllText();
      void drawHud();
      void updateText();

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
      Camera* getCurrentCamera() const;

      void menuFunc();
      void nextLevel();
      void reset(bool shouldLoad = false);
      
      void setLevelTimer();
      
      void addAIPlayer();
      
      void serialize(std::ostream &oss);
      void deserialize(std::istream &iss);
      
      void save();
      void load();
   
   //private functions------------------------------
   private:
      void addLevelMessage();
      void debugPosition();
      void spectatorCameraUpdate(double timeDiff);
};

#endif
