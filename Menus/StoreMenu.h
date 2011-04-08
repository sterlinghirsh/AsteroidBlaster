
#ifndef __STOREMENU_H__
#define __STOREMENU_H__


#include "Menus/Menu.h"
#include "Utility/InputManager.h"

class GameState;

class StoreMenu : public Menu {
   public:
   StoreMenu(GameState*& _gameState);
   ~StoreMenu();

   GameState*& gameState;

   void draw();
   
   void keyUp(int key);
   void keyDown(int key);
   void mouseDown(int button);
   void mouseMove(int dx, int dy, int x, int y);
   void mouseUp(int button);
   void drawLogo();
   void drawTexts(std::vector<Text*> texts);
   
   //the current mouse location
   double x, y;
   
   bool menuActive;
   
   
   std::vector<Text*> menuTexts;
   
   std::vector<Text*> weaponsTexts;
   std::vector<Text*> upgradesTexts;
   std::vector<Text*> ammoTexts;
   std::vector<Text*> shipTexts;
   
   enum StoreMenuEnum { WEAPONS, UPGRADES, AMMO, SHIP };

   int menuSelection;
   
   
};


#endif

