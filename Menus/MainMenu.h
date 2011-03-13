
#ifndef __MAINMENU_H__
#define __MAINMENU_H__


#include "Utility/Text.h"
#include "Utility/InputManager.h"
#include "Menus/Menu.h"

class MainMenu : public Menu {
   public:
   MainMenu();
   ~MainMenu();

   virtual void draw();
   virtual void update();
   
   virtual void keyUp(int key);
   virtual void keyDown(int key);
   virtual void mouseDown(int button);
   virtual void mouseMove(int dx, int dy, int x, int y);
   virtual void mouseUp(int button);
   
   //the current mouse location
   double x, y;
   
   bool menuActive;
   
   
   std::vector<Text*> menuTexts;
   
   
};


#endif

