
#include <iostream>
#include "SDL.h"
#include "Utility/GlobalUtility.h"
#include "Utility/StoreMenu.h"
#include "Utility/Image.h"
#include "Utility/Texture.h"
#include "Utility/Text.h"
#include "Items/AsteroidShip.h"

#define RAILGUN_STRING_INDEX 0
#define TRACTORBEAM_STRING_INDEX 1
#define PIKACHUSWRATH_STRING_INDEX 2
#define DONE_STRING_INDEX 3

#define RAILGUN_WEAPON_INDEX 1
#define TRACTORBEAM_WEAPON_INDEX 2
#define PIKACHUSWRATH_WEAPON_INDEX 3

#define RAILGUN_PRICE 20
#define TRACTORBEAM_PRICE 10
#define PIKACHUSWRATH_PRICE 20

#define RAILGUN_AMMO_AMOUNT 15
#define TRACTORBEAM_AMMO_AMOUNT 1000
#define PIKACHUSWRATH_AMMO_AMOUNT 500

#define RAILGUN_AMMO_PRICE 20
#define TRACTORBEAM_AMMO_PRICE 10
#define PIKACHUSWRATH_AMMO_PRICE 20

StoreMenu::StoreMenu() {
   menuActive = false;
   
   SDL_Rect position = {0,0};
   std::string fontName = "Font/Slider.ttf";
   
   
   
   std::stringstream out;
   out << "Buy Railgun  $" << RAILGUN_PRICE << " (" << gameState->ship->getWeapon(RAILGUN_WEAPON_INDEX)->curAmmo << ")";
   menuTexts.push_back(new Text(out.str(), fontName, position, 24));
   
   out.str("");
   out << "Buy Tractor Beam $" << TRACTORBEAM_PRICE << " (" << gameState->ship->getWeapon(TRACTORBEAM_WEAPON_INDEX)->curAmmo << ")";
   menuTexts.push_back(new Text(out.str(), fontName, position, 24));

   out.str(""); 
   out << "Buy Pikachu's Wrath $" << PIKACHUSWRATH_PRICE << " (" << gameState->ship->getWeapon(PIKACHUSWRATH_WEAPON_INDEX)->curAmmo << ")";
   menuTexts.push_back(new Text(out.str(), fontName, position, 24));
   
   
   menuTexts.push_back(new Text("Done", fontName, position, 24));
   
}


StoreMenu::~StoreMenu() {
   for(int i = 0; i < menuTexts.size(); i++) {
      delete menuTexts[i];
   }
}

