/**
 * GameState - Stores the state of the game.
 * Lists of objects, etc.
 * @author Sterling Hirsh
 * @date 1-23-11
 * CPE 476
 */

#ifndef __GAMESTATE_H__
#define __GAMESTATE_H__

#include "Utility/Custodian.h"
#include "Utility/InputManager.h"
#include "Network/ClientCommand.h"

#include <list>

class Camera;
class Skybox;
class BoundingSpace;
class AsteroidShip;
class Minimap;
class Spring;
class Text;
class ProgressBar;
class WeaponDisplay;
class Screen;
class UDP_Server;
class UDP_Client;

enum GameStateMode { SingleMode, MenuMode, ClientMode, ServerMode };

namespace boost {
   class thread;
   
   namespace asio {
      class io_service;
   }
}

class GameState : public InputReceiver {
   //public variables------------------------------
   public:
      /* All of the text objects to be drawn each frame. If you want more text drawn, declare it here,
       * update it in updateText(), and make it draw in drawAllText().
       */
      Text *FPSText,
           *scoreText,
           *shardText,
           //*weaponText,
           //*ammoText,
           *curLevelText,
           *timerText,
           *gameOverText,
           *winText;

      Custodian custodian;
      AsteroidShip* ship;
      Spring* spring;
      // Used when looping over Drawable objects
      std::vector<Object3D*>::iterator item;
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
      bool drawGraphics;
   
      ProgressBar* weaponReadyBar;
      ProgressBar* healthBar;
      WeaponDisplay* weaponBar;
      double curFPS, worldSize;
      double lastDrawTime;
      ClientCommand clientCommand;
      
      BoundingSpace* cube;

      GameStateMode gsm;

      //server stuff
      UDP_Server* udpServer;
      UDP_Client* udpClient;
      boost::thread* networkThread;
      boost::asio::io_service* io;
      
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
      Minimap* minimap;
      std::vector<Screen> screens;

      std::list<Object3D*> objects;
   
   
   //public functions------------------------------
   public:
      GameState(GameStateMode _gsm);
      virtual ~GameState();
      
      // virtual functions required by InputReciever
      virtual void keyUp(int key);
      virtual void keyDown(int key, int unicode);
      virtual void mouseDown(int button);
      virtual void mouseMove(int dx, int dy, int x, int y);
      virtual void mouseUp(int button);
      
      void draw();
      void update(double timeDiff);

      // Update functions of various kinds
      void updateText();
      void networkUpdate(double timeDiff);

      // Draw functions of various kinds
      void drawObjects(bool drawGlow = false);
      void drawScreens();
      void hBlur();
      void vBlur();
      void drawBlur();
      void drawBloom();
      void drawMinimap();
      void drawAllText();
      void drawHud();
      void drawOverlay();

      void addScreens();

      bool minimapOn();
      void toggleMinimap();
      void pauseLevelTimer();
      void resumeLevelTimer();


      

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
      void addNetworkPlayer(unsigned clientID);
      
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
