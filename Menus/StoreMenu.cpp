
#include <iostream>
#include "SDL.h"
#include "Utility/GlobalUtility.h"
#include "Utility/StoreMenu.h"
#include "Utility/Image.h"
#include "Utility/Texture.h"
#include "Items/AsteroidShip.h"
#include "Utility/Music.h"
#include "Utility/SoundEffect.h"

#define RAILGUN_STRING_INDEX 0
#define TRACTORBEAM_STRING_INDEX 1
#define PIKACHUSWRATH_STRING_INDEX 2
#define DONE_STRING_INDEX 3
#define SHARDS_STRING_INDEX 4

#define RAILGUN_WEAPON_INDEX 1
#define TRACTORBEAM_WEAPON_INDEX 2
#define PIKACHUSWRATH_WEAPON_INDEX 3

#define RAILGUN_PRICE 20
#define TRACTORBEAM_PRICE 10
#define PIKACHUSWRATH_PRICE 20

#define RAILGUN_AMMO_AMOUNT 15
#define TRACTORBEAM_AMMO_AMOUNT 1000
#define PIKACHUSWRATH_AMMO_AMOUNT 500

#define RAILGUN_AMMO_PRICE 10
#define TRACTORBEAM_AMMO_PRICE 5
#define PIKACHUSWRATH_AMMO_PRICE 10

StoreMenu::StoreMenu() {
   menuActive = false;
   
   SDL_Rect position = {0,0};
   std::string fontName = "Font/Slider.ttf";
   
   std::stringstream out;
   out << "Buy Railgun  $" << RAILGUN_PRICE;
   menuTexts.push_back(new Text(out.str(), fontName, position, 24));
   
   out.str("");
   out << "Buy Tractor Beam $" << TRACTORBEAM_PRICE;
   menuTexts.push_back(new Text(out.str(), fontName, position, 24));

   out.str(""); 
   out << "Buy Pikachu's Wrath $" << PIKACHUSWRATH_PRICE;
   menuTexts.push_back(new Text(out.str(), fontName, position, 24));
   
   
   menuTexts.push_back(new Text("Done", fontName, position, 24));
   
   menuTexts.push_back(new Text("Shards: ", 0, "", fontName, position, 24));
   
}


StoreMenu::~StoreMenu() {
   for(int i = 0; i < menuTexts.size(); i++) {
      delete menuTexts[i];
   }
}

