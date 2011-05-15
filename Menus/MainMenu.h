
#ifndef __MAINMENU_H__
#define __MAINMENU_H__


#include "Menus/Menu.h"
#include "Utility/InputManager.h"

class AsteroidShip;
class GameState;

class MainMenu : public Menu {
   public:
   MainMenu(GameState* _mainGameState);
   virtual ~MainMenu();

   Text *newGameText,
        *continueText,
        *loadGameText,
        *saveGameText,
        *settingsText,
        *helpText,
        *creditsText,
        *quitText;
   
   
   void draw();
   //void draw(GameState* gameState);
   
   void keyUp(int key);
   void keyDown(int key, int unicode);
   void mouseDown(int button);
   void mouseMove(int dx, int dy, int x, int y);
   void mouseUp(int button);
   
   void activate();
   void deactivate();
   void gameDeactivate(bool shouldLoad = false);

   void setupShips();

   virtual void update(double timeDiff);
   
   //the current mouse location
   int x, y;
   
   bool menuActive;
   bool firstTime;
   
   std::vector<Text*> menuTexts;

   AsteroidShip* ship1;
   AsteroidShip* ship2;
   AsteroidShip* ship3;
   AsteroidShip* ship4;
   
   GameState* menuGameState;
   GameState* mainGameState;
};


#endif

