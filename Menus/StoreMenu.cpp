
#include <iostream>
#include "SDL.h"
#include "Utility/GlobalUtility.h"
#include "Menus/StoreMenu.h"
#include "Utility/Image.h"
#include "Utility/Texture.h"
#include "Items/AsteroidShip.h"
#include "Utility/Music.h"
#include "Utility/SoundEffect.h"

// indexes

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

#define BLASTER_UPGRADESTEXTSINDEX 0
#define RAILGUN_UPGRADESTEXTSINDEX 1
#define PIKACHUSWRATH_UPGRADESTEXTSINDEX 2

#define RAILGUN_AMMOTEXTSINDEX 0
#define PIKACHUSWRATH_AMMOTEXTSINDEX 1
#define ANTIINERTIA_AMMOTEXTSINDEX 2

#define BUYHEALTH_SHIPTEXTSINDEX 0
#define ENGINEUPGRADE_SHIPTEXTSINDEX 1

#define BLASTER_WEAPON_INDEX 0
#define RAILGUN_WEAPON_INDEX 1
#define PIKACHUSWRATH_WEAPON_INDEX 3
#define ANTIINERTIA_WEAPON_INDEX 6

// prices

#define BLASTERUPGRADE_PRICE 2
#define RAILGUNUPGRADE_PRICE 2
#define PIKACHUSWRATHUPGRADE_PRICE 2

#define RAILGUN_PRICE 4
#define PIKACHUSWRATH_PRICE 3
#define ANTIINERTIA_PRICE 2
#define ENGINEUPGRADE_PRICE 3
#define HEALTH_PRICE 1

#define HEALTH_AMOUNT 10
#define RAILGUN_AMMO_AMOUNT 15
#define PIKACHUSWRATH_AMMO_AMOUNT 500
#define ANTIINERTIA_AMMO_AMOUNT 500

#define RAILGUN_AMMO_PRICE 1
#define PIKACHUSWRATH_AMMO_PRICE 1
#define ANTIINERTIA_AMMO_PRICE 1

