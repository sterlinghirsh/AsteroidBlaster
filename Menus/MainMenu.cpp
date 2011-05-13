

#include "SDL.h"
#include "Utility/GlobalUtility.h"
#include "Utility/GameState.h"
#include "Menus/MainMenu.h"
#include "Graphics/Image.h"
#include "Utility/Music.h"
#include "Utility/SoundEffect.h"
#include "Items/AsteroidShip.h"
#include "Text/Text.h"
#include <iostream>
#include <fstream>

// Also defined in main.cpp
#define WORLD_SIZE 80.00

#define SAVEFILENAME "AsteroidBlaster.sav"

MainMenu::MainMenu(GameState* _mainGameState) {
   menuActive = false;
   firstTime = true;
   x = y = -1;

   SDL_Rect position = {0,0};
   std::string fontName = DEFAULT_FONT;

   newGameText = new Text("New Game (n)",  menuFont, position);
   menuTexts.push_back(newGameText);
   
   continueText = new Text("Continue (c)",  menuFont, position);
   menuTexts.push_back(continueText);
   
   loadGameText = new Text("Load Game",  menuFont, position);
   menuTexts.push_back(loadGameText);
   
   saveGameText = new Text("Save Game",  menuFont, position);
   menuTexts.push_back(saveGameText);
   
   settingsText = new Text("Settings",  menuFont, position);
   menuTexts.push_back(settingsText);
   
   helpText = new Text("Help",  menuFont, position);
   menuTexts.push_back(helpText);
   
   creditsText = new Text("Credits",  menuFont, position);
   menuTexts.push_back(creditsText);
   
   quitText = new Text("Quit (esc)", menuFont, position);
   menuTexts.push_back(quitText);

   for (unsigned i = 0; i < menuTexts.size(); i++) {
      menuTexts[i]->selectable = true;
      menuTexts[i]->alignment = CENTERED;
   }
   
   std::ifstream fin(SAVEFILENAME);
   if(fin) {
      loadGameText->setColor(SDL_WHITE);
      loadGameText->selectable = true;
   } else {
      loadGameText->setColor(SDL_GREY);
      loadGameText->selectable = true;
   }
   

   // Initialize the ship(s) in the background.
   menuGameState = new GameState(WORLD_SIZE, true);

   ship1 = new AsteroidShip(menuGameState);
   ship2 = new AsteroidShip(menuGameState);
   ship3 = new AsteroidShip(menuGameState);
   ship4 = new AsteroidShip(menuGameState);

   setupShips();

   menuGameState->custodian.add(ship1);
   menuGameState->custodian.add(ship2);
   menuGameState->custodian.add(ship3);
   menuGameState->custodian.add(ship4);

   mainGameState = _mainGameState;
   menuGameState->addScreens();
}


MainMenu::~MainMenu() {
   for(unsigned i = 0; i < menuTexts.size(); i++) {
      delete menuTexts[i];
   }
   delete menuGameState;
}

void MainMenu::draw() {
   SDL_Rect position;
   position.x = (Sint16) (gameSettings->GW/2);
   position.y = (Sint16) (gameSettings->GH/2.3);
   for(unsigned i = 0; i < menuTexts.size(); i++) {
      menuTexts[i]->setPosition(position);
      position.y = (Sint16) (position.y + (gameSettings->GH/15));
   }

   // make stuff selectable and greyed out depending on stuff
   continueText->selectable = !firstTime;
   saveGameText->selectable = !firstTime;
   if(firstTime) {
      continueText->setColor(SDL_GREY);
      saveGameText->setColor(SDL_GREY);
   } else {
      continueText->setColor(SDL_WHITE);
      saveGameText->setColor(SDL_WHITE);
   }
   
   

   drawGameState(menuGameState, true, 0, 0, 20,
         0, 0, -1,
         0, 1, 0);
/*
   // Clear the screen.
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

   glPushMatrix();
   // Draw glowing objects to a texture for the bloom effect.
   menuGameState->aspect = (float) (gameSettings->GW / gameSettings->GH);
   if (gameSettings->bloom) {
      glPushMatrix();
      gluLookAt(0, 0, 20, // Eye at origin.
            0, 0, -1, // X goes right, Z goes away.
            0, 1, 0); // Y goes up.

      glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fbo);
      glDrawBuffer(GL_COLOR_ATTACHMENT0_EXT);

      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      glPushMatrix();
      menuGameState->drawGlow();

      glPopMatrix();
      glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);

      menuGameState->hBlur();
      menuGameState->vBlur();

      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      glPopMatrix();
   }
   usePerspective();

   // Draw menu background stuff.

   gluLookAt(0, 0, 20, // Eye at origin.
         0, 0, -1, // X goes right, Z goes away.
         0, 1, 0); // Y goes up.

   // Draw background.
   menuGameState->draw();
   //drawGameState(menuGameState);

   glClear(GL_DEPTH_BUFFER_BIT);
   //menuGameState->drawScreens();
   if (gameSettings->bloom) {
      menuGameState->drawBloom();
   }
   */
   mainGameState->drawScreens();
   //menuGameState->drawScreens();

   glPopMatrix();

   // Clear the depth buffer to draw on top of the ship.
   glClear(GL_DEPTH_BUFFER_BIT);

   //draw the text
   glPushMatrix();
   useOrtho();
   glDisable(GL_CULL_FACE);
   glDisable(GL_LIGHTING);

   for(unsigned i = 0; i < menuTexts.size(); i++) {
      menuTexts[i]->draw();
   }

   usePerspective();
   glPopMatrix();

   Image::getImage("MainMenuLogo")->drawImage();

   SDL_GL_SwapBuffers();

}

