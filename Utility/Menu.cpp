
#include <iostream>
#include "SDL.h"
#include "Utility/Menu.h"

Menu::Menu() {
   menuActive = true;
   newGame = new BitmapTextDisplay("New Game", GW/2 - 50, GH/2 - 60);
   saveLoadGame = new BitmapTextDisplay("Save/Load Game", GW/2 - 50, GH/2 - 20);
   settings = new BitmapTextDisplay("Settings", GW/2 - 50, GH/2 + 20);
   quit = new BitmapTextDisplay("Quit", GW/2 - 50, GH/2 + 60);

   newGame->setFont(GLUT_BITMAP_TIMES_ROMAN_24);
   saveLoadGame->setFont(GLUT_BITMAP_TIMES_ROMAN_24);
   settings->setFont(GLUT_BITMAP_TIMES_ROMAN_24);
   quit->setFont(GLUT_BITMAP_TIMES_ROMAN_24);
   
   SDL_Surface *temp;
    
   temp = SDL_LoadBMP("Images/Logo.bmp");
   
   
   if (temp == NULL) {
	   printf("Unable to load bitmap: %s\n", SDL_GetError());
	   exit(0);
   }

   SDL_SetColorKey(temp, (SDL_SRCCOLORKEY|SDL_RLEACCEL), SDL_MapRGB(temp->format, 0, 0, 0));

   titleImage = SDL_DisplayFormat(temp);
   SDL_FreeSurface(temp);
   
     
   src.x = 0;
   src.y = 0;
   src.w = titleImage->w;
   src.h = titleImage->h;
    
   dest.x = 100;
   dest.y = 100;
   dest.w = titleImage->w;
   dest.h = titleImage->h;
   
}


Menu::~Menu() {
   SDL_FreeSurface(titleImage);
}

bool Menu::isActive() {
   if (menuActive) {
      SDL_ShowCursor(SDL_ENABLE);
   } else {
      SDL_ShowCursor(SDL_DISABLE);
   }
   return menuActive;
}

void Menu::update() {

}

void Menu::draw() {
   glDisable(GL_TEXTURE_2D);
   
   glClearColor(0.0, 0.0, 0.0, 0.0);
   
   // Clear the screen
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   

   
   //draw the text
   glPushMatrix();
   useOrtho();
   glDisable(GL_LIGHTING);
   SDL_FillRect(gDrawSurface, NULL, 0);
   SDL_BlitSurface(titleImage, &src, gDrawSurface, &dest);
   newGame->draw();
   saveLoadGame->draw();
   settings->draw();
   quit->draw();
   glEnable(GL_LIGHTING);
   usePerspective();
   glPopMatrix();

   SDL_GL_SwapBuffers();
   glEnable(GL_TEXTURE_2D);
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
   if (x >= 350 &&
         x <= 460 &&
         y >= 225 &&
         y <= 245) {
      SDL_ShowCursor(SDL_DISABLE);
      menuActive = false;
   } else if (x >= 350 &&
         x <= 450 &&
         y >= 345 &&
         y <= 365) {
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
   if (x >= 350 &&
         x <= 450 &&
         y >= 230 &&
         y <= 240) {
      newGame->setColor(1.0,0.0,0.0);
   } else {
      newGame->setColor(1.0,1.0,1.0);
   }

   if (x >= 350 &&
         x <= 450 &&
         y >= 345 &&
         y <= 365) {
      quit->setColor(1.0,0.0,0.0);
   } else {
      quit->setColor(1.0,1.0,1.0);
   }
}



