
#ifndef __MAINMENU_H__
#define __MAINMENU_H__


#include "Menus/Menu.h"
#include "Utility/InputManager.h"

class MainMenu : public Menu {
   public:
   MainMenu();
   ~MainMenu();

   void draw();
   
   void keyUp(int key);
   void keyDown(int key);
   void mouseDown(int button);
   void mouseMove(int dx, int dy, int x, int y);
   void mouseUp(int button);
   
   //the current mouse location
   double x, y;
   
   bool menuActive;
   bool firstTime;
   
   
   std::vector<Text*> menuTexts;
   
   
};


#endif

