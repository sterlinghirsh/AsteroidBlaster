
#include <iostream>
#include "SDL.h"
#include "Utility/GlobalUtility.h"
#include "Utility/GameState.h"
#include "Menus/SettingsMenu.h"
#include "Graphics/Texture.h"
#include "Text/Text.h"
#include "Utility/SoundEffect.h"
#include "Items/AsteroidShip.h"
#include "Menus/MainMenu.h"

#define TITLE_INDEX 0
#define BLOOM_INDEX 2
#define OVERLAY_INDEX 4
#define DEFER_INDEX 6
#define MOUSE_CAPTURE_INDEX 8
#define FULLSCREEN_INDEX 10
#define MINIMAP_INDEX 12
#define CAMERA_VIEW_INDEX 14
#define MUSIC_INDEX 16
#define SFX_INDEX 18
#define RETURN_INDEX (menuTexts.size() - 1)

#define TITLE_TYPE 0
#define SINGLE_SELECTABLE_TYPE 1
#define LEFT_TYPE 2
#define RIGHT_TYPE 3

SettingsMenu::SettingsMenu(GameState*& _gameState) : gameState(_gameState) {
   menuActive = false;
   x = y = -1;

   SDL_Rect position = {0,0};
   std::string fontName = DEFAULT_FONT;

   menuTexts.push_back(new Text("Settings",  menuFont, position));
   types.push_back(TITLE_TYPE);

   menuTexts.push_back(new Text("Bloom Lighting:",  menuFont, position));
   types.push_back(LEFT_TYPE);
   menuTexts.push_back(new Text(getStatus(gameSettings->bloom), menuFont, position));
   types.push_back(RIGHT_TYPE);

   menuTexts.push_back(new Text("Overlay Store:",  menuFont, position));
   types.push_back(LEFT_TYPE);
   menuTexts.push_back(new Text(getStatus(gameSettings->useOverlay), menuFont, position));
   types.push_back(RIGHT_TYPE);

   menuTexts.push_back(new Text("Deferred Rendering:",  menuFont, position));
   types.push_back(LEFT_TYPE);
   menuTexts.push_back(new Text(getStatus(gameSettings->drawDeferred), menuFont, position));
   types.push_back(RIGHT_TYPE);

   menuTexts.push_back(new Text("Mouse Capture:",  menuFont, position));
   types.push_back(LEFT_TYPE);
   menuTexts.push_back(new Text(getStatus(SDL_WM_GrabInput(SDL_GRAB_QUERY) == SDL_GRAB_ON), menuFont, position));
   types.push_back(RIGHT_TYPE);

   menuTexts.push_back(new Text("Fullscreen:",  menuFont, position));
   types.push_back(LEFT_TYPE);
   menuTexts.push_back(new Text(getStatus(gameSettings->fullscreen), menuFont, position));
   types.push_back(RIGHT_TYPE);

   menuTexts.push_back(new Text("Minimap:",  menuFont, position));
   types.push_back(LEFT_TYPE);
   menuTexts.push_back(new Text(getStatus(gameState->minimapOn()), menuFont, position));
   types.push_back(RIGHT_TYPE);

   menuTexts.push_back(new Text("Camera:",  menuFont, position));
   types.push_back(LEFT_TYPE);
   menuTexts.push_back(new Text(getViewStatus(gameState->ship->getCurrentView()), menuFont, position));
   types.push_back(RIGHT_TYPE);

   menuTexts.push_back(new Text("Music:",  menuFont, position));
   types.push_back(LEFT_TYPE);
   menuTexts.push_back(new Text(getStatus(gameSettings->musicOn), menuFont, position));
   types.push_back(RIGHT_TYPE);

   menuTexts.push_back(new Text("SFX:",  menuFont, position));
   types.push_back(LEFT_TYPE);
   menuTexts.push_back(new Text(getStatus(gameSettings->soundOn), menuFont, position));
   types.push_back(RIGHT_TYPE);

   menuTexts.push_back(new Text("Return", menuFont, position));
   types.push_back(SINGLE_SELECTABLE_TYPE);

   for (unsigned i = 0; i < menuTexts.size(); i++) {
      switch (types[i]) {
      case TITLE_TYPE:
         menuTexts[i]->alignment = CENTERED;
         break;
      case LEFT_TYPE:
         menuTexts[i]->selectable = false;
         menuTexts[i]->alignment = LEFT_ALIGN;
         break;
      case RIGHT_TYPE:
         if (!gameSettings->goodBuffers &&
               (i == BLOOM_INDEX || i == OVERLAY_INDEX || i == DEFER_INDEX)) {
            menuTexts[i]->disabled = true;
         } else {
            menuTexts[i]->disabled = false;
            menuTexts[i]->selectable = true;
         }
         menuTexts[i]->alignment = RIGHT_ALIGN;
         break;
      case SINGLE_SELECTABLE_TYPE:
         menuTexts[i]->selectable = true;
         menuTexts[i]->alignment = CENTERED;
         break;
      }
   }
}

