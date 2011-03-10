
#include <iostream>
#include "SDL.h"
#include "Utility/Menu.h"
#include "Utility/Image.h"
#include "Utility/Texture.h"

Menu::Menu() {
   menuActive = false;
   newGame = new BitmapTextDisplay("New Game", GW/2 - 50, GH/2 - 60);
   saveLoadGame = new BitmapTextDisplay("Save/Load Game", GW/2 - 50, GH/2 - 20);
   settings = new BitmapTextDisplay("Settings", GW/2 - 50, GH/2 + 20);
   quit = new BitmapTextDisplay("Quit", GW/2 - 50, GH/2 + 60);

   // grey out the option to show that it is disabled
   saveLoadGame->setColor(0.5,0.5,0.5);
   settings->setColor(0.5,0.5,0.5);
   
   
}


Menu::~Menu() {
   SDL_FreeSurface(titleImage);
}

void Menu::update() {

}

void Menu::draw() {
   newGame->setPosition(GW/2 - 50, GH/2 - 20);
   saveLoadGame->setPosition(GW/2 - 50, GH/2 + 20);
   settings->setPosition(GW/2 - 50, GH/2 + 60);
   quit->setPosition(GW/2 - 50, GH/2 + 100);
   
   glDisable(GL_TEXTURE_2D);
   
   glClearColor(0.0, 0.0, 0.0, 0.5);
   
   // Clear the screen
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   

   
   //draw the text
   glPushMatrix();
      useOrtho();
      glDisable(GL_LIGHTING);
      
      newGame->draw();
      saveLoadGame->draw();
      settings->draw();
      quit->draw();

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
void Menu::keyDown(int key) {
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
void Menu::keyUp(int key) {
   if (!menuActive) { return; }
   
}

/**
 * Handles the player clicking the mouse
 */
void Menu::mouseDown(int button) {
   if (!menuActive) { return; }
   if (x >= newGame->xCoord &&
         x <= newGame->xCoord + 100 &&
         y >= newGame->yCoord - 20 &&
         y <= newGame->yCoord ) {
      SDL_ShowCursor(SDL_DISABLE);
      menuActive = false;
   }
   
   if (x >= quit->xCoord &&
         x <= quit->xCoord + 50 &&
         y >= quit->yCoord - 20 &&
         y <= quit->yCoord ) {
      exit(0);
   }
}

/**
 * Handles the player letting go of a mouse click
 */
void Menu::mouseUp(int button) {
   if (!menuActive) { return; }
   
}

void Menu::mouseMove(int dx, int dy, int _x, int _y) {
   if (!menuActive) { return; }
   x = _x;
   y = _y;
   if (x >= newGame->xCoord &&
         x <= newGame->xCoord + 100 &&
         y >= newGame->yCoord - 20 &&
         y <= newGame->yCoord ) {
      newGame->setColor(1.0,0.0,0.0);
   } else {
      newGame->setColor(1.0,1.0,1.0);
   }

   if (x >= quit->xCoord &&
         x <= quit->xCoord + 50 &&
         y >= quit->yCoord - 20 &&
         y <= quit->yCoord ) {
      quit->setColor(1.0,0.0,0.0);
   } else {
      quit->setColor(1.0,1.0,1.0);
   }
}

