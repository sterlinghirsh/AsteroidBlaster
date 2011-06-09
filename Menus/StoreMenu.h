
#ifndef __STOREMENU_H__
#define __STOREMENU_H__


#include "Menus/Menu.h"

class GameState;

class StoreMenu : public Menu {
   enum StoreMenuEnum { WEAPONS, AMMO, SHIP };
   
   //public variables------------------------------
   public:
      bool menuActive;
      
   //private variables------------------------------
   private:
      GameState*& gameState;
      //the current mouse location
      double x, y;
      int menuSelection;
      
      Text *doneText,
           *shardsText,
           *maxHealthText,
           *lifeText,
           *timerText,
           *weaponsText,
           *ammoText,
           *shipText,
           *engineShipText,
           *maxHealthShipText,
           *regenHealthShipText,
           *buyOneLifeText,
           *bankShipText;
      
      //list of strings...
      std::vector<Text*> menuTexts;
      std::vector<Text*> weaponsTexts;
      std::vector<Text*> ammoTexts;
      std::vector<Text*> shipTexts;
   
   //public functions------------------------------
   public:
      //Constructor
      StoreMenu(GameState*& _gameState);
      //Destructor
      virtual ~StoreMenu();
      
      //called from the outside
      void draw();
      void update(double dt);
      void clearOverlay();
      
      //virtual functions required by InputReciever
      virtual void keyUp(int key);
      virtual void keyDown(int key, int unicode);
      virtual void mouseDown(int button);
      virtual void mouseMove(int dx, int dy, int x, int y);
      virtual void mouseUp(int button);
      
   //private functions------------------------------
   private:
      void handleLevelTimer();
      void drawLogo();
      void drawTexts(std::vector<Text*> texts);
      // Common function used when the player clicks Done or presses (n)
      void donePressed();
};


#endif