SettingsMenu::~SettingsMenu() {
   for(unsigned i = 0; i < menuTexts.size(); i++) {
      delete menuTexts[i];
   }
}

std::string SettingsMenu::getStatus(bool status) {
   if (status) {
      return "Enabled";
   }
   return "Disabled";
}

std::string SettingsMenu::getViewStatus(int status) {
   switch (status) {
   case VIEW_FIRSTPERSON_SHIP:
      return "First Person Ship";
      break;
   case VIEW_THIRDPERSON_SHIP:
      return "Third Person Ship";
      break;
   case VIEW_FIRSTPERSON_GUN:
      return "First Person Gun";
      break;
   case VIEW_THIRDPERSON_GUN:
      return "Third Person Gun";
      break;
   }
   return "ERROR";
}

void SettingsMenu::draw() {
   menuTexts[BLOOM_INDEX]->textToDisplay =
      getStatus(gameSettings->bloom && gameSettings->drawDeferred);
   menuTexts[OVERLAY_INDEX]->textToDisplay =
      getStatus(gameSettings->useOverlay && gameSettings->drawDeferred);
   //menuTexts[OVERLAY_INDEX]->textToDisplay = getStatus(gameSettings->useOverlay);
   menuTexts[DEFER_INDEX]->textToDisplay = getStatus(gameSettings->drawDeferred);
   menuTexts[MOUSE_CAPTURE_INDEX]->textToDisplay = getStatus(SDL_WM_GrabInput(SDL_GRAB_QUERY) == SDL_GRAB_ON);
   menuTexts[FULLSCREEN_INDEX]->textToDisplay = getStatus(gameSettings->fullscreen);
   menuTexts[MINIMAP_INDEX]->textToDisplay = getStatus(gameState->minimapOn());
   menuTexts[CAMERA_VIEW_INDEX]->textToDisplay = getViewStatus(gameState->ship->getCurrentView());
   menuTexts[MUSIC_INDEX]->textToDisplay = getStatus(gameSettings->musicOn);
   menuTexts[SFX_INDEX]->textToDisplay = getStatus(gameSettings->soundOn);
   SDL_Rect position;
   position.y = (Sint16) (gameSettings->GH/20);
   for(unsigned i = 0; i < menuTexts.size(); i++) {
      switch (types[i]) {
      case TITLE_TYPE:
         position.y = (Sint16) (position.y + (gameSettings->GH/10));
         position.x = (Sint16) (gameSettings->GW/2);
         menuTexts[i]->setPosition(position);
         position.y = (Sint16) (position.y + (gameSettings->GH/10));
         break;
      case LEFT_TYPE:
         position.x = (Sint16) (gameSettings->GW/8);
         menuTexts[i]->setPosition(position);
         break;
      case RIGHT_TYPE:
         position.x = (Sint16) (gameSettings->GW * 7/8);
         menuTexts[i]->setPosition(position);
         position.y = (Sint16) (position.y + (gameSettings->GH/RETURN_INDEX / 0.75));
         break;
      case SINGLE_SELECTABLE_TYPE:
         position.x = (Sint16) (gameSettings->GW/2);
         menuTexts[i]->setPosition(position);
         position.y = (Sint16) (position.y + (gameSettings->GH/12));
         break;
      }
   }

   // Clear the screen
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

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

   glEnable(GL_LIGHTING);

   SDL_GL_SwapBuffers();

}

/**
 * Handles the player pressing down a key
 */
