

#include "SDL.h"
#include "Utility/GlobalUtility.h"
#include "Menus/StoreMenu.h"
#include "Menus/MainMenu.h"
#include "Graphics/Texture.h"
#include "Items/AsteroidShip.h"
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
   
   SDL_Rect position = {0,0};
   std::string fontName = DEFAULT_FONT; 
   std::stringstream out;
   
   
   //Get shards, health and Done text onto menuTexts
   doneText = new Text("Done (n)", menuFont, position);
   doneText->selectable = true;
   menuTexts.push_back(doneText);
   
   shardsText = new Text("Shards: ", 0, "", menuFont, position);
   menuTexts.push_back(shardsText);
   
   maxHealthText = new Text("Max Health: ", 0, "", menuFont, position);
   menuTexts.push_back(maxHealthText);

   lifeText = new Text("Lives: ", 0, "", menuFont, position);
   menuTexts.push_back(lifeText);

   timerText = new Text("Timer: ", 0, "", menuFont, position);
   menuTexts.push_back(timerText);
   
   weaponsText = new Text("Weapons", menuFont, position);
   weaponsText->selectable = true;
   menuTexts.push_back(weaponsText);
   
   //ammoText = new Text("Ammo", menuFont, position);
   //ammoText->selectable = true;
   //menuTexts.push_back(ammoText);

   shipText = new Text("Ship", menuFont, position);
   shipText->selectable = true;
   menuTexts.push_back(shipText);
   
   
   
   //get weapon purchase text in weaponsTexts
   if (gameState->ship != NULL) {
      std::vector<Weapon*> weaponList = gameState->ship->getWeapons();
      for(unsigned int i = 0; i < weaponList.size(); i++) {
         weaponsTexts.push_back(new Text(weaponList.at(i)->weaponString(), menuFont, position));
      }
      
      for(unsigned int i = 0; i < weaponList.size(); i++) {
         ammoTexts.push_back(new Text(weaponList.at(i)->ammoString(), menuFont, position));
      }
   
      //get ship related text in shipTexts
      
      engineShipText = new Text(out.str(), menuFont, position);
      shipTexts.push_back(engineShipText);
      
      maxHealthShipText = new Text(out.str(), menuFont, position);
      shipTexts.push_back(maxHealthShipText);
      
      regenHealthShipText = new Text(out.str(), menuFont, position);
      shipTexts.push_back(regenHealthShipText);

      buyOneLifeText = new Text(out.str(), menuFont, position);
      shipTexts.push_back(buyOneLifeText);

      bankShipText = new Text(out.str(), menuFont, position);
      shipTexts.push_back(bankShipText);
      
      
       
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
      fboBegin();
      glDrawBuffer(OVERLAY_BUFFER);

      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      
      fboEnd();
   }
}

