
#include <iostream>
#include "SDL.h"
#include "Utility/StoreMenu.h"
#include "Utility/Image.h"
#include "Utility/Texture.h"

StoreMenu::StoreMenu() {
   menuActive = false;
   buyRailGun = new BitmapTextDisplay("Buy Rail Gun", GW/2 - 50, GH/2 - 60);
   buyPikachusWrath = new BitmapTextDisplay("Buy Pikachu's Wrath", GW/2 - 50, GH/2 - 20);
   buyTractorBeam = new BitmapTextDisplay("Buy Tractor Beam", GW/2 - 50, GH/2 + 20);
   buyHealth = new BitmapTextDisplay("Buy Health", GW/2 - 50, GH/2 + 60);
   done = new BitmapTextDisplay("Done", GW/2 - 50, GH/2 + 100);
   
   
   
   numOfShards = new BitmapTextDisplay("Shards: ", 5, "",50, 50);
   
}


StoreMenu::~StoreMenu() {
   SDL_FreeSurface(titleImage);
}

bool StoreMenu::isActive() {
   if (menuActive) {
      SDL_ShowCursor(SDL_ENABLE);
   } else {
      SDL_ShowCursor(SDL_DISABLE);
   }
   return menuActive;
}

void StoreMenu::update() {

}

void StoreMenu::draw(int shards) {
   buyRailGun->setPosition(GW/2 - 50, GH/2 - 60);
   buyPikachusWrath->setPosition(GW/2 - 50, GH/2 - 20);
   buyTractorBeam->setPosition(GW/2 - 50, GH/2 + 20);
   buyHealth->setPosition(GW/2 - 50, GH/2 + 60);
   done->setPosition(GW/2 - 50, GH/2 + 100);
   numOfShards->updateBody(shards);
   
   glDisable(GL_TEXTURE_2D);
   
   glClearColor(0.0, 0.0, 0.0, 0.5);
   
   // Clear the screen
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   

   
   //draw the text
   glPushMatrix();
      useOrtho();
      glDisable(GL_LIGHTING);
      
      buyRailGun->draw();
      buyPikachusWrath->draw();
      buyTractorBeam->draw();
      buyHealth->draw();
      done->draw();
      numOfShards->draw();

      glEnable(GL_LIGHTING);
      usePerspective();
   glPopMatrix();


   //draw the text
   glDisable(GL_LIGHTING);
   glEnable(GL_TEXTURE_2D);
   
   /* Enable smooth shading */
   glShadeModel( GL_SMOOTH );

   /* Depth buffer setup */
   glClearDepth( 1.0f );

   /* Enables Depth Testing */
   glEnable( GL_DEPTH_TEST );

   /* The Type Of Depth Test To Do */
   glDepthFunc( GL_LEQUAL );

   /* Really Nice Perspective Calculations */
   glHint( GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST );
   
   glPushMatrix();
      glLoadIdentity( );
      glTranslatef( 0.0f, 1.0f, -5.0f );
      glScalef( 2.0, 0.5, 1.0 );
      glColor3f(1.0, 1.0, 1.0);

      /* Select Our Texture */
      glBindTexture( GL_TEXTURE_2D, Texture::getTexture("Logo.png") );

      /* NOTE:
      *   The x coordinates of the glTexCoord2f function need to inverted
      * for SDL because of the way SDL_LoadBmp loads the data. So where
      * in the tutorial it has glTexCoord2f( 1.0f, 0.0f ); it should
      * now read glTexCoord2f( 0.0f, 1.0f );
      */
      glBegin(GL_QUADS);
         /* Front Face */
         /* Bottom Left Of The Texture and Quad */
         glTexCoord2f( 0.0f, 1.0f ); glVertex3f( -1.0f, -1.0f, 1.0f );
         /* Bottom Right Of The Texture and Quad */
         glTexCoord2f( 1.0f, 1.0f ); glVertex3f(  1.0f, -1.0f, 1.0f );
         /* Top Right Of The Texture and Quad */
         glTexCoord2f( 1.0f, 0.0f ); glVertex3f(  1.0f,  1.0f, 1.0f );
         /* Top Left Of The Texture and Quad */
         glTexCoord2f( 0.0f, 0.0f ); glVertex3f( -1.0f,  1.0f, 1.0f );
      glEnd();
      glEnable(GL_CULL_FACE);
   glPopMatrix();
   glDisable(GL_TEXTURE_2D);
   glEnable(GL_LIGHTING);

   SDL_GL_SwapBuffers();

}

