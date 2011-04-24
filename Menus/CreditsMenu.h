
#ifndef __CREDITMENU_H__
#define __CREDITMENU_H__


#include "Menus/Menu.h"
#include "Utility/InputManager.h"

#define SCROLL_RATE -0.000001

class CreditsMenu : public Menu {
   public:
   CreditsMenu();
   virtual ~CreditsMenu();

   void addSingleColName(std::string name);
   void addDualColName(std::string name);
   void addTitle(std::string name);

   void draw();
   void update(double ms);
   
   void keyUp(int key);
   void keyDown(int key);
   void mouseDown(int button);
   void mouseMove(int dx, int dy, int x, int y);
   void mouseUp(int button);
   
   //the current mouse location
   double x, y;
   
   bool menuActive;
   bool firstDrawn;
   double lastTick;
   
   double yOffset;
   
   std::vector<Text*> menuTexts;
   std::vector<int> types;
   
   
};


#endif

