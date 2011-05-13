

#include "SDL.h"
#include "Utility/GlobalUtility.h"
#include "Menus/StoreMenu.h"
#include "Menus/MainMenu.h"
#include "Graphics/Texture.h"
#include "Items/AsteroidShip.h"
#include "Utility/Music.h"
#include "Utility/SoundEffect.h"
#include "Graphics/Image.h"
#include "Text/Text.h"
#include <iostream>


#define DEFAULTTIME 60

/**
 * We pass in a reference to a pointer to a gameState, 
 * so that if main() ever makes a new GameState, 
 * the pointer should still be valid.
 */
StoreMenu::StoreMenu(GameState*& _gameState) : gameState(_gameState) {
   menuActive = false;
   menuSelection = WEAPONS;
   
   x = y = -1;
   
   scrollWeapon = scrollAmmo = 0;
   
   SDL_Rect position = {0,0};
   std::string fontName = DEFAULT_FONT; 
   std::stringstream out;
   
   
   //Get shards, health and Done text onto menuTexts
   doneText = new Text("Done (n)", menuFont, position);
   doneText->selectable = true;
   menuTexts.push_back(doneText);
   
   shardsText = new Text("Shards: ", 0, "", menuFont, position);
   menuTexts.push_back(shardsText);
   
   healthText = new Text("Health: ", 0, "", menuFont, position);
   menuTexts.push_back(healthText);
   
   timerText = new Text("Timer: ", 0, "", menuFont, position);
   menuTexts.push_back(timerText);
   
   weaponsText = new Text("Weapons", menuFont, position);
   weaponsText->selectable = true;
   menuTexts.push_back(weaponsText);
   
   ammoText = new Text("Ammo", menuFont, position);
   ammoText->selectable = true;
   menuTexts.push_back(ammoText);

   shipText = new Text("Ship", menuFont, position);
   shipText->selectable = true;
   menuTexts.push_back(shipText);
   
   
   
   //get weapon purchase text in weaponsTexts
   std::vector<Weapon*> weaponList = gameState->ship->getWeapons();
   for(unsigned int i = 0; i < weaponList.size(); i++) {
      weaponsTexts.push_back(new Text(weaponList.at(i)->weaponString(), menuFont, position));
   }
   
   for(unsigned int i = 0; i < weaponList.size(); i++) {
      ammoTexts.push_back(new Text(weaponList.at(i)->ammoString(), menuFont, position));
   }
   
   //get ship related text in shipTexts
   
   out.str(""); 
   out << "Upgrade Engine $" << gameState->ship->enginePrice;
   engineShipText = new Text(out.str(), menuFont, position);
   shipTexts.push_back(engineShipText);
   
   out.str(""); 
   out << "Upgrade Max Health $" << gameState->ship->healthUpgradePrice;
   maxHealthShipText = new Text(out.str(), menuFont, position);
   shipTexts.push_back(maxHealthShipText);
   
   out.str(""); 
   out << "Upgrade Regen Health $" << gameState->ship->regenHealthUpgradePrice;
   regenHealthShipText = new Text(out.str(), menuFont, position);
   shipTexts.push_back(regenHealthShipText);
   
   
   
    
   for(unsigned i = 0; i < weaponsTexts.size(); i++) {
      weaponsTexts[i]->selectable = true;
   }
   for(unsigned i = 0; i < ammoTexts.size(); i++) {
      ammoTexts[i]->selectable = true;
   }
   for(unsigned i = 0; i < shipTexts.size(); i++) {
      shipTexts[i]->selectable = true;
   }
}


StoreMenu::~StoreMenu() {
   for(unsigned i = 0; i < menuTexts.size(); i++) {
      delete menuTexts[i];
   }
   for(unsigned i = 0; i < weaponsTexts.size(); i++) {
      delete weaponsTexts[i];
   }
   for(unsigned i = 0; i < ammoTexts.size(); i++) {
      delete ammoTexts[i];
   }
   for(unsigned i = 0; i < shipTexts.size(); i++) {
      delete shipTexts[i];
   }
}

