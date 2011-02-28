
#ifndef __MENU_H__
#define __MENU_H__


#include "Utility/BitmapTextDisplay.h"
#include "Utility/InputManager.h"

class Menu : public InputReceiver {
   public:
   Menu();
   ~Menu();

   void draw();
   void update();
   bool isActive();
   
   virtual void keyUp(int key);
   virtual void keyDown(int key);
   virtual void mouseDown(int button);
   virtual void mouseMove(int dx, int dy, int x, int y);
   virtual void mouseUp(int button);
   
   //the current mouse location
   double x, y;
   
   bool menuActive;
   
   
   //glut text display
   BitmapTextDisplay* newGame;
   BitmapTextDisplay* saveLoadGame;
   BitmapTextDisplay* settings;
   BitmapTextDisplay* quit;
   
   SDL_Surface *titleImage;
   SDL_Rect src, dest;
};


#endif