void StoreMenu::draw() {
   SDL_Rect position;
   position.x = GW/3;
   position.y = GH/2 + 50;
   
   std::stringstream out;
   out << "Buy Railgun  $" << RAILGUN_PRICE << " (" << gameState->ship->getWeapon(RAILGUN_WEAPON_INDEX)->curAmmo << ")";
   menuTexts[RAILGUN_STRING_INDEX]->updateBody(out.str());
   
   out.str("");
   out << "Buy Tractor Beam $" << TRACTORBEAM_PRICE << " (" << gameState->ship->getWeapon(TRACTORBEAM_WEAPON_INDEX)->curAmmo << ")";
   menuTexts[TRACTORBEAM_STRING_INDEX]->updateBody(out.str());

   out.str(""); 
   out << "Buy Pikachu's Wrath $" << PIKACHUSWRATH_PRICE << " (" << gameState->ship->getWeapon(PIKACHUSWRATH_WEAPON_INDEX)->curAmmo << ")";
   menuTexts[PIKACHUSWRATH_STRING_INDEX]->updateBody(out.str());
   
   for(int i = 0; i < menuTexts.size(); i++) {
      menuTexts[i]->setPosition(position);
      position.y += GH/10;
   }
   
   
   
   // Clear the screen
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   

   
   //draw the text
   glPushMatrix();
      useOrtho();
      glDisable(GL_LIGHTING);
      
      for(int i = 0; i < menuTexts.size(); i++) {
         menuTexts[i]->draw();
      }

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
      glBindTexture( GL_TEXTURE_2D, Texture::getTexture("StoreLogo") );

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
void StoreMenu::keyDown(int key) {
   if (key == SDLK_p )  {
      menuActive = !menuActive;
      std::cout << "StoreMenu toggled" << std::endl;
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

   //for railgun
   if(menuTexts[RAILGUN_STRING_INDEX]->mouseSelect(x,y)) {
      if(gameState->ship->getWeapon(RAILGUN_WEAPON_INDEX)->purchased && gameState->ship->nShards >= RAILGUN_AMMO_PRICE) {
         std::cout << "bought railgun ammo" << std::endl;
         gameState->ship->getWeapon(RAILGUN_WEAPON_INDEX)->curAmmo += RAILGUN_AMMO_AMOUNT;
         gameState->ship->nShards -= RAILGUN_AMMO_PRICE;
      } else if(gameState->ship->nShards >= RAILGUN_PRICE) {
         std::cout << "bought railgun" << std::endl;
         gameState->ship->getWeapon(RAILGUN_WEAPON_INDEX)->purchased = true;
         gameState->ship->nShards -= RAILGUN_PRICE;
         
         std::stringstream out;
         out << "Buy " << RAILGUN_AMMO_AMOUNT << " Railgun Ammo $" << RAILGUN_AMMO_PRICE;
         menuTexts[RAILGUN_STRING_INDEX]->updateBody(out.str());
         
      }
   }
   //for tractor beam
   if(menuTexts[TRACTORBEAM_STRING_INDEX]->mouseSelect(x,y)) {
      if(gameState->ship->getWeapon(TRACTORBEAM_WEAPON_INDEX)->purchased && gameState->ship->nShards >= 5) {
         std::cout << "bought tractor beam ammo" << std::endl;
         gameState->ship->getWeapon(TRACTORBEAM_WEAPON_INDEX)->curAmmo += TRACTORBEAM_AMMO_AMOUNT;
         gameState->ship->nShards -= TRACTORBEAM_AMMO_PRICE;
      } else if(gameState->ship->nShards >= TRACTORBEAM_PRICE) {
         std::cout << "bought tractor beam" << std::endl;
         gameState->ship->getWeapon(TRACTORBEAM_WEAPON_INDEX)->purchased = true;
         gameState->ship->nShards -= TRACTORBEAM_PRICE;
         
         std::stringstream out;
         out << "Buy " << RAILGUN_AMMO_AMOUNT << " Tractor Beam Ammo $" << TRACTORBEAM_AMMO_PRICE;
         menuTexts[TRACTORBEAM_STRING_INDEX]->updateBody(out.str());
      }
   }
   //for pikachu's wrath
   if(menuTexts[PIKACHUSWRATH_STRING_INDEX]->mouseSelect(x,y)) {
      if(gameState->ship->getWeapon(PIKACHUSWRATH_WEAPON_INDEX)->purchased && gameState->ship->nShards >= PIKACHUSWRATH_AMMO_PRICE) {
         std::cout << "bought pikachu's wrath ammo" << std::endl;
         gameState->ship->getWeapon(PIKACHUSWRATH_WEAPON_INDEX)->curAmmo += PIKACHUSWRATH_AMMO_AMOUNT;
         gameState->ship->nShards -= PIKACHUSWRATH_AMMO_PRICE;
      } else if(gameState->ship->nShards >= PIKACHUSWRATH_PRICE) {
         std::cout << "bought pikachu's wrath" << std::endl;
         gameState->ship->getWeapon(PIKACHUSWRATH_WEAPON_INDEX)->purchased = true;
         gameState->ship->nShards -= PIKACHUSWRATH_PRICE;
         
         std::stringstream out;
         out << "Buy " << PIKACHUSWRATH_AMMO_AMOUNT << " Pikachu's Wrath Ammo $" << PIKACHUSWRATH_AMMO_PRICE;
         menuTexts[PIKACHUSWRATH_STRING_INDEX]->updateBody(out.str());
      }
   }
   if(menuTexts[DONE_STRING_INDEX]->mouseSelect(x,y)) {
      SDL_ShowCursor(SDL_DISABLE);
      menuActive = false;
   }
}

/**
 * Handles the player letting go of a mouse click
 */
void StoreMenu::mouseUp(int button) {
   if (!menuActive) { return; }
   
}

void StoreMenu::mouseMove(int dx, int dy, int _x, int _y) {
   //std::cout << "mouseMove=(" << _x << "," << _y << ")" << std::endl;
   if (!menuActive) { return; }
   x = _x;
   y = _y;
   
   if(menuTexts[0]->mouseSelect(x,y)) {
      menuTexts[0]->setColor(SDL_RED);
   } else {
      menuTexts[0]->setColor(SDL_WHITE);
   }
   if(menuTexts[1]->mouseSelect(x,y)) {
      menuTexts[1]->setColor(SDL_RED);
   } else {
      menuTexts[1]->setColor(SDL_WHITE);
   }
   if(menuTexts[2]->mouseSelect(x,y)) {
      menuTexts[2]->setColor(SDL_RED);
   } else {
      menuTexts[2]->setColor(SDL_WHITE);
   }
   if(menuTexts[3]->mouseSelect(x,y)) {
      menuTexts[3]->setColor(SDL_RED);
   } else {
      menuTexts[3]->setColor(SDL_WHITE);
   }
}