void StoreMenu::clearOverlay() {
   if (gameSettings->useOverlay) {
      glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fbo);
      glDrawBuffer(GL_COLOR_ATTACHMENT3_EXT);

      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      
      glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
   }
}

void StoreMenu::draw() {
   if (gameSettings->useOverlay) {
      glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fbo);
      glDrawBuffer(GL_COLOR_ATTACHMENT3_EXT);
   }
   
   SDL_Rect position;
   position.x = (Sint16) (gameSettings->GW/3);
   position.y = (Sint16) (gameSettings->GH/2);
   std::stringstream out;

   // Clear the screen
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   
   //Done
   position.x = (Sint16) (gameSettings->GW*(8.0/10.0));
   position.y = (Sint16) (gameSettings->GH*(9.5/10.0));
   doneText->setPosition(position);
   
   //shards
   position.x = (Sint16) (gameSettings->GW*(0.1/10.0));
   position.y = (Sint16) (gameSettings->GH*(9.5/10.0));
   shardsText->updateBody(gameState->ship->nShards);
   shardsText->setPosition(position);
   
   //health
   position.x = (Sint16) (gameSettings->GW*(2.0/10.0));
   position.y = (Sint16) (gameSettings->GH*(9.5/10.0));
   healthText->updateBody((int)gameState->ship->health);
   healthText->setPosition(position);
   
   
   //weapons
   position.x = (Sint16) (gameSettings->GW*(1.0/10.0));
   position.y = (Sint16) (gameSettings->GH*(5.0/10.0));
   weaponsText->setPosition(position);
   
   //ammo
   position.y = (Sint16) (position.y + (gameSettings->GH/10));
   ammoText->setPosition(position);
   
   //ship
   position.y = (Sint16) (position.y + (gameSettings->GH/10));
   shipText->setPosition(position);

   drawTexts(menuTexts);
   
   
   //set menu colors to white
   weaponsText->setColor(SDL_WHITE);
   ammoText->setColor(SDL_WHITE);
   shipText->setColor(SDL_WHITE);
   
   //initialize the first position
   position.x = (Sint16) (gameSettings->GW*(3.0/10.0));
   position.y = (Sint16) (gameSettings->GH*(5.0/10.0));
   
   //get the list of weapons from ship
   std::vector<Weapon*> weaponList = gameState->ship->getWeapons();
   
   if(menuSelection == WEAPONS) {
      //make the weapons selection blue
      weaponsText->setColor(SDL_BLUE);
      //add scroll value to the y position
      position.y = (Sint16) (position.y + (scrollWeapon/1));
      //for each weapon, display the appropriate menu, set the selectability
      for(unsigned int i = 0; i < weaponList.size(); i++) {
         if(weaponList[i]->purchased && weaponList[i]->level == weaponList[i]->levelMax) {
            weaponsTexts[i]->selectable = false;
            weaponsTexts[i]->disabled = true;
            continue; //continue so that this weapon is not included in the y++ calculation
         } else {
            weaponsTexts[i]->selectable = true;
            weaponsTexts[i]->disabled = false;
         }
         weaponsTexts[i]->setPosition(position);
         if((position.y >= (gameSettings->GH/2) + (gameSettings->GH/4) + (gameSettings->GH/8) ) || 
            (position.y <= (gameSettings->GH/2) - (gameSettings->GH/4) + (gameSettings->GH/8) )) {
            weaponsTexts[i]->disabled = true;
         }
         weaponsTexts[i]->updateBody(weaponList[i]->weaponString());
         position.y = (Sint16) (position.y + (gameSettings->GH/10));
      }
   
      drawTexts(weaponsTexts);
      
   } else if(menuSelection == AMMO) {
      //set menu color
      ammoText->setColor(SDL_BLUE);
      //add scroll value to the y position
      position.y = (Sint16) (position.y + (scrollAmmo/1));
      //for each weapon, display the appropriate menu, set the selectability
      for(unsigned int i = 0; i < weaponList.size(); i++) {
         if(weaponList[i]->curAmmo == -1 || !weaponList[i]->purchased) {
            ammoTexts[i]->selectable = false;
            ammoTexts[i]->disabled = true;
            continue; //continue so that this weapon is not included in the y++ calculation
         } else {
            ammoTexts[i]->selectable = true;
            ammoTexts[i]->disabled = false;
         }
         ammoTexts[i]->setPosition(position);
         if((position.y >= (gameSettings->GH/2) + (gameSettings->GH/4) + (gameSettings->GH/8) ) || 
            (position.y <= (gameSettings->GH/2) - (gameSettings->GH/4) + (gameSettings->GH/8) )) {
            ammoTexts[i]->disabled = true;
         }
         ammoTexts[i]->updateBody(weaponList[i]->ammoString());
         position.y = (Sint16) (position.y + (gameSettings->GH/10));
      }
      
      drawTexts(ammoTexts);
      
      
   }  else if(menuSelection == SHIP) {
      int nextEngineLevel = gameState->ship->engineUpgrade + 1;
      int nextRegenHealthLevel = gameState->ship->regenHealthLevel + 1;
      //set menu colors
      shipText->setColor(SDL_BLUE);
      
      position.y = (Sint16) ((gameSettings->GH*(5.0/10.0)));
      engineShipText->setPosition(position);
      position.y = (Sint16) ((position.y + (gameSettings->GH/10)));
      maxHealthShipText->setPosition(position);
      position.y = (Sint16) ((position.y + (gameSettings->GH/10)));
      regenHealthShipText->setPosition(position);
      
      // buy engine upgrade
      out.str(""); 
      if (nextEngineLevel <= gameState->ship->engineMax) {
         out << "Upgrade Engine Level to " << nextEngineLevel << " $" << nextEngineLevel*gameState->ship->enginePrice;
         engineShipText->selectable = true;
      } else {
         out << "Can't upgrade engine anymore!";
         engineShipText->selectable = false;
      }
      engineShipText->updateBody(out.str());
      // buy regen health upgrade
      out.str(""); 
      if (nextRegenHealthLevel <= gameState->ship->regenHealthLevelMax) {
         out << "Upgrade Regen Health Level to " << nextRegenHealthLevel << " $" << nextRegenHealthLevel*gameState->ship->regenHealthUpgradePrice;
         regenHealthShipText->selectable = true;
      } else {
         out << "Can't upgrade regen anymore!";
         regenHealthShipText->selectable = false;
      }
      regenHealthShipText->updateBody(out.str());
      
      out.str(""); 
      out << "Upgrade Max Health to " << gameState->ship->healthMax + gameState->ship->healthUpgradeAmount << " $" << gameState->ship->healthMaxUpgradePrice() << " (" << gameState->ship->healthMax << ")";
      
      maxHealthShipText->updateBody(out.str());
      
      
      drawTexts(shipTexts);
      
   } else {
      std::cerr << "Menu selection error! Quitting..." << std::endl;
      exit(1);
   }
   
   Image::getImage("StoreMenuLogo")->drawImage();
   glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
   usePerspective();
   glPopMatrix();

   // We swap buffers in gameSettings if we're using the overlay.
   if (!gameSettings->useOverlay) {
      SDL_GL_SwapBuffers();
   }
}

