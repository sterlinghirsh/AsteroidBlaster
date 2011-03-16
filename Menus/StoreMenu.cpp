
#include <iostream>
#include "SDL.h"
#include "Utility/GlobalUtility.h"
#include "Utility/StoreMenu.h"
#include "Utility/Image.h"
#include "Utility/Texture.h"
#include "Items/AsteroidShip.h"
#include "Utility/Music.h"
#include "Utility/SoundEffect.h"

#define DONE_STOREMENUINDEX 0
#define SHARDS_STOREMENUINDEX 1
#define HEALTH_STOREMENUINDEX 2
#define WEAPONS_STOREMENUINDEX 3
#define UPGRADES_STOREMENUINDEX 4
#define AMMO_STOREMENUINDEX 5
#define SHIP_STOREMENUINDEX 6

#define RAILGUN_WEAPONSTEXTSINDEX 0
#define PIKACHUSWRATH_WEAPONSTEXTSINDEX 1
#define ANTIINERTIA_WEAPONSTEXTSINDEX 2

#define RAILGUN_AMMOTEXTSINDEX 0
#define PIKACHUSWRATH_AMMOTEXTSINDEX 1
#define ANTIINERTIA_AMMOTEXTSINDEX 2

#define BUYHEALTH_SHIPTEXTSINDEX 0
#define ENGINEUPGRADE_SHIPTEXTSINDEX 1

#define RAILGUN_WEAPON_INDEX 1
#define PIKACHUSWRATH_WEAPON_INDEX 3
#define ANTIINERTIA_WEAPON_INDEX 6

#define RAILGUN_PRICE 20
#define PIKACHUSWRATH_PRICE 20
#define ANTIINERTIA_PRICE 10

#define ENGINEUPGRADE_PRICE 10
#define ENGINEUPGRADE_UPPRICE 10


#define RAILGUN_AMMO_AMOUNT 15
#define PIKACHUSWRATH_AMMO_AMOUNT 500
#define ANTIINERTIA_AMMO_AMOUNT 500

#define RAILGUN_AMMO_PRICE 10
#define PIKACHUSWRATH_AMMO_PRICE 10
#define ANTIINERTIA_AMMO_PRICE 10