/**
 * Handles the player pressing down a key
 */
void StoreMenu::keyDown(int key) {
   if (key == SDLK_m )  {
      menuActive = !menuActive;
   }
   if (!menuActive) { return; }
   
   
   switch(key) {
    case SDLK_ESCAPE: exit(0);
    case SDLK_n:
      SDL_ShowCursor(SDL_DISABLE);
      menuActive = false;
      break;
   }
}

/**
 * Handles the player letting go of a key
 */
void StoreMenu::keyUp(int key) {
   if (!menuActive) { return; }
   
}

/**
 * Handles the player clicking the mouse
 */
void StoreMenu::mouseDown(int button) {
   if (!menuActive) { return; }
   if (x >= buyRailGun->xCoord &&
         x <= buyRailGun->xCoord + 100 &&
         y >= buyRailGun->yCoord - 20 &&
         y <= buyRailGun->yCoord ) {
          
   }
   
   if (x >= done->xCoord &&
         x <= done->xCoord + 50 &&
         y >= done->yCoord - 20 &&
         y <= done->yCoord ) {
      menuActive = false;
      SDL_ShowCursor(SDL_DISABLE);
   }
}

/**
 * Handles the player letting go of a mouse click
 */
void StoreMenu::mouseUp(int button) {
   if (!menuActive) { return; }
   
}

void StoreMenu::mouseMove(int dx, int dy, int _x, int _y) {
   if (!menuActive) { return; }
   x = _x;
   y = _y;
   if (x >= buyRailGun->xCoord &&
         x <= buyRailGun->xCoord + 100 &&
         y >= buyRailGun->yCoord - 20 &&
         y <= buyRailGun->yCoord ) {
      buyRailGun->setColor(1.0,0.0,0.0);
   } else {
      buyRailGun->setColor(1.0,1.0,1.0);
   }
   if (x >= buyPikachusWrath->xCoord &&
         x <= buyPikachusWrath->xCoord + 50 &&
         y >= buyPikachusWrath->yCoord - 20 &&
         y <= buyPikachusWrath->yCoord ) {
      buyPikachusWrath->setColor(1.0,0.0,0.0);
   } else {
      buyPikachusWrath->setColor(1.0,1.0,1.0);
   }
   if (x >= buyTractorBeam->xCoord &&
         x <= buyTractorBeam->xCoord + 50 &&
         y >= buyTractorBeam->yCoord - 20 &&
         y <= buyTractorBeam->yCoord ) {
      buyTractorBeam->setColor(1.0,0.0,0.0);
   } else {
      buyTractorBeam->setColor(1.0,1.0,1.0);
   }
   if (x >= buyHealth->xCoord &&
         x <= buyHealth->xCoord + 50 &&
         y >= buyHealth->yCoord - 20 &&
         y <= buyHealth->yCoord ) {
      buyHealth->setColor(1.0,0.0,0.0);
   } else {
      buyHealth->setColor(1.0,1.0,1.0);
   }
   if (x >= done->xCoord &&
         x <= done->xCoord + 50 &&
         y >= done->yCoord - 20 &&
         y <= done->yCoord ) {
      done->setColor(1.0,0.0,0.0);
   } else {
      done->setColor(1.0,1.0,1.0);
   }

   
   
}


