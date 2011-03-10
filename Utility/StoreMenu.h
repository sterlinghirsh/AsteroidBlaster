
#ifndef __STOREMENU_H__
#define __STOREMENU_H__


#include "Utility/BitmapTextDisplay.h"
#include "Utility/InputManager.h"

class StoreMenu : public InputReceiver {
   public:
   StoreMenu();
   ~StoreMenu();

   void draw(int shards);
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
   BitmapTextDisplay* buyRailGun;
   BitmapTextDisplay* buyPikachusWrath;
   BitmapTextDisplay* buyTractorBeam;
   BitmapTextDisplay* done;
   BitmapTextDisplay* buyHealth;
   BitmapTextDisplay* numOfShards;
   
   SDL_Surface *titleImage;
   SDL_Rect src, dest;
};


#endif

