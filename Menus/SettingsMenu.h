
#ifndef __SETTINGSMENU_H__
#define __SETTINGSMENU_H__


#include "Menus/Menu.h"
#include "Utility/InputManager.h"

class SettingsMenu : public Menu {
   public:
   SettingsMenu();
   ~SettingsMenu();

   std::string getStatus(bool status);
   std::string getViewStatus(int status);
   void draw();
   
   void keyUp(int key);
   void keyDown(int key);
   void mouseDown(int button);
   void mouseMove(int dx, int dy, int x, int y);
   void mouseUp(int button);
   
   void activate();
   void deactivate();
   void newGameDeactivate();
   
   //the current mouse location
   double x, y;
   
   bool menuActive;
   bool firstTime;
   
   
   std::vector<Text*> menuTexts;
   std::vector<int> types;
   
   
};


#endif