StoreMenu::StoreMenu() {
   menuActive = false;
   menuSelection = WEAPONS;
   
   x = y = -1;
   
   SDL_Rect position = {0,0};
   std::string fontName = DEFAULT_FONT; 
   std::stringstream out;
   
   
   //Get shards, health and Done text onto menuTexts
   menuTexts.push_back(new Text("Done (n)", menuFont, position));
   menuTexts[DONE_STOREMENUINDEX]->selectable = true;
   menuTexts.push_back(new Text("Shards: ", 0, "", menuFont, position));
   menuTexts.push_back(new Text("Health: ", 0, "", menuFont, position));
   
   menuTexts.push_back(new Text("Weapons", menuFont, position));
   menuTexts[WEAPONS_STOREMENUINDEX]->selectable = true;
   menuTexts.push_back(new Text("Upgrades", menuFont, position));
   menuTexts[UPGRADES_STOREMENUINDEX]->selectable = true;
   menuTexts.push_back(new Text("Ammo", menuFont, position));
   menuTexts[AMMO_STOREMENUINDEX]->selectable = true;
   menuTexts.push_back(new Text("Ship", menuFont, position));
   menuTexts[SHIP_STOREMENUINDEX]->selectable = true;
   
   
   
   //get weapon purchase text in weaponsTexts
   out << "Buy Railgun  $" << RAILGUN_PRICE;
   weaponsTexts.push_back(new Text(out.str(), menuFont, position));
   
   out.str(""); 
   out << "Buy Pikachu's Wrath $" << PIKACHUSWRATH_PRICE;
   weaponsTexts.push_back(new Text(out.str(), menuFont, position));
   
   out.str(""); 
   out << "Buy Anti Inertia Beam $" << ANTIINERTIA_PRICE;
   weaponsTexts.push_back(new Text(out.str(), menuFont, position));
   
   
   //get upgrade text in upgradesTexts
   out.str(""); 
   out << "Upgrade Blaster $";
   upgradesTexts.push_back(new Text(out.str(), menuFont, position));
   
   out.str(""); 
   out << "Upgrade Railgun $";
   upgradesTexts.push_back(new Text(out.str(), menuFont, position));
   
   out.str(""); 
   out << "Upgrade Pikachu's Wrath $";
   upgradesTexts.push_back(new Text(out.str(), menuFont, position));
   
   
   //get ammo text in ammoTexts
   out.str(""); 
   out << "Buy " << RAILGUN_AMMO_AMOUNT << " Railgun Ammo $" << RAILGUN_AMMO_PRICE;
   ammoTexts.push_back(new Text(out.str(), menuFont, position));
   
   out.str(""); 
   out << "Buy " << PIKACHUSWRATH_AMMO_AMOUNT << " Pikachu's Wrath Ammo $" << PIKACHUSWRATH_AMMO_PRICE;
   ammoTexts.push_back(new Text(out.str(), menuFont, position));
   
   out.str(""); 
   out << "Buy " << ANTIINERTIA_AMMO_AMOUNT << " Anti Inertia Beam Ammo $" << ANTIINERTIA_AMMO_PRICE;
   ammoTexts.push_back(new Text(out.str(), menuFont, position));
   
   //get ship related text in shipTexts
   out.str(""); 
   out << "Buy Ship Health $" << HEALTH_PRICE;
   shipTexts.push_back(new Text(out.str(), menuFont, position));
   
   out.str(""); 
   out << "Upgrade Engine $" <<ENGINEUPGRADE_PRICE;
   shipTexts.push_back(new Text(out.str(), menuFont, position));
    
    
   for(int i = 0; i < weaponsTexts.size(); i++) {
      weaponsTexts[i]->selectable = true;
   }
   for(int i = 0; i < upgradesTexts.size(); i++) {
      //upgradesTexts[i]->selectable = true;
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
   position.x = (Sint16) (GW/3);
   position.y = (Sint16) (GH/2);
   std::stringstream out;

   // Clear the screen
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


   //Done
   position.x = (Sint16) (GW*(8.0/10.0));
   position.y = (Sint16) (GH*(9.5/10.0));
   menuTexts[DONE_STOREMENUINDEX]->setPosition(position);
   
   //shards
   position.x = (Sint16) (GW*(0.1/10.0));
   position.y = (Sint16) (GH*(9.5/10.0));
   menuTexts[SHARDS_STOREMENUINDEX]->updateBody(gameState->ship->nShards);
   menuTexts[SHARDS_STOREMENUINDEX]->setPosition(position);
   
   //health
   position.x = (Sint16) (GW*(2.0/10.0));
   position.y = (Sint16) (GH*(9.5/10.0));
   menuTexts[HEALTH_STOREMENUINDEX]->updateBody(gameState->ship->health);
   menuTexts[HEALTH_STOREMENUINDEX]->setPosition(position);
   
   //weapons
   position.x = (Sint16) (GW*(1.0/10.0));
   position.y = (Sint16) (GH*(5.0/10.0));
   menuTexts[WEAPONS_STOREMENUINDEX]->setPosition(position);
   
   //upgrades
   position.y = (Sint16) (position.y + (GH/10));
   menuTexts[UPGRADES_STOREMENUINDEX]->setPosition(position);
   
   //weapons
   position.y = (Sint16) (position.y + (GH/10));
   menuTexts[AMMO_STOREMENUINDEX]->setPosition(position);
   
   //weapons
   position.y = (Sint16) (position.y + (GH/10));
   menuTexts[SHIP_STOREMENUINDEX]->setPosition(position);

   drawTexts(menuTexts);
   
   position.x = (Sint16) (GW*(3.0/10.0));
   position.y = (Sint16) (GH*(5.0/10.0));
   if(menuSelection == WEAPONS) {
      //set menu colors
      menuTexts[WEAPONS_STOREMENUINDEX]->setColor(SDL_BLUE);
      menuTexts[UPGRADES_STOREMENUINDEX]->setColor(SDL_WHITE);
      menuTexts[AMMO_STOREMENUINDEX]->setColor(SDL_WHITE);
      menuTexts[SHIP_STOREMENUINDEX]->setColor(SDL_WHITE);
      
      // railgun
      if(gameState->ship->getWeapon(RAILGUN_WEAPON_INDEX)->purchased) {
         out.str(""); 
         out << "Railgun Already Bought!";
         weaponsTexts[RAILGUN_WEAPONSTEXTSINDEX]->updateBody(out.str());
         weaponsTexts[RAILGUN_WEAPONSTEXTSINDEX]->selectable = false;
      } else {
         out.str(""); 
         out << "Buy Railgun  $" << RAILGUN_PRICE;
         weaponsTexts[RAILGUN_WEAPONSTEXTSINDEX]->updateBody(out.str());
         weaponsTexts[RAILGUN_WEAPONSTEXTSINDEX]->selectable = true;
      }
      weaponsTexts[RAILGUN_WEAPONSTEXTSINDEX]->setPosition(position);
      position.y = (Sint16) (position.y + (GH/10));
      
      // pikachu's wrath
      if(gameState->ship->getWeapon(PIKACHUSWRATH_WEAPON_INDEX)->purchased) {
         out.str(""); 
         out << "Pikachu's Wrath Already Bought!";
         weaponsTexts[PIKACHUSWRATH_WEAPONSTEXTSINDEX]->updateBody(out.str());
         weaponsTexts[PIKACHUSWRATH_WEAPONSTEXTSINDEX]->selectable = false;
      } else {
         out.str(""); 
         out << "Buy Pikachu's Wrath $" << PIKACHUSWRATH_PRICE;
         weaponsTexts[PIKACHUSWRATH_WEAPONSTEXTSINDEX]->updateBody(out.str());
         weaponsTexts[PIKACHUSWRATH_WEAPONSTEXTSINDEX]->selectable = true;
      }
      weaponsTexts[PIKACHUSWRATH_WEAPONSTEXTSINDEX]->setPosition(position);
      position.y = (Sint16) (position.y + (GH/10));
      
      // Anti Intertia
      if(gameState->ship->getWeapon(ANTIINERTIA_WEAPON_INDEX)->purchased) {
         out.str(""); 
         out << "Anti Intertia Beam Already Bought!";
         weaponsTexts[ANTIINERTIA_WEAPONSTEXTSINDEX]->updateBody(out.str());
         weaponsTexts[ANTIINERTIA_WEAPONSTEXTSINDEX]->selectable = false;
      } else {
         out.str(""); 
         out << "Anti Intertia Beam $" << ANTIINERTIA_PRICE;
         weaponsTexts[ANTIINERTIA_WEAPONSTEXTSINDEX]->updateBody(out.str());
         weaponsTexts[ANTIINERTIA_WEAPONSTEXTSINDEX]->selectable = true;
      }
      weaponsTexts[ANTIINERTIA_WEAPONSTEXTSINDEX]->setPosition(position);
   
      drawTexts(weaponsTexts);

   } else if(menuSelection == UPGRADES) {
      int wLevel = -1;
      //set menu colors
      menuTexts[WEAPONS_STOREMENUINDEX]->setColor(SDL_WHITE);
      menuTexts[UPGRADES_STOREMENUINDEX]->setColor(SDL_BLUE);
      menuTexts[AMMO_STOREMENUINDEX]->setColor(SDL_WHITE);
      menuTexts[SHIP_STOREMENUINDEX]->setColor(SDL_WHITE);
      
      position.y = (Sint16) (GH*(5.0/10.0));
      // buy blaster upgrade
      out.str(""); 
      wLevel = gameState->ship->getWeapon(BLASTER_WEAPON_INDEX)->level + 1;
      if (wLevel <= 5) {
         out << "Buy Upgrade Blaster Level " <<  wLevel << " $" << wLevel*BLASTERUPGRADE_PRICE;
         upgradesTexts[BLASTER_UPGRADESTEXTSINDEX]->selectable = true;
      } else {
         out << "Can't Upgrade Blaster Anymore!";
         upgradesTexts[BLASTER_UPGRADESTEXTSINDEX]->selectable = false;
      }
      upgradesTexts[BLASTER_UPGRADESTEXTSINDEX]->updateBody(out.str());
      upgradesTexts[BLASTER_UPGRADESTEXTSINDEX]->setPosition(position);
      position.y = (Sint16) (position.y + (GH/10));
      
      // buy railgun upgrade
      out.str(""); 
      wLevel = gameState->ship->getWeapon(RAILGUN_WEAPON_INDEX)->level + 1;
      if (gameState->ship->getWeapon(RAILGUN_WEAPON_INDEX)->purchased) {
         if (wLevel <= 5) {
            out << "Buy Upgrade Railgun Level " <<  wLevel << " $" << wLevel*RAILGUNUPGRADE_PRICE;
            upgradesTexts[RAILGUN_UPGRADESTEXTSINDEX]->selectable = true;
         } else {
            out << "Can't Upgrade Railgun Anymore!";
            upgradesTexts[RAILGUN_UPGRADESTEXTSINDEX]->selectable = false;
         }
      } else {
         out << "Buy Railgun first!";
         upgradesTexts[RAILGUN_UPGRADESTEXTSINDEX]->selectable = false;
      }
      upgradesTexts[RAILGUN_UPGRADESTEXTSINDEX]->updateBody(out.str());
      upgradesTexts[RAILGUN_UPGRADESTEXTSINDEX]->setPosition(position);
      position.y = (Sint16) (position.y + (GH/10));
      
      // buy pikachu's wrath upgrade
      out.str(""); 
      wLevel = gameState->ship->getWeapon(PIKACHUSWRATH_WEAPON_INDEX)->level + 1;
      if (gameState->ship->getWeapon(PIKACHUSWRATH_WEAPON_INDEX)->purchased) {
         if (wLevel <= 5) {
            out << "Buy Upgrade Pikachu's Wrath Level " <<  wLevel << " $" << wLevel*PIKACHUSWRATHUPGRADE_PRICE;
            upgradesTexts[PIKACHUSWRATH_UPGRADESTEXTSINDEX]->selectable = true;
         } else {
            out << "Can't Upgrade Pikachu's Wrath Anymore!";
            upgradesTexts[PIKACHUSWRATH_UPGRADESTEXTSINDEX]->selectable = false;
         }
      } else {
         out << "Buy Pikachu's Wrath first!";
         upgradesTexts[PIKACHUSWRATH_UPGRADESTEXTSINDEX]->selectable = false;
      }
      upgradesTexts[PIKACHUSWRATH_UPGRADESTEXTSINDEX]->updateBody(out.str());
      upgradesTexts[PIKACHUSWRATH_UPGRADESTEXTSINDEX]->setPosition(position);

      drawTexts(upgradesTexts);
      
   }  else if(menuSelection == AMMO) {
      //set menu colors
      menuTexts[WEAPONS_STOREMENUINDEX]->setColor(SDL_WHITE);
      menuTexts[UPGRADES_STOREMENUINDEX]->setColor(SDL_WHITE);
      menuTexts[AMMO_STOREMENUINDEX]->setColor(SDL_BLUE);
      menuTexts[SHIP_STOREMENUINDEX]->setColor(SDL_WHITE);
      
      int currAmmo = -1;
      position.y = (Sint16) (GH*(5.0/10.0));
      // railgun
      out.str(""); 
      if (gameState->ship->getWeapon(RAILGUN_WEAPON_INDEX)->purchased) {
         currAmmo = gameState->ship->getWeapon(RAILGUN_WEAPON_INDEX)->curAmmo;
         out << "Buy " << RAILGUN_AMMO_AMOUNT << " Railgun Ammo $" << RAILGUN_AMMO_PRICE << " (" << currAmmo << ")";
         ammoTexts[RAILGUN_AMMOTEXTSINDEX]->selectable = true;
      } else {
         out << "Buy Railgun first!";
         ammoTexts[RAILGUN_AMMOTEXTSINDEX]->selectable = false;
      }
      ammoTexts[RAILGUN_AMMOTEXTSINDEX]->updateBody(out.str());
      ammoTexts[RAILGUN_AMMOTEXTSINDEX]->setPosition(position);
      position.y = (Sint16) (position.y + (GH/10));
      
      // pikachu's wrath
      out.str(""); 
      if (gameState->ship->getWeapon(PIKACHUSWRATH_WEAPON_INDEX)->purchased) {
         currAmmo = gameState->ship->getWeapon(PIKACHUSWRATH_WEAPON_INDEX)->curAmmo;
         out << "Buy " << PIKACHUSWRATH_AMMO_AMOUNT << " Pikachu's Wrath Ammo $" << PIKACHUSWRATH_AMMO_PRICE << " (" << currAmmo << ")";
         ammoTexts[PIKACHUSWRATH_AMMOTEXTSINDEX]->selectable = true;
      } else {
         out << "Buy Pikachu's Wrath first!";
         ammoTexts[PIKACHUSWRATH_AMMOTEXTSINDEX]->selectable = false;
      }
      ammoTexts[PIKACHUSWRATH_AMMOTEXTSINDEX]->updateBody(out.str());
      ammoTexts[PIKACHUSWRATH_AMMOTEXTSINDEX]->setPosition(position);
      position.y = (Sint16) (position.y + (GH/10));
      
      // anti inertia beam
      out.str(""); 
      if (gameState->ship->getWeapon(ANTIINERTIA_WEAPON_INDEX)->purchased) {
         currAmmo = gameState->ship->getWeapon(ANTIINERTIA_WEAPON_INDEX)->curAmmo;
         out << "Buy " << ANTIINERTIA_AMMO_AMOUNT << " Anti Inertia Beam Ammo $" << ANTIINERTIA_AMMO_PRICE << " (" << currAmmo << ")";
         ammoTexts[ANTIINERTIA_AMMOTEXTSINDEX]->selectable = true;
      } else {
         out << "Buy Anti Inertia Beam first!";
         ammoTexts[ANTIINERTIA_AMMOTEXTSINDEX]->selectable = false;
      }
      ammoTexts[ANTIINERTIA_AMMOTEXTSINDEX]->updateBody(out.str());
      ammoTexts[ANTIINERTIA_AMMOTEXTSINDEX]->setPosition(position);
      position.y = (Sint16) (position.y + (GH/10));
      
      drawTexts(ammoTexts);
      
      
   }  else if(menuSelection == SHIP) {
      int nextEngineLevel = gameState->ship->engineUpgrade + 1;
      //set menu colors
      menuTexts[WEAPONS_STOREMENUINDEX]->setColor(SDL_WHITE);
      menuTexts[UPGRADES_STOREMENUINDEX]->setColor(SDL_WHITE);
      menuTexts[AMMO_STOREMENUINDEX]->setColor(SDL_WHITE);
      menuTexts[SHIP_STOREMENUINDEX]->setColor(SDL_BLUE);
      
      position.y = (Sint16) (GH*(5.0/10.0));
      //health
      shipTexts[BUYHEALTH_SHIPTEXTSINDEX]->setPosition(position);
      position.y = (Sint16) (position.y + (GH/10));
      
      // buy engine upgrade
      out.str(""); 
      if (nextEngineLevel <= 5) {
         out << "Buy Engine Upgrade Level " << nextEngineLevel << " $" << nextEngineLevel*ENGINEUPGRADE_PRICE;
         shipTexts[ENGINEUPGRADE_SHIPTEXTSINDEX]->selectable = true;
      } else {
         out << "Can't upgrade engine anymore!";
         shipTexts[ENGINEUPGRADE_SHIPTEXTSINDEX]->selectable = false;
      }
      shipTexts[ENGINEUPGRADE_SHIPTEXTSINDEX]->updateBody(out.str());
      shipTexts[ENGINEUPGRADE_SHIPTEXTSINDEX]->setPosition(position);
      
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
      glScaled( 0.3625, 0.1, 1.0 );
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
         menuActive = false;
         SoundEffect::stopAllSoundEffect();
         Music::stopMusic();
         Music::playMusic("Asteroids2.ogg");
      } else {
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
   
   int shardsOwned = gameState->ship->nShards;
   
   if(menuSelection == WEAPONS) {
      if(weaponsTexts[RAILGUN_WEAPONSTEXTSINDEX]->mouseSelect(x,y) && shardsOwned >= RAILGUN_PRICE) {
         gameState->ship->getWeapon(RAILGUN_WEAPON_INDEX)->purchased = true;
         gameState->ship->nShards -= RAILGUN_PRICE;
      } else if(weaponsTexts[PIKACHUSWRATH_WEAPONSTEXTSINDEX]->mouseSelect(x,y) && shardsOwned >= PIKACHUSWRATH_PRICE) {
         gameState->ship->getWeapon(PIKACHUSWRATH_WEAPON_INDEX)->purchased = true;
         gameState->ship->nShards -= PIKACHUSWRATH_PRICE;
      } else if(weaponsTexts[ANTIINERTIA_WEAPONSTEXTSINDEX]->mouseSelect(x,y) && shardsOwned >= ANTIINERTIA_PRICE) {
         gameState->ship->getWeapon(ANTIINERTIA_WEAPON_INDEX)->purchased = true;
         gameState->ship->nShards -= ANTIINERTIA_PRICE;
      }
   } else if(menuSelection == UPGRADES) {
      int level = gameState->ship->getWeapon(BLASTER_WEAPON_INDEX)->level + 1;
      if(upgradesTexts[BLASTER_UPGRADESTEXTSINDEX]->mouseSelect(x,y) && shardsOwned >= BLASTERUPGRADE_PRICE*level) {
         gameState->ship->nShards -= BLASTERUPGRADE_PRICE*level;
         gameState->ship->getWeapon(BLASTER_WEAPON_INDEX)->level += 1;
      }
      level = gameState->ship->getWeapon(RAILGUN_WEAPON_INDEX)->level + 1;
      if(upgradesTexts[RAILGUN_UPGRADESTEXTSINDEX]->mouseSelect(x,y) && shardsOwned >= RAILGUNUPGRADE_PRICE*level) {
         gameState->ship->nShards -= RAILGUNUPGRADE_PRICE*level;
         gameState->ship->getWeapon(RAILGUN_WEAPON_INDEX)->level += 1;
      }
      level = gameState->ship->getWeapon(PIKACHUSWRATH_WEAPON_INDEX)->level + 1;
      if(upgradesTexts[PIKACHUSWRATH_UPGRADESTEXTSINDEX]->mouseSelect(x,y) && shardsOwned >= PIKACHUSWRATHUPGRADE_PRICE*level) {
         gameState->ship->nShards -= PIKACHUSWRATHUPGRADE_PRICE*level;
         gameState->ship->getWeapon(PIKACHUSWRATH_WEAPON_INDEX)->level += 1;
      }
   } else if(menuSelection == AMMO) {
      if(ammoTexts[RAILGUN_AMMOTEXTSINDEX]->mouseSelect(x,y) && shardsOwned >= RAILGUN_AMMO_PRICE) {
         gameState->ship->getWeapon(RAILGUN_WEAPON_INDEX)->curAmmo += RAILGUN_AMMO_AMOUNT;
         gameState->ship->nShards -= RAILGUN_AMMO_PRICE;
      } else if(ammoTexts[PIKACHUSWRATH_AMMOTEXTSINDEX]->mouseSelect(x,y) && shardsOwned >= PIKACHUSWRATH_AMMO_PRICE) {
         gameState->ship->getWeapon(PIKACHUSWRATH_WEAPON_INDEX)->curAmmo += PIKACHUSWRATH_AMMO_AMOUNT;
         gameState->ship->nShards -= PIKACHUSWRATH_AMMO_PRICE;
      } else if(ammoTexts[ANTIINERTIA_AMMOTEXTSINDEX]->mouseSelect(x,y) && shardsOwned >= ANTIINERTIA_AMMO_PRICE) {
         gameState->ship->getWeapon(ANTIINERTIA_WEAPON_INDEX)->curAmmo += ANTIINERTIA_AMMO_AMOUNT;
         gameState->ship->nShards -= ANTIINERTIA_AMMO_PRICE;
      }
   } else if(menuSelection == SHIP) {
      int health = gameState->ship->health;
      int nextEngineLevel = gameState->ship->engineUpgrade + 1;
      if(shipTexts[BUYHEALTH_SHIPTEXTSINDEX]->mouseSelect(x,y) && shardsOwned >= HEALTH_PRICE) {
         if(health >= (100 - HEALTH_AMOUNT)) {
            gameState->ship->health = 100;
         } else {
            gameState->ship->health += HEALTH_AMOUNT;
         }
         gameState->ship->nShards -= HEALTH_PRICE;
      } else if(shipTexts[ENGINEUPGRADE_SHIPTEXTSINDEX]->mouseSelect(x,y) && shardsOwned >= ENGINEUPGRADE_PRICE*nextEngineLevel) {
         gameState->ship->nShards -= ENGINEUPGRADE_PRICE*nextEngineLevel;
         gameState->ship->engineUpgrade += 1;
      }
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