StoreMenu::StoreMenu() {
   menuActive = false;
   menuSelection = WEAPONS;
   
   SDL_Rect position = {0,0};
   std::string fontName = DEFAULT_FONT; 
   std::stringstream out;
   
   
   //Get shards, health and Done text onto menuTexts
   menuTexts.push_back(new Text("Done (n)", fontName, position, 24));
   menuTexts[DONE_STOREMENUINDEX]->selectable = true;
   menuTexts.push_back(new Text("Shards: ", 0, "", fontName, position, 24));
   menuTexts.push_back(new Text("Health: ", 0, "", fontName, position, 24));
   
   menuTexts.push_back(new Text("Weapons", fontName, position, 24));
   menuTexts[WEAPONS_STOREMENUINDEX]->selectable = true;
   menuTexts.push_back(new Text("Upgrades", fontName, position, 24));
   menuTexts[UPGRADES_STOREMENUINDEX]->selectable = true;
   menuTexts.push_back(new Text("Ammo", fontName, position, 24));
   menuTexts[AMMO_STOREMENUINDEX]->selectable = true;
   menuTexts.push_back(new Text("Ship", fontName, position, 24));
   menuTexts[SHIP_STOREMENUINDEX]->selectable = true;
   
   
   
   //get weapon purchase text in weaponsTexts
   out << "Buy Railgun  $" << RAILGUN_PRICE;
   weaponsTexts.push_back(new Text(out.str(), fontName, position, 24));
   
   out.str(""); 
   out << "Buy Pikachu's Wrath $" << PIKACHUSWRATH_PRICE;
   weaponsTexts.push_back(new Text(out.str(), fontName, position, 24));
   
   out.str(""); 
   out << "Buy Anti Inertia Beam $" << ANTIINERTIA_PRICE;
   weaponsTexts.push_back(new Text(out.str(), fontName, position, 24));
   
   
   //get upgrade text in upgradesTexts
   out.str(""); 
   out << "Upgrade Unavailable!";
   upgradesTexts.push_back(new Text(out.str(), fontName, position, 24));
   
   
   //get ammo text in ammoTexts
   out.str(""); 
   out << "Buy " << RAILGUN_AMMO_AMOUNT << " Railgun Ammo $" << RAILGUN_AMMO_PRICE;
   ammoTexts.push_back(new Text(out.str(), fontName, position, 24));
   
   out.str(""); 
   out << "Buy " << PIKACHUSWRATH_AMMO_AMOUNT << " Pikachu's Wrath Ammo Ammo $" << PIKACHUSWRATH_AMMO_PRICE;
   ammoTexts.push_back(new Text(out.str(), fontName, position, 24));
   
   out.str(""); 
   out << "Buy " << ANTIINERTIA_AMMO_AMOUNT << " Anti Inertia Beam Ammo $" << ANTIINERTIA_AMMO_PRICE;
   ammoTexts.push_back(new Text(out.str(), fontName, position, 24));
   
   //get ship related text in shipTexts
   out.str(""); 
   out << "Buy Ship Health";
   shipTexts.push_back(new Text(out.str(), fontName, position, 24));
   
   out.str(""); 
   out << "Upgrade Engine $" <<ENGINEUPGRADE_PRICE;
   shipTexts.push_back(new Text(out.str(), fontName, position, 24));
    
    
   for(int i = 0; i < weaponsTexts.size(); i++) {
      weaponsTexts[i]->selectable = true;
   }
   for(int i = 0; i < upgradesTexts.size(); i++) {
      upgradesTexts[i]->selectable = true;
   }
   for(int i = 0; i < ammoTexts.size(); i++) {
      ammoTexts[i]->selectable = true;
   }
   for(int i = 0; i < shipTexts.size(); i++) {
      shipTexts[i]->selectable = true;
   }
   
}


StoreMenu::~StoreMenu() {
   for(int i = 0; i < menuTexts.size(); i++) {
      delete menuTexts[i];
   }
   for(int i = 0; i < weaponsTexts.size(); i++) {
      delete weaponsTexts[i];
   }
   for(int i = 0; i < upgradesTexts.size(); i++) {
      delete upgradesTexts[i];
   }
   for(int i = 0; i < ammoTexts.size(); i++) {
      delete ammoTexts[i];
   }
   for(int i = 0; i < shipTexts.size(); i++) {
      delete shipTexts[i];
   }
   
}

void StoreMenu::draw() {
   SDL_Rect position;
   position.x = GW/3;
   position.y = GH/2;

   // Clear the screen
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


   //Done
   position.x = GW*(8.0/10.0);
   position.y = GH*(9.5/10.0);
   menuTexts[DONE_STOREMENUINDEX]->setPosition(position);
   
   //shards
   position.x = GW*(0.1/10.0);
   position.y = GH*(9.5/10.0);
   menuTexts[SHARDS_STOREMENUINDEX]->updateBody(gameState->ship->nShards);
   menuTexts[SHARDS_STOREMENUINDEX]->setPosition(position);
   
   //health
   position.x = GW*(2.0/10.0);
   position.y = GH*(9.5/10.0);
   menuTexts[HEALTH_STOREMENUINDEX]->updateBody(gameState->ship->health);
   menuTexts[HEALTH_STOREMENUINDEX]->setPosition(position);
   
   //weapons
   position.x = GW*(1.0/10.0);
   position.y = GH*(5.0/10.0);
   menuTexts[WEAPONS_STOREMENUINDEX]->setPosition(position);
   
   //upgrades
   position.y += GH/10;
   menuTexts[UPGRADES_STOREMENUINDEX]->setPosition(position);
   
   //weapons
   position.y += GH/10;
   menuTexts[AMMO_STOREMENUINDEX]->setPosition(position);
   
   //weapons
   position.y += GH/10;
   menuTexts[SHIP_STOREMENUINDEX]->setPosition(position);


   drawTexts(menuTexts);
   
   position.x = GW*(3.0/10.0);
   position.y = GH*(5.0/10.0);
   if(menuSelection == WEAPONS) {
      for(int i = 0; i < weaponsTexts.size(); i++) {
         weaponsTexts[i]->setPosition(position);
         position.y += GH/10;
      }
      drawTexts(weaponsTexts);
   } else if(menuSelection == UPGRADES) {
      position.y = GH*(5.0/10.0);
      for(int i = 0; i < upgradesTexts.size(); i++) {
         upgradesTexts[i]->setPosition(position);
         position.y += GH/10;
      }
      drawTexts(upgradesTexts);
   }  else if(menuSelection == AMMO) {
      position.y = GH*(5.0/10.0);
      for(int i = 0; i < ammoTexts.size(); i++) {
         ammoTexts[i]->setPosition(position);
         position.y += GH/10;
      }
      drawTexts(ammoTexts);
   }  else if(menuSelection == SHIP) {
      position.y = GH*(5.0/10.0);
      for(int i = 0; i < shipTexts.size(); i++) {
         shipTexts[i]->setPosition(position);
         position.y += GH/10;
      }
      drawTexts(shipTexts);
   } else {
      std::cerr << "Menu selection error! Quitting..." << std::endl;
      exit(1);
   }
   
   drawLogo();
   
   SDL_GL_SwapBuffers();

}

