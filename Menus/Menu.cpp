
#include <iostream>
#include "SDL.h"
#include "Utility/GlobalUtility.h"
#include "Menus/Menu.h"
#include "Utility/Image.h"
#include "Utility/Texture.h"
#include "Utility/Text.h"

Menu::Menu() {
   menuActive = false;
   
   SDL_Rect position = {0,0};
   std::string fontName = "Font/FreeMonoBold.ttf";
   
   menuTexts.push_back(new Text("New Game",  fontName, position, 24));
   menuTexts.push_back(new Text("Save/Load Game",  fontName, position, 24));
   menuTexts.push_back(new Text("Settings",  fontName, position, 24));
   menuTexts.push_back(new Text("Quit", fontName, position, 24));
   

   SDL_Color greyColor = {128,128,128};
   // grey out the option to show that it is disabled
   menuTexts[1]->setColor(greyColor);
   menuTexts[2]->setColor(greyColor);
   

}


Menu::~Menu() {
   for(int i = 0; i < menuTexts.size(); i++) {
      delete menuTexts[i];
   }
}

void Menu::update() {

}

void Menu::draw() {
   SDL_Rect position;
   position.x = GW/2 - 50;
   position.y = GH/2 + 50;
   for(int i = 0; i < menuTexts.size(); i++) {
      menuTexts[i]->setPosition(position);
      position.y += GH/10;
   }
   
   
   
   // Clear the screen
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   

   
   //draw the text
   glPushMatrix();
      useOrtho();
      glDisable(GL_CULL_FACE);
      glDisable(GL_LIGHTING);
      
      
      for(int i = 0; i < menuTexts.size(); i++) {
         menuTexts[i]->draw();
      }
      

      usePerspective();
   glPopMatrix();



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
   
   glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,
                     GL_CLAMP );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,
                     GL_CLAMP );
   
   glPushMatrix();
      glLoadIdentity( );
      glTranslatef( 0.0f, 0.2f, -1.0f );
      glScalef( 0.3625, 0.1, 1.0 );
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
         glTexCoord2f( 0.0f, 1.0f ); glVertex2f( -1.0f, -1.0f);
         /* Bottom Right Of The Texture and Quad */
         glTexCoord2f( 1.0f, 1.0f ); glVertex2f(  1.0f, -1.0f);
         /* Top Right Of The Texture and Quad */
         glTexCoord2f( 1.0f, 0.0f ); glVertex2f(  1.0f,  1.0f);
         /* Top Left Of The Texture and Quad */
         glTexCoord2f( 0.0f, 0.0f ); glVertex2f( -1.0f,  1.0f);
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
      if(menuActive) {
         menuActive = false;
         SDL_ShowCursor(SDL_DISABLE);
      } else {
         SDL_ShowCursor(SDL_ENABLE);
         menuActive = true;
      }
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
   
   for(int i = 0; i < menuTexts.size(); i++) {
      if(menuTexts[i]->mouseSelect(x,y) && i == 0) {
         SDL_ShowCursor(SDL_DISABLE);
         menuActive = false;
      }
      if(menuTexts[i]->mouseSelect(x,y) && i == 3) {
         exit(0);
      }
   }
}

/**
 * Handles the player letting go of a mouse click
 */
void Menu::mouseUp(int button) {
   if (!menuActive) { return; }
   
}

void Menu::mouseMove(int dx, int dy, int _x, int _y) {
   //std::cout << "mouseMove=(" << _x << "," << _y << ")" << std::endl;
   if (!menuActive) { return; }
   x = _x;
   y = _y;
   
   if(menuTexts[0]->mouseSelect(x,y)) {
      menuTexts[0]->setColor(SDL_RED);
   } else {
      menuTexts[0]->setColor(SDL_WHITE);
   }
   if(menuTexts[3]->mouseSelect(x,y)) {
      menuTexts[3]->setColor(SDL_RED);
   } else {
      menuTexts[3]->setColor(SDL_WHITE);
   }
}