void StoreMenu::drawTexts(std::vector<Text*> texts) {
   glPushMatrix();
      useOrtho();
      glDisable(GL_LIGHTING);
      
      for(unsigned i = 0; i < texts.size(); i++) {
         if(!texts[i]->disabled) {
            texts[i]->draw();
         }
      }

      glEnable(GL_LIGHTING);
      usePerspective();
   glPopMatrix();
}

void StoreMenu::handleLevelTimer() {
   /* If the level is over and we're exiting the buying menu, then set 
    * the timer again and mark that we're on a new level.
    */
   if (gameState->levelOver) {
      gameState->setLevelTimer();
      gameState->levelOver = false;
   }

   // Resume the level timer again since it was paused when we entered the menu.
   gameState->resumeLevelTimer();
}

/**
 * Handles the player pressing down a key
 */
void StoreMenu::keyDown(int key) {
   //do nothing if main menu is active
   if(mainMenu->menuActive) { return; } 
   
   if (key == SDLK_p)  {
      if(menuActive){
         SDL_ShowCursor(SDL_DISABLE);
         menuActive = false;
         clearOverlay();
         SoundEffect::stopAllSoundEffect();
         Music::playMusic("Asteroids2.ogg");
         // Take care of repeated logic with the level timer.
         handleLevelTimer();
      } else {
         menuActive = true;
         SoundEffect::stopAllSoundEffect();
         Music::playMusic("8-bit3.ogg");
         // Pause the level timer.
         gameState->pauseLevelTimer();
      }
   }
   if (!menuActive) { return; }
   
   switch(key) {
    case SDLK_n:
      SDL_ShowCursor(SDL_DISABLE);
      menuActive = false;
      clearOverlay();
      // Take care of repeated logic with the level timer.
      handleLevelTimer();

      Music::playMusic("Asteroids2.ogg");
      break;
   case SDLK_F10:
      gameState->ship->nShards += 10;
      gameState->ship->bankedShards += 10;
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
   
   //left
   if(button == 1) {
      //if done was pushed, quit out of the menu
      if(doneText->mouseSelect(x,y)) {
         SDL_ShowCursor(SDL_DISABLE);
         menuActive = false;
         clearOverlay();
         // Take care of repeated logic with the level timer.
         handleLevelTimer();
         Music::playMusic("Asteroids2.ogg");
         return;
      }
      // select weapons menu
      if(weaponsText->mouseSelect(x,y)) {
         menuSelection = WEAPONS;
         return;
      } 
      // select ammo menu
      if(ammoText->mouseSelect(x,y)) {
         menuSelection = AMMO;
         return;
      }
      // select shipupgrade menu
      if(shipText->mouseSelect(x,y)) {
         menuSelection = SHIP;
         return;
      }
   
      //get the list of weapons from ship
      std::vector<Weapon*> weaponList = gameState->ship->getWeapons();
      int &shardsOwned = gameState->ship->nShards;
      
      //if weapon menu section is selected
      if(menuSelection == WEAPONS) {
         //for each of the items in the menu
         for(unsigned int i = 0; i < weaponList.size(); i++) {
            //if mouse is actually clicked and it is enabled
            if(weaponsTexts[i]->mouseSelect(x,y) && weaponsTexts[i]->selectable) {
               // if not purchased
               if(!weaponList[i]->purchased) {
                  // if you have enough money
                  if(shardsOwned >= weaponList[i]->buyPrice()) {
                     std::cout << "bought " << weaponList[i]->getName() << "!" << std::endl;
                     gameState->ship->nShards -= weaponList[i]->buyPrice();
                     gameState->ship->bankedShards -= weaponList[i]->buyPrice();
                     // Will we be buying stuff in the middle of the game?
                     weaponList[i]->purchased = true;
                  // not enough money
                  } else {
                     // TODO: Make this print in the menu.
                     std::cout << "not enough money to buy " << weaponList[i]->getName() << "!" << std::endl;
                  }
               // if it is purchased, and you have enough money, upgrade
               } else if(shardsOwned >= weaponList[i]->buyPrice()) {
                     std::cout << "upgraded " << weaponList[i]->getName() << "!" << std::endl;
                     gameState->ship->nShards -= weaponList[i]->buyPrice();
                     gameState->ship->bankedShards -= weaponList[i]->buyPrice();
                     weaponList[i]->level++;
               //not enough money
               } else {
                  std::cout << "not enough money to upgrade " << weaponList[i]->getName() << "!" << std::endl;
               }
            }
         }
      } else if(menuSelection == AMMO) {
         //for each of the items in the menu
         for(unsigned int i = 0; i < weaponList.size(); i++) {
            //if mouse is actually clicked and it is enabled
            if(ammoTexts[i]->mouseSelect(x,y) && ammoTexts[i]->selectable) {
               // if ammo not infinate
               if(weaponList[i]->curAmmo != -1) {
                  // if have enough money for ammo
                  if(shardsOwned >= weaponList[i]->ammoPrice) {
                     weaponList[i]->curAmmo += weaponList[i]->ammoAmount;
                     gameState->ship->nShards -= weaponList[i]->ammoAmount;
                     gameState->ship->bankedShards -= weaponList[i]->ammoAmount;
                  } else {
                     std::cout << "not enough money to buy ammo for " << weaponList[i]->getName() << "!" << std::endl;
                  }
               }
            }
         }
      } else if(menuSelection == SHIP) {
         double health = gameState->ship->health;
         int nextEngineLevel = gameState->ship->engineUpgrade + 1;
         int nextRegenHealthLevel = gameState->ship->regenHealthLevel + 1;
         if(engineShipText->mouseSelect(x,y) && shardsOwned >= gameState->ship->enginePrice*nextEngineLevel) {
            gameState->ship->nShards -= gameState->ship->enginePrice*nextEngineLevel;
            gameState->ship->bankedShards -= gameState->ship->enginePrice*nextEngineLevel;
            gameState->ship->engineUpgrade += 1;
         } else if(maxHealthShipText->mouseSelect(x,y) && shardsOwned >= gameState->ship->healthMaxUpgradePrice()) {
            gameState->ship->nShards -= gameState->ship->healthMaxUpgradePrice();
            gameState->ship->bankedShards -= gameState->ship->healthMaxUpgradePrice();
            gameState->ship->healthMax += gameState->ship->healthUpgradeAmount;
         } else if(regenHealthShipText->mouseSelect(x,y) && shardsOwned >= nextRegenHealthLevel*gameState->ship->regenHealthUpgradePrice) {
            gameState->ship->nShards -= nextRegenHealthLevel*gameState->ship->regenHealthUpgradePrice;
            gameState->ship->bankedShards -= nextRegenHealthLevel*gameState->ship->regenHealthUpgradePrice;
            gameState->ship->regenHealthLevel += 1;
         }
      } else {
         std::cerr << "Menu selection error! Quitting..." << std::endl;
         exit(1);
      }
   } else if(button == 4) {
      if(menuSelection == WEAPONS) {
         scrollWeapon += 7;
      } else if(menuSelection == AMMO) {
         scrollAmmo += 7;
      }
      return;
   } else if(button == 5) {
      if(menuSelection == WEAPONS) {
         scrollWeapon -= 7;
      } else if(menuSelection == AMMO) {
         scrollAmmo -= 7;
      }
      return;
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
      for(unsigned i = 0; i < weaponsTexts.size(); i++) {
         weaponsTexts[i]->mouseHighlight(x,y);
      }
   }  else if(menuSelection == AMMO) {
      for(unsigned i = 0; i < ammoTexts.size(); i++) {
         ammoTexts[i]->mouseHighlight(x,y);
      }
   }  else if(menuSelection == SHIP) {
      for(unsigned i = 0; i < shipTexts.size(); i++) {
         shipTexts[i]->mouseHighlight(x,y);
      }
   } else {
      std::cerr << "Menu selection error! Quitting..." << std::endl;
      exit(1);
   }
   
   for(unsigned i = 0; i < menuTexts.size(); i++) {
      menuTexts[i]->mouseHighlight(x,y);
   }
}

void StoreMenu::update(double timeDiff) {

}