void SettingsMenu::keyDown(int key, int unicode) {
   return;
}

/**
 * Handles the player letting go of a key
 */
void SettingsMenu::keyUp(int key) {
   if (!menuActive) { return; }

}

/**
 * Handles the player clicking the mouse
 */
void SettingsMenu::mouseDown(int button) {
   if (!menuActive) { return; }
   
   if(menuTexts[BLOOM_INDEX]->mouseSelect(x,y)) {
      gameSettings->bloom = !gameSettings->bloom
         && gameSettings->goodBuffers;
   } else if(menuTexts[OVERLAY_INDEX]->mouseSelect(x,y)) {
      gameSettings->useOverlay = !gameSettings->useOverlay
         && gameSettings->goodBuffers;
   } else if(menuTexts[DEFER_INDEX]->mouseSelect(x,y)) {
      gameSettings->drawDeferred = !gameSettings->drawDeferred
         && gameSettings->goodBuffers;
   } else if(menuTexts[MOUSE_CAPTURE_INDEX]->mouseSelect(x,y)) {
      toggleGrabMode();
   } else if(menuTexts[FULLSCREEN_INDEX]->mouseSelect(x,y)) {
      toggleFullScreen();
   } else if(menuTexts[MINIMAP_INDEX]->mouseSelect(x,y)) {
      gameState->toggleMinimap();
   } else if(menuTexts[CAMERA_VIEW_INDEX]->mouseSelect(x,y)) {
      gameState->ship->nextView();
   } else if(menuTexts[SFX_INDEX]->mouseSelect(x,y)) {
      gameSettings->soundOn = !gameSettings->soundOn;
   } else if(menuTexts[MUSIC_INDEX]->mouseSelect(x,y)) {
      if (gameSettings->musicOn) {
         printf("Disabling music.\n");
         gameSettings->musicOn = false;
         SoundEffect::stopMusic();
      } else {
         printf("Enabling music.\n");
         gameSettings->musicOn = true;
         SoundEffect::playMusic("AsteroidsMenu2");
      }
   } else if(menuTexts[RETURN_INDEX]->mouseSelect(x,y)) {
      menuActive = false;
      mainMenu->menuActive = true;
      x = y = -1;
   }
   
   if (!gameSettings->drawDeferred) {
      menuTexts[BLOOM_INDEX]->disabled = true;
      menuTexts[OVERLAY_INDEX]->disabled = true;
   } else {
      menuTexts[BLOOM_INDEX]->disabled = false;
      menuTexts[OVERLAY_INDEX]->disabled = false;
   }
}

/**
 * Handles the player letting go of a mouse click
 */
void SettingsMenu::mouseUp(int button) {
   if (!menuActive) { return; }
}

void SettingsMenu::mouseMove(int dx, int dy, int _x, int _y) {
   //std::cout << "mouseMove=(" << _x << "," << _y << ")" << std::endl;
   if (!menuActive) { return; }
   x = _x;
   y = _y;
   //decide the color for each menu text
   menuTexts[BLOOM_INDEX]->mouseHighlight(x,y);
   menuTexts[OVERLAY_INDEX]->mouseHighlight(x,y);
   menuTexts[DEFER_INDEX]->mouseHighlight(x,y);
   menuTexts[MOUSE_CAPTURE_INDEX]->mouseHighlight(x,y);
   menuTexts[FULLSCREEN_INDEX]->mouseHighlight(x,y);
   menuTexts[MINIMAP_INDEX]->mouseHighlight(x,y);
   menuTexts[CAMERA_VIEW_INDEX]->mouseHighlight(x,y);
   menuTexts[SFX_INDEX]->mouseHighlight(x,y);
   menuTexts[MUSIC_INDEX]->mouseHighlight(x,y);
   menuTexts[RETURN_INDEX]->mouseHighlight(x,y);

}

void SettingsMenu::activate() {
   SDL_ShowCursor(SDL_ENABLE);
   menuActive = true;
   SoundEffect::stopAllSoundEffect();
   SoundEffect::playMusic("AsteroidsMenu2");
}

void SettingsMenu::deactivate() {
   SDL_ShowCursor(SDL_DISABLE);
   menuActive = false;
   SoundEffect::playMusic("Asteroids2.ogg");
}


