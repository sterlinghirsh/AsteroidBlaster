
#ifndef __CREDITMENU_H__
#define __CREDITMENU_H__


#include "Utility/Text.h"
#include "Utility/InputManager.h"
#include "Menus/Menu.h"

class CreditsMenu : public Menu {
   public:
   CreditsMenu();
   ~CreditsMenu();

   void draw();
   
   void keyUp(int key);
   void keyDown(int key);
   void mouseDown(int button);
   void mouseMove(int dx, int dy, int x, int y);
   void mouseUp(int button);
   
   //the current mouse location
   double x, y;
   
   bool menuActive;
   
   
   std::vector<Text*> menuTexts;
   
   
};


#endif