void StoreMenu::draw() {
   SDL_Rect position;
   position.x = GW/3;
   position.y = GH/2;
   int currAmmo = -1;
   
   
   //Set the position and the string depending on if the weapon was bought
   menuTexts[RAILGUN_STRING_INDEX]->setPosition(position);
   currAmmo = gameState->ship->getWeapon(RAILGUN_WEAPON_INDEX)->curAmmo;
   if (gameState->ship->getWeapon(RAILGUN_WEAPON_INDEX)->purchased) {
      std::stringstream out;
      out << "Buy " << RAILGUN_AMMO_AMOUNT << " Railgun Ammo $" << RAILGUN_AMMO_PRICE << " (" << currAmmo << ")";
      menuTexts[RAILGUN_STRING_INDEX]->updateBody(out.str());
   }
   
   //Set the position and the string depending on if the weapon was bought
   position.y += GH/10;
   menuTexts[TRACTORBEAM_STRING_INDEX]->setPosition(position);
   currAmmo = gameState->ship->getWeapon(TRACTORBEAM_WEAPON_INDEX)->curAmmo;
   if (gameState->ship->getWeapon(TRACTORBEAM_WEAPON_INDEX)->purchased) {
      std::stringstream out;
      out << "Buy " << TRACTORBEAM_AMMO_AMOUNT << " Tractor Beam Ammo $" << TRACTORBEAM_AMMO_PRICE << " (" << currAmmo << ")";
      menuTexts[TRACTORBEAM_STRING_INDEX]->updateBody(out.str());
   }
   
   //Set the position and the string depending on if the weapon was bought
   position.y += GH/10;
   menuTexts[PIKACHUSWRATH_STRING_INDEX]->setPosition(position);
   currAmmo = gameState->ship->getWeapon(PIKACHUSWRATH_WEAPON_INDEX)->curAmmo;
   if (gameState->ship->getWeapon(PIKACHUSWRATH_WEAPON_INDEX)->purchased) {
      std::stringstream out;
      out << "Buy " << PIKACHUSWRATH_AMMO_AMOUNT << " Pikachu's Wrath Ammo $" << PIKACHUSWRATH_AMMO_PRICE << " (" << currAmmo << ")";
      menuTexts[PIKACHUSWRATH_STRING_INDEX]->updateBody(out.str());
   }
   
   //Done
   position.y += GH/10;
   menuTexts[3]->setPosition(position);
   
   
   //shards
   position.x = GW/9;
   position.y = GH/2;
   menuTexts[SHARDS_STRING_INDEX]->updateBody(gameState->ship->nShards);
   menuTexts[SHARDS_STRING_INDEX]->setPosition(position);
   
   
   
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
      if(menuActive){
         std::cout << "StoreMenu off" << std::endl;
         menuActive = false;
         SoundEffect::stopAllSoundEffect();
         Music::stopMusic();
         Music::playMusic("Asteroids2.ogg");
      } else {
         std::cout << "StoreMenu on" << std::endl;
         menuActive = true;
         SoundEffect::stopAllSoundEffect();
         Music::stopMusic();
         Music::playMusic("8-bit3.ogg");
      }
   }
   if (!menuActive) { return; }
   
   switch(key) {
    case SDLK_ESCAPE: exit(0);
    case SDLK_n:
      SDL_ShowCursor(SDL_DISABLE);
      menuActive = false;
      Music::stopMusic();
      Music::playMusic("Asteroids2.ogg");
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
   bool weaponBought = false;
   int shardsOwned = -1;
   int currAmmo = -1;
   bool boughtSomething = false;
   //for railgun
   if(menuTexts[RAILGUN_STRING_INDEX]->mouseSelect(x,y)) {
      weaponBought = gameState->ship->getWeapon(RAILGUN_WEAPON_INDEX)->purchased;
      shardsOwned = gameState->ship->nShards;
      currAmmo = gameState->ship->getWeapon(RAILGUN_WEAPON_INDEX)->curAmmo;
      if(weaponBought && shardsOwned >= RAILGUN_AMMO_PRICE) {
         std::cout << "bought railgun ammo" << std::endl;
         gameState->ship->getWeapon(RAILGUN_WEAPON_INDEX)->curAmmo += RAILGUN_AMMO_AMOUNT;
         gameState->ship->nShards -= RAILGUN_AMMO_PRICE;
      } else if(shardsOwned >= RAILGUN_PRICE) {
         std::cout << "bought railgun" << std::endl;
         gameState->ship->getWeapon(RAILGUN_WEAPON_INDEX)->purchased = true;
         gameState->ship->nShards -= RAILGUN_PRICE;
      } else {
         std::cout << "not enough money!" << std::endl;
      }
   }
   
   //for tractor beam
   if(menuTexts[TRACTORBEAM_STRING_INDEX]->mouseSelect(x,y)) {
      weaponBought = gameState->ship->getWeapon(TRACTORBEAM_WEAPON_INDEX)->purchased;
      shardsOwned = gameState->ship->nShards;
      currAmmo = gameState->ship->getWeapon(TRACTORBEAM_WEAPON_INDEX)->curAmmo;
      if(weaponBought && shardsOwned >= TRACTORBEAM_AMMO_PRICE) {
         std::cout << "bought TRACTORBEAM ammo" << std::endl;
         gameState->ship->getWeapon(TRACTORBEAM_WEAPON_INDEX)->curAmmo += TRACTORBEAM_AMMO_AMOUNT;
         gameState->ship->nShards -= TRACTORBEAM_AMMO_PRICE;
      } else if(shardsOwned >= TRACTORBEAM_PRICE) {
         std::cout << "bought TRACTORBEAM" << std::endl;
         gameState->ship->getWeapon(TRACTORBEAM_WEAPON_INDEX)->purchased = true;
         gameState->ship->nShards -= TRACTORBEAM_PRICE;
      } else {
         std::cout << "not enough money!" << std::endl;
      }
   }
   
   //for pikachu's wrath
   if(menuTexts[PIKACHUSWRATH_STRING_INDEX]->mouseSelect(x,y)) {
      weaponBought = gameState->ship->getWeapon(PIKACHUSWRATH_WEAPON_INDEX)->purchased;
      shardsOwned = gameState->ship->nShards;
      currAmmo = gameState->ship->getWeapon(PIKACHUSWRATH_WEAPON_INDEX)->curAmmo;
      if(weaponBought && shardsOwned >= PIKACHUSWRATH_AMMO_PRICE) {
         std::cout << "bought PIKACHUSWRATH ammo" << std::endl;
         gameState->ship->getWeapon(PIKACHUSWRATH_WEAPON_INDEX)->curAmmo += PIKACHUSWRATH_AMMO_AMOUNT;
         gameState->ship->nShards -= PIKACHUSWRATH_AMMO_PRICE;
      } else if(shardsOwned >= PIKACHUSWRATH_PRICE) {
         std::cout << "bought PIKACHUSWRATH" << std::endl;
         gameState->ship->getWeapon(PIKACHUSWRATH_WEAPON_INDEX)->purchased = true;
         weaponBought = true;
         gameState->ship->nShards -= PIKACHUSWRATH_PRICE;
      } else {
         std::cout << "not enough money!" << std::endl;
      }
   }
   
   if(menuTexts[DONE_STRING_INDEX]->mouseSelect(x,y)) {
      SDL_ShowCursor(SDL_DISABLE);
      menuActive = false;
      Music::stopMusic();
      Music::playMusic("Asteroids2.ogg");
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