void StoreMenu::draw() {
   if (gameState->ship == NULL) {
      return;
   }
   
   if (gameSettings->useOverlay) {
      fboBegin();
      glDrawBuffer(OVERLAY_BUFFER);
   }

   SDL_Rect position;
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
   shardsText->updateBody(gameState->ship->bankedShards);
   shardsText->setPosition(position);
   
   //max health
   position.x = (Sint16) (gameSettings->GW*(2.4/10.0));
   position.y = (Sint16) (gameSettings->GH*(9.5/10.0));
   maxHealthText->updateBody((int)gameState->ship->healthMax);
   maxHealthText->setPosition(position);

   //lives
   position.x = (Sint16) (gameSettings->GW*(5.0/10.0));
   position.y = (Sint16) (gameSettings->GH*(9.5/10.0));
   lifeText->updateBody((int)gameState->ship->lives);
   lifeText->setPosition(position);
   
   
   //weapons
   position.x = (Sint16) (gameSettings->GW*(1.0/10.0));
   position.y = (Sint16) (gameSettings->GH*(3.5/10.0));
   weaponsText->setPosition(position);
   
   //ammo
   //position.y = (Sint16) (position.y + (gameSettings->GH/10));
   //ammoText->setPosition(position);
   
   //ship
   position.y = (Sint16) (position.y + (gameSettings->GH/10));
   shipText->setPosition(position);

   drawTexts(menuTexts);
   
   
   //set menu colors to white
   weaponsText->setColor(SDL_WHITE);
   //ammoText->setColor(SDL_WHITE);
   shipText->setColor(SDL_WHITE);
   
   //initialize the first position
   position.x = (Sint16) (gameSettings->GW*(3.0/10.0));
   position.y = (Sint16) (gameSettings->GH*(3.5/10.0));
   
   //get the list of weapons from ship
   std::vector<Weapon*> weaponList = gameState->ship->getWeapons();
   
   if(menuSelection == WEAPONS) {
      //make the weapons selection blue
      weaponsText->setColor(SDL_BLUE);
      //for each weapon, display the appropriate menu, set the selectability
      for(unsigned int i = 0; i < weaponList.size(); i++) {
         //if the weapon is enabled and the weapon is 
         if(!weaponList[i]->purchased) {
            weaponsTexts[i]->selectable = false;
            weaponsTexts[i]->disabled = true;
            continue; //continue so that this weapon is not included in the y++ calculation
         } else if (weaponList[i]->level == weaponList[i]->levelMax) {
            weaponsTexts[i]->selectable = false;
            weaponsTexts[i]->disabled = false;
         } else {
            weaponsTexts[i]->selectable = true;
            weaponsTexts[i]->disabled = false;
         }
         weaponsTexts[i]->setPosition(position);
         //if((position.y >= (gameSettings->GH/2) + (gameSettings->GH/4) + (gameSettings->GH/8) ) || 
         //   (position.y <= (gameSettings->GH/2) - (gameSettings->GH/4) + (gameSettings->GH/8) )) {
         //   weaponsTexts[i]->disabled = true;
         //}
         weaponsTexts[i]->updateBody(weaponList[i]->weaponString());
         position.y = (Sint16) (position.y + (gameSettings->GH/15));
      }
   
      drawTexts(weaponsTexts);
      
   } else if(menuSelection == AMMO) {
      //set menu color
      ammoText->setColor(SDL_BLUE);
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
      int nextEngineLevel = gameState->ship->engineLevel + 1;
      int nextRegenHealthLevel = gameState->ship->regenHealthLevel + 1;
      int nextBankLevel = gameState->ship->bankLevel + 1;
      int nextHealthMax = gameState->ship->healthMax + 1;
      //set menu colors
      shipText->setColor(SDL_BLUE);
      
      position.y = (Sint16) ((gameSettings->GH*(3.5/10.0)));
      engineShipText->setPosition(position);
      position.y = (Sint16) ((position.y + (gameSettings->GH/15)));
      maxHealthShipText->setPosition(position);
      position.y = (Sint16) ((position.y + (gameSettings->GH/15)));
      regenHealthShipText->setPosition(position);
      position.y = (Sint16) ((position.y + (gameSettings->GH/15)));
      buyOneLifeText->setPosition(position);
      position.y = (Sint16) ((position.y + (gameSettings->GH/15)));
      bankShipText->setPosition(position);


      // Update ship text
      
      // buy engine upgrade
      out.str(""); 
      if (nextEngineLevel <= gameState->ship->engineMaxLevel()) {
         out << "Buy Engine Lvl " << nextEngineLevel << 
            " for " << gameState->ship->engineUpgradePrice();
         engineShipText->selectable = true;
      } else {
         out << "Can't upgrade engine anymore!";
         engineShipText->selectable = false;
      }
      engineShipText->updateBody(out.str());

      out.str(""); 

      // buy upgrade max health
      out.str(""); 
      if (nextHealthMax <= gameState->ship->healthLevelMax()) {
      out << "Upgrade Max Health to " << 
         gameState->ship->healthNextLevelAmount() << " for " << 
         gameState->ship->healthMaxUpgradePrice();
         maxHealthShipText->selectable = true;
      } else {
         out << "Can't upgrade max health anymore!";
         maxHealthShipText->selectable = false;
      }
      maxHealthShipText->updateBody(out.str());


      // buy regen health upgrade
      out.str(""); 
      if (nextRegenHealthLevel <= gameState->ship->regenHealthLevelMax()) {
         out << "Buy Regen Health Lvl " << nextRegenHealthLevel << " for " << gameState->ship->regenUpgradePrice();
         regenHealthShipText->selectable = true;
      } else {
         out << "Can't upgrade regen anymore!";
         regenHealthShipText->selectable = false;
      }
      regenHealthShipText->updateBody(out.str());

      // buy life
      out.str(""); 
      out << "Buy one life for " << gameState->ship->lifePrice();
      buyOneLifeText->selectable = true;
      buyOneLifeText->updateBody(out.str());

      // buy bank speed upgrade
      out.str(""); 
      if (nextBankLevel <= gameState->ship->bankLevelMax()) {
         out << "Buy Bank Speed Lvl " << (gameState->ship->bankLevel+1) <<  " for " << gameState->ship->bankUpgradePrice();
         bankShipText->selectable = true;
      } else {
         out << "Can't upgrade bank speed anymore!";
         bankShipText->selectable = false;
      }
      bankShipText->updateBody(out.str());      
      
      drawTexts(shipTexts);
      
   } else {
      std::cerr << "Menu selection error! Quitting..." << std::endl;
      exit(1);
   }
   
   Image::getImage("StoreMenuLogo")->drawImage();
   fboEnd();
   usePerspective();

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
void StoreMenu::keyDown(int key, int unicode) {
   //do nothing if main menu is active
   if(mainMenu->menuActive) { return; } 
   
   if (key == SDLK_p)  {
      if(menuActive){
         SDL_ShowCursor(SDL_DISABLE);
         menuActive = false;
         clearOverlay();
         SoundEffect::stopAllSoundEffect();
         SoundEffect::playMusic("Asteroids2.ogg");
         // Take care of repeated logic with the level timer.
         handleLevelTimer();
      } else {
         menuActive = true;
         SoundEffect::stopAllSoundEffect();
         SoundEffect::playMusic("8-bit3.ogg");
         // Pause the level timer.
         gameState->pauseLevelTimer();
      }
   }
   if (!menuActive) { return; }
   
   switch(key) {
    case SDLK_n:
       donePressed();
      break;
   case SDLK_F10:
      gameState->ship->bankedShards += 10;
      break;
   }
}


void StoreMenu::donePressed() {
   SDL_ShowCursor(SDL_DISABLE);
   menuActive = false;
   clearOverlay();

   // Take care of repeated logic with the level timer.
   handleLevelTimer();
   SoundEffect::playMusic("Asteroids2.ogg");

   // Unlock whatever new weapon should be made available.
   gameState->ship->unlockWeapons();
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
         donePressed();
         return;
      }
      // select weapons menu
      if(weaponsText->mouseSelect(x,y)) {
         menuSelection = WEAPONS;
         return;
      } 
      // select ammo menu
      //if(ammoText->mouseSelect(x,y)) {
      //   menuSelection = AMMO;
      //   return;
      //}
      // select shipupgrade menu
      if(shipText->mouseSelect(x,y)) {
         menuSelection = SHIP;
         return;
      }
   
      //get the list of weapons from ship
      std::vector<Weapon*> weaponList = gameState->ship->getWeapons();
      int shardsOwned = gameState->ship->bankedShards;
      
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
                     gameState->ship->bankedShards -= weaponList[i]->ammoAmount;
                  } else {
                     std::cout << "not enough money to buy ammo for " << weaponList[i]->getName() << "!" << std::endl;
                  }
               }
            }
         }
      } else if(menuSelection == SHIP) {
         if(engineShipText->mouseSelect(x,y) && shardsOwned >= gameState->ship->engineUpgradePrice()) {
            gameState->ship->engineUpgrade();
         } else if(maxHealthShipText->mouseSelect(x,y) && shardsOwned >= gameState->ship->healthMaxUpgradePrice()) {
            gameState->ship->healthMaxUpgrade();
         } else if(regenHealthShipText->mouseSelect(x,y) && shardsOwned >= gameState->ship->regenUpgradePrice()) {
            gameState->ship->regenUpgrade();
         } else if(buyOneLifeText->mouseSelect(x,y) && shardsOwned >= gameState->ship->lifePrice()) {
            gameState->ship->buyLife();
         } else if(bankShipText->mouseSelect(x,y) && shardsOwned >= gameState->ship->bankUpgradePrice()) {
            gameState->ship->bankUpgrade();
         }
      } else {
         std::cerr << "Menu selection error! Quitting..." << std::endl;
         exit(1);
      }
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

