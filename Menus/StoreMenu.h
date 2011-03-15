
#ifndef __STOREMENU_H__
#define __STOREMENU_H__


#include "Menus/Menu.h"
#include "Utility/InputManager.h"

class StoreMenu : public Menu {
   public:
   StoreMenu();
   ~StoreMenu();

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