void StoreMenu::drawTexts(std::vector<Text*> texts) {
   glPushMatrix();
      useOrtho();
      glDisable(GL_LIGHTING);
      
      for(int i = 0; i < texts.size(); i++) {
         texts[i]->draw();
      }

      glEnable(GL_LIGHTING);
      usePerspective();
   glPopMatrix();
}


void StoreMenu::drawLogo() {
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
}


/**
 * Handles the player pressing down a key
 */
void StoreMenu::keyDown(int key) {
   //do nothing if main menu is active
   if(mainMenu->menuActive) { return;} 
   
   if (key == SDLK_p)  {
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
   
   if(menuSelection == WEAPONS) {
   
   } else if(menuSelection == UPGRADES) {
   
   }  else if(menuSelection == AMMO) {
   
   }  else if(menuSelection == SHIP) {
   
   } else {
      std::cerr << "Menu selection error! Quitting..." << std::endl;
      exit(1);
   }
   
   
   if(menuTexts[DONE_STOREMENUINDEX]->mouseSelect(x,y)) {
      SDL_ShowCursor(SDL_DISABLE);
      menuActive = false;
      Music::stopMusic();
      Music::playMusic("Asteroids2.ogg");
   }
   
   if(menuTexts[WEAPONS_STOREMENUINDEX]->mouseSelect(x,y)) {
      menuSelection = WEAPONS;
   } 
   if(menuTexts[UPGRADES_STOREMENUINDEX]->mouseSelect(x,y)) {
      menuSelection = UPGRADES;
   } 
   if(menuTexts[AMMO_STOREMENUINDEX]->mouseSelect(x,y)) {
      menuSelection = AMMO;
   }
   if(menuTexts[SHIP_STOREMENUINDEX]->mouseSelect(x,y)) {
      menuSelection = SHIP;
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
   
   if(menuSelection == WEAPONS) {
      for(int i = 0; i < weaponsTexts.size(); i++) {
         weaponsTexts[i]->mouseHighlight(x,y);
      }
   } else if(menuSelection == UPGRADES) {
      for(int i = 0; i < upgradesTexts.size(); i++) {
         upgradesTexts[i]->mouseHighlight(x,y);
      }
   }  else if(menuSelection == AMMO) {
      for(int i = 0; i < ammoTexts.size(); i++) {
         ammoTexts[i]->mouseHighlight(x,y);
      }
   }  else if(menuSelection == SHIP) {
      for(int i = 0; i < shipTexts.size(); i++) {
         shipTexts[i]->mouseHighlight(x,y);
      }
   } else {
      std::cerr << "Menu selection error! Quitting..." << std::endl;
      exit(1);
   }
   
   
   for(int i = 0; i < menuTexts.size(); i++) {
      menuTexts[i]->mouseHighlight(x,y);
   }
}