/*
void MainMenu::draw(GameState* gameState) {
   // the time difference since last update call
   //double timeDiff = doubleTime() - lastDrawTime;

   //gameState->setCurFPS(1 / timeDiff);

   // Clear the screen
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
   usePerspective();

   glMatrixMode(GL_MODELVIEW);


   // Draw glowing objects to a texture for the bloom effect.
   gameState->aspect = (float)gameSettings->GW/(float)gameSettings->GH;
   if (gameSettings->bloom) {
      glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fbo);
      glDrawBuffer(GL_COLOR_ATTACHMENT0_EXT);

      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      glPushMatrix();
      gameState->drawGlow();

      glPopMatrix();
      glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);

      gameState->hBlur();
      gameState->vBlur();

      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   }

   usePerspective();
   // Draw the main screen
   glPushMatrix();
   gameState->draw();

   //Particle::drawParticles();
   //drawCrosshair();
   glPopMatrix();
}
*/

/**
 * Handles the player pressing down a key
 */
void MainMenu::keyDown(int key) {
   switch(key) {
   case SDLK_m:
      if(menuActive) {
         // If we're in the menu already, don't do anything when the player presses m.
      }
      else {
         mainGameState->pauseLevelTimer();
         activate();
      }
      break;
   case SDLK_n:
      if(menuActive) {
         gameDeactivate(false);
      }
      break;
   case SDLK_c:
      if(!firstTime) {
         deactivate();
         /* If there's no menu's left open after deactivating this one, then 
          * we should resume the timer. Otherwise, the Store menu should be in 
          * charge of resuming the timer when it is closed.
          */
         if (!storeMenu->menuActive) {
            mainGameState->resumeLevelTimer();
         }
      }
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

   if(newGameText->mouseSelect(x,y)) {
      gameDeactivate(false);
   } else if(continueText->mouseSelect(x,y) && !firstTime) {
      deactivate();
   } else if(settingsText->mouseSelect(x,y)) {
      menuActive = false;
      settingsMenu->menuActive = true;
   } else if(loadGameText->mouseSelect(x,y)) {
      gameDeactivate(true);
   } else if(saveGameText->mouseSelect(x,y)) {
      mainGameState->save();
   } else if(helpText->mouseSelect(x,y)) {
      menuActive = false;
      helpMenu->menuActive = true;
   } else if(creditsText->mouseSelect(x,y)) {
      menuActive = false;
      creditsMenu->menuActive = true;
      Music::playMusic("Careless_Whisper.ogg");
   } else if(quitText->mouseSelect(x,y)) {
      running = false;
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
   //decide the color for each menu text
   
   for(unsigned i = 0; i < menuTexts.size(); i++) {
      menuTexts[i]->mouseHighlight(x,y);
   }

   // -20 is the near plane. 3 units in front is a good target point for the ships.
   ship3->lookAt(p2wx(x), p2wy(y), 17, 0, 1, 0);
   ship4->lookAt(p2wx(x), p2wy(y), 17, 0, 1, 0);

}

void MainMenu::activate() {
   SDL_ShowCursor(SDL_ENABLE);
   menuActive = true;
   SoundEffect::stopAllSoundEffect();
   Music::playMusic("8-bit3.ogg");

   setupShips();
}

void MainMenu::deactivate() {
   SDL_ShowCursor(SDL_DISABLE);
   menuActive = false;
   Music::playMusic("Asteroids2.ogg");
}

void MainMenu::gameDeactivate(bool shouldLoad) {
   deactivate();
   firstTime = false;
   mainGameState->reset(shouldLoad);
   continueText->setColor(SDL_WHITE);
}

void MainMenu::update(double timeDiff) {
   Menu::update(timeDiff);

   ship1->setYawSpeed(0.1);

   if (ship2->position->y > 25) {
      ship2->setOrientation(0, -1, 0,
            -1, 0, 0);
   } else if (ship2->position->y < -25) {
      ship2->setOrientation(0, 1, 0,
            -1, 0, 0);
   }
   menuGameState->update(timeDiff);
}

void MainMenu::setupShips() {
   ship1->position->update(-15, 2, 5);
   ship1->setOrientation(1, 0, 0, 0, 1, 0);
   ship1->accelerateForward(1);

   ship2->position->update(15, -20, -20);
   ship2->setOrientation(0, 1, 0,
         -1, 0, 0);
   ship2->accelerateForward(1);
   ship2->setRollSpeed(0.2);

   ship3->position->update(8, -4, 0);

   ship4->position->update(-2, -3, 10);
}
