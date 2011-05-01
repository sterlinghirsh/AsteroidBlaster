
#include <iostream>
#include "SDL.h"
#include "Utility/GlobalUtility.h"
#include "Menus/StoreMenu.h"
#include "Graphics/Texture.h"
#include "Items/AsteroidShip.h"
#include "Utility/Music.h"
#include "Utility/SoundEffect.h"
#include "Graphics/Image.h"

// indexes

#define DEFAULTTIME 60

#define DONE_STOREMENUINDEX 0
#define SHARDS_STOREMENUINDEX 1
#define HEALTH_STOREMENUINDEX 2
#define TIMER_STOREMENUINDEX 3
#define WEAPONS_STOREMENUINDEX 4
#define AMMO_STOREMENUINDEX 5
#define SHIP_STOREMENUINDEX 6

#define BUYHEALTH_SHIPTEXTSINDEX 0
#define ENGINEUPGRADE_SHIPTEXTSINDEX 1
#define BUYMAXHEALTH_SHIPTEXTSINDEX 2


// prices
#define HEALTH_AMOUNT 10

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
   
   timeLeft = DEFAULTTIME;
   
   SDL_Rect position = {0,0};
   std::string fontName = DEFAULT_FONT; 
   std::stringstream out;
   
   
   //Get shards, health and Done text onto menuTexts
   menuTexts.push_back(new Text("Done (n)", menuFont, position));
   menuTexts[DONE_STOREMENUINDEX]->selectable = true;
   menuTexts.push_back(new Text("Shards: ", 0, "", menuFont, position));
   menuTexts.push_back(new Text("Health: ", 0, "", menuFont, position));
   menuTexts.push_back(new Text("Timer: ", 0, "", menuFont, position));
   
   menuTexts.push_back(new Text("Weapons", menuFont, position));
   menuTexts[WEAPONS_STOREMENUINDEX]->selectable = true;
   menuTexts.push_back(new Text("Ammo", menuFont, position));
   menuTexts[AMMO_STOREMENUINDEX]->selectable = true;
   menuTexts.push_back(new Text("Ship", menuFont, position));
   menuTexts[SHIP_STOREMENUINDEX]->selectable = true;
   
   
   
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
   out << "Buy Ship Health $" << gameState->ship->healthPrice;
   shipTexts.push_back(new Text(out.str(), menuFont, position));
   
   out.str(""); 
   out << "Upgrade Engine $" << gameState->ship->enginePrice;
   shipTexts.push_back(new Text(out.str(), menuFont, position));
   
   out.str(""); 
   out << "Upgrade Max Health $" << gameState->ship->healthUpgradePrice;
   shipTexts.push_back(new Text(out.str(), menuFont, position));
   
    
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

