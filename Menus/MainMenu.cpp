
#include <iostream>
#include "SDL.h"
#include "Utility/GlobalUtility.h"
#include "Utility/GameState.h"
#include "Menus/MainMenu.h"
#include "Utility/Image.h"
#include "Utility/Texture.h"
#include "Utility/Music.h"
#include "Utility/SoundEffect.h"

#define NEWGAME_STRING_INDEX 0
#define CONTINUE_STRING_INDEX 1
#define SAVELOAD_STRING_INDEX 2
#define SETTINGS_STRING_INDEX 3
#define CREDITS_STRING_INDEX 4
#define QUIT_STRING_INDEX 5

MainMenu::MainMenu() {
   menuActive = false;
   firstTime = true;
   
   SDL_Rect position = {0,0};
   std::string fontName = "Font/Slider.ttf";
   
   menuTexts.push_back(new Text("New Game (n)",  fontName, position, 24));
   menuTexts.push_back(new Text("Continue (c)",  fontName, position, 24));
   menuTexts.push_back(new Text("Save/Load Game",  fontName, position, 24));
   menuTexts.push_back(new Text("Settings",  fontName, position, 24));
   menuTexts.push_back(new Text("Credits",  fontName, position, 24));
   menuTexts.push_back(new Text("Quit (esc)", fontName, position, 24));

   for (int i = 0; i < menuTexts.size(); i++) {
      //menuTexts[i]->centered = true;
      menuTexts[i]->alignment = CENTERED;
   }

   SDL_Color greyColor = {128,128,128};
   // grey out the option to show that it is disabled
   menuTexts[CONTINUE_STRING_INDEX]->setColor(greyColor);
   menuTexts[SAVELOAD_STRING_INDEX]->setColor(greyColor);
   menuTexts[SETTINGS_STRING_INDEX]->setColor(greyColor);
   

}


MainMenu::~MainMenu() {
   for(int i = 0; i < menuTexts.size(); i++) {
      delete menuTexts[i];
   }
}

void MainMenu::draw() {
   SDL_Rect position;
   position.x = GW/2;
   position.y = GH/2.3;
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
void MainMenu::keyDown(int key) {
   if (key == SDLK_m )  {
      if(menuActive) {
         menuActive = false;
         SDL_ShowCursor(SDL_DISABLE);
         //since there is a game now, enable continue
         firstTime = false;
         menuTexts[CONTINUE_STRING_INDEX]->setColor(SDL_WHITE);
         Music::stopMusic();
         Music::playMusic("Asteroids2.ogg");
      } else {
         SDL_ShowCursor(SDL_ENABLE);
         menuActive = true;
         SoundEffect::stopAllSoundEffect();
         Music::stopMusic();
         Music::playMusic("8-bit3.ogg");
      }
   }
   if (!menuActive) { return; }
   
   
   switch(key) {
    case SDLK_n:
      SDL_ShowCursor(SDL_DISABLE);
      menuActive = false;
      gameState->reset();
      Music::stopMusic();
      Music::playMusic("Asteroids2.ogg");
      break;
    case SDLK_c:
      SDL_ShowCursor(SDL_DISABLE);
      menuActive = false;
      Music::stopMusic();
      Music::playMusic("Asteroids2.ogg");
      break;
    case SDLK_ESCAPE: 
      exit(0);
      break;
   }
}

/**
 * Handles the player letting go of a key
 */
void MainMenu::keyUp(int key) {
   if (!menuActive) { return; }
   
}

/**
 * Handles the player clicking the mouse
 */
void MainMenu::mouseDown(int button) {
   if (!menuActive) { return; }
   
   if(menuTexts[NEWGAME_STRING_INDEX]->mouseSelect(x,y)) {
      SDL_ShowCursor(SDL_DISABLE);
      gameState->reset();
      menuActive = false;
      firstTime = false;
      Music::stopMusic();
      Music::playMusic("Asteroids2.ogg");
   } else if(menuTexts[CONTINUE_STRING_INDEX]->mouseSelect(x,y) && !firstTime) {
      SDL_ShowCursor(SDL_DISABLE);
      menuActive = false;
      Music::stopMusic();
      Music::playMusic("Asteroids2.ogg");
   } else if(menuTexts[CREDITS_STRING_INDEX]->mouseSelect(x,y)) {
      menuActive = false;
      creditsMenu->menuActive = true;
      printf("creditsMenu active: %d\n", creditsMenu->menuActive);
      printf("menuActive: %d\n", menuActive);
      Music::stopMusic();
      Music::playMusic("Careless_Whisper.ogg");
   } else if(menuTexts[QUIT_STRING_INDEX]->mouseSelect(x,y)) {
      exit(0);
   }
}

/**
 * Handles the player letting go of a mouse click
 */
void MainMenu::mouseUp(int button) {
   if (!menuActive) { return; }
}

void MainMenu::mouseMove(int dx, int dy, int _x, int _y) {
   //std::cout << "mouseMove=(" << _x << "," << _y << ")" << std::endl;
   if (!menuActive) { return; }
   x = _x;
   y = _y;
   
   if(menuTexts[NEWGAME_STRING_INDEX]->mouseSelect(x,y)) {
      menuTexts[NEWGAME_STRING_INDEX]->setColor(SDL_RED);
   } else {
      menuTexts[NEWGAME_STRING_INDEX]->setColor(SDL_WHITE);
   }
   
   if(!firstTime) {
      if(menuTexts[CONTINUE_STRING_INDEX]->mouseSelect(x,y)) {
         menuTexts[CONTINUE_STRING_INDEX]->setColor(SDL_RED);
      } else {
         menuTexts[CONTINUE_STRING_INDEX]->setColor(SDL_WHITE);
      }
   }

   if(menuTexts[CREDITS_STRING_INDEX]->mouseSelect(x,y)) {
      menuTexts[CREDITS_STRING_INDEX]->setColor(SDL_RED);
   } else {
      menuTexts[CREDITS_STRING_INDEX]->setColor(SDL_WHITE);
   }
   
   if(menuTexts[QUIT_STRING_INDEX]->mouseSelect(x,y)) {
      menuTexts[QUIT_STRING_INDEX]->setColor(SDL_RED);
   } else {
      menuTexts[QUIT_STRING_INDEX]->setColor(SDL_WHITE);
   }
}