void StoreMenu::draw() {
   SDL_Rect position;
   position.x = (Sint16) (gameSettings->GW/3);
   position.y = (Sint16) (gameSettings->GH/2);
   std::stringstream out;

   // Clear the screen
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

   //timer
   position.x = (Sint16) (gameSettings->GW*(1.0/10.0));
   position.y = (Sint16) (gameSettings->GH*(1.0/10.0));
   menuTexts[TIMER_STOREMENUINDEX]->updateBody((int)timeLeft);
   //menuTexts[TIMER_STOREMENUINDEX]->setPosition(position);
   
   //Done
   position.x = (Sint16) (gameSettings->GW*(8.0/10.0));
   position.y = (Sint16) (gameSettings->GH*(9.5/10.0));
   menuTexts[DONE_STOREMENUINDEX]->setPosition(position);
   
   //shards
   position.x = (Sint16) (gameSettings->GW*(0.1/10.0));
   position.y = (Sint16) (gameSettings->GH*(9.5/10.0));
   menuTexts[SHARDS_STOREMENUINDEX]->updateBody(gameState->ship->nShards);
   menuTexts[SHARDS_STOREMENUINDEX]->setPosition(position);
   
   //health
   position.x = (Sint16) (gameSettings->GW*(2.0/10.0));
   position.y = (Sint16) (gameSettings->GH*(9.5/10.0));
   menuTexts[HEALTH_STOREMENUINDEX]->updateBody(gameState->ship->health);
   menuTexts[HEALTH_STOREMENUINDEX]->setPosition(position);
   
   
   //weapons
   position.x = (Sint16) (gameSettings->GW*(1.0/10.0));
   position.y = (Sint16) (gameSettings->GH*(5.0/10.0));
   menuTexts[WEAPONS_STOREMENUINDEX]->setPosition(position);
   
   //ammo
   position.y = (Sint16) (position.y + (gameSettings->GH/10));
   menuTexts[AMMO_STOREMENUINDEX]->setPosition(position);
   
   //ship
   position.y = (Sint16) (position.y + (gameSettings->GH/10));
   menuTexts[SHIP_STOREMENUINDEX]->setPosition(position);

   drawTexts(menuTexts);
   
   
   //set menu colors to white
   menuTexts[WEAPONS_STOREMENUINDEX]->setColor(SDL_WHITE);
   menuTexts[AMMO_STOREMENUINDEX]->setColor(SDL_WHITE);
   menuTexts[SHIP_STOREMENUINDEX]->setColor(SDL_WHITE);
   
   //initialize the first position
   position.x = (Sint16) (gameSettings->GW*(3.0/10.0));
   position.y = (Sint16) (gameSettings->GH*(5.0/10.0));
   
   //get the list of weapons from ship
   std::vector<Weapon*> weaponList = gameState->ship->getWeapons();
   
   if(menuSelection == WEAPONS) {
      //make the weapons selection blue
      menuTexts[WEAPONS_STOREMENUINDEX]->setColor(SDL_BLUE);
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
      menuTexts[AMMO_STOREMENUINDEX]->setColor(SDL_BLUE);
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
      //set menu colors
      menuTexts[SHIP_STOREMENUINDEX]->setColor(SDL_BLUE);
      
      position.y = (Sint16) ((gameSettings->GH*(5.0/10.0)));
      //health
      shipTexts[BUYHEALTH_SHIPTEXTSINDEX]->setPosition(position);
      position.y = (Sint16) ((position.y + (gameSettings->GH/10)));
      shipTexts[ENGINEUPGRADE_SHIPTEXTSINDEX]->setPosition(position);
      position.y = (Sint16) ((position.y + (gameSettings->GH/10)));
      shipTexts[BUYMAXHEALTH_SHIPTEXTSINDEX]->setPosition(position);
      
      // buy engine upgrade
      out.str(""); 
      if (nextEngineLevel <= 5) {
         out << "Upgrade Engine Level to " << nextEngineLevel << " $" << nextEngineLevel*gameState->ship->enginePrice;
         shipTexts[ENGINEUPGRADE_SHIPTEXTSINDEX]->selectable = true;
      } else {
         out << "Can't upgrade engine anymore!";
         shipTexts[ENGINEUPGRADE_SHIPTEXTSINDEX]->selectable = false;
      }
      shipTexts[ENGINEUPGRADE_SHIPTEXTSINDEX]->updateBody(out.str());
      
      out.str(""); 
      out << "Upgrade Max Health to " << gameState->ship->healthMax + gameState->ship->healthUpgradeAmount << " $" << gameState->ship->healthMaxUpgradePrice() << " (" << gameState->ship->healthMax << ")";
      
      shipTexts[BUYMAXHEALTH_SHIPTEXTSINDEX]->updateBody(out.str());
      
      
      drawTexts(shipTexts);
      
   } else {
      std::cerr << "Menu selection error! Quitting..." << std::endl;
      exit(1);
   }
   
   Image::getImage("StoreMenuLogo")->drawImage();
   SDL_GL_SwapBuffers();

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
         SoundEffect::stopAllSoundEffect();
         Music::stopMusic();
         Music::playMusic("Asteroids2.ogg");
         // Take care of repeated logic with the level timer.
         handleLevelTimer();
      } else {
         menuActive = true;
         SoundEffect::stopAllSoundEffect();
         Music::stopMusic();
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
      // Take care of repeated logic with the level timer.
      handleLevelTimer();

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
   
   //left
   if(button == 1) {
      //if done was pushed, quit out of the menu
      if(menuTexts[DONE_STOREMENUINDEX]->mouseSelect(x,y)) {
         timeLeft = timeLeft + DEFAULTTIME;
         SDL_ShowCursor(SDL_DISABLE);
         menuActive = false;
         // Take care of repeated logic with the level timer.
         handleLevelTimer();
         Music::stopMusic();
         Music::playMusic("Asteroids2.ogg");
         return;
      }
      // select weapons menu
      if(menuTexts[WEAPONS_STOREMENUINDEX]->mouseSelect(x,y)) {
         menuSelection = WEAPONS;
         return;
      } 
      // select ammo menu
      if(menuTexts[AMMO_STOREMENUINDEX]->mouseSelect(x,y)) {
         menuSelection = AMMO;
         return;
      }
      // select shipupgrade menu
      if(menuTexts[SHIP_STOREMENUINDEX]->mouseSelect(x,y)) {
         menuSelection = SHIP;
         return;
      }
   
      //get the list of weapons from ship
      std::vector<Weapon*> weaponList = gameState->ship->getWeapons();
      int shardsOwned = gameState->ship->nShards;
      
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
                     weaponList[i]->purchased = true;
                     gameState->ship->nShards -= weaponList[i]->buyPrice();
                  // not enough money
                  } else {
                     std::cout << "not enough money to buy " << weaponList[i]->getName() << "!" << std::endl;
                  }
               // if it is purchased, and you have enough money, upgrade
               } else if(shardsOwned >= weaponList[i]->buyPrice()) {
                     weaponList[i]->level++;
                     gameState->ship->nShards -= weaponList[i]->buyPrice();
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
                  } else {
                     std::cout << "not enough money to buy ammo for " << weaponList[i]->getName() << "!" << std::endl;
                  }
               }
            }
         }
      } else if(menuSelection == SHIP) {
         double health = gameState->ship->health;
         int nextEngineLevel = gameState->ship->engineUpgrade + 1;
         if(shipTexts[BUYHEALTH_SHIPTEXTSINDEX]->mouseSelect(x,y) && shardsOwned >= gameState->ship->healthPrice) {
            if(gameState->ship->healthMax <= (health + gameState->ship->healthAmount)) {
               gameState->ship->health = gameState->ship->healthMax;
            } else {
               gameState->ship->health += gameState->ship->healthAmount;
            }
            gameState->ship->nShards -= gameState->ship->healthPrice;
         } else if(shipTexts[ENGINEUPGRADE_SHIPTEXTSINDEX]->mouseSelect(x,y) && shardsOwned >= gameState->ship->enginePrice*nextEngineLevel) {
            gameState->ship->nShards -= gameState->ship->enginePrice*nextEngineLevel;
            gameState->ship->engineUpgrade += 1;
         } else if(shipTexts[BUYMAXHEALTH_SHIPTEXTSINDEX]->mouseSelect(x,y) && shardsOwned >= gameState->ship->healthMaxUpgradePrice()) {
            gameState->ship->nShards -= gameState->ship->healthMaxUpgradePrice();
            gameState->ship->healthMax += gameState->ship->healthUpgradeAmount;
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
   timeLeft -= timeDiff;
   /*if (timeLeft <= 0) {
      timeLeft = DEFAULTTIME;
      SDL_ShowCursor(SDL_DISABLE);
      menuActive = false;
      Music::stopMusic();
      Music::playMusic("Asteroids2.ogg");
   }*/
}

