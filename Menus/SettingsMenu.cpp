
#include <iostream>
#include "SDL.h"
#include "Utility/GlobalUtility.h"
#include "Utility/GameState.h"
#include "Menus/SettingsMenu.h"
#include "Utility/Image.h"
#include "Utility/Texture.h"
#include "Utility/Music.h"
#include "Utility/SoundEffect.h"

#define TITLE_INDEX 0
#define BLOOM_INDEX 2
#define MOUSE_CAPTURE_INDEX 4
#define FULLSCREEN_INDEX 6
#define MINIMAP_INDEX 8
#define CAMERA_VIEW_INDEX 10
#define RETURN_INDEX (menuTexts.size() - 1)

#define TITLE_TYPE 0
#define SINGLE_SELECTABLE_TYPE 1
#define LEFT_TYPE 2
#define RIGHT_TYPE 3

SettingsMenu::SettingsMenu() {
   menuActive = false;
   x = y = -1;

   SDL_Rect position = {0,0};
   std::string fontName = DEFAULT_FONT;

   menuTexts.push_back(new Text("Settings",  fontName, position, 48));
   types.push_back(TITLE_TYPE);
   menuTexts.push_back(new Text("Bloom:",  fontName, position, 24));
   types.push_back(LEFT_TYPE);
   menuTexts.push_back(new Text(getStatus(bloom), fontName, position, 24));
   types.push_back(RIGHT_TYPE);
   menuTexts.push_back(new Text("Mouse Capture:",  fontName, position, 24));
   types.push_back(LEFT_TYPE);
   menuTexts.push_back(new Text(getStatus(SDL_WM_GrabInput(SDL_GRAB_QUERY) == SDL_GRAB_ON), fontName, position, 24));
   types.push_back(RIGHT_TYPE);
   menuTexts.push_back(new Text("Fullscreen:",  fontName, position, 24));
   types.push_back(LEFT_TYPE);
   menuTexts.push_back(new Text(getStatus(fullscreen), fontName, position, 24));
   types.push_back(RIGHT_TYPE);
   menuTexts.push_back(new Text("Minimap:",  fontName, position, 24));
   types.push_back(LEFT_TYPE);
   menuTexts.push_back(new Text(getStatus(gameState->minimapOn()), fontName, position, 24));
   types.push_back(RIGHT_TYPE);
   menuTexts.push_back(new Text("Camera:",  fontName, position, 24));
   types.push_back(LEFT_TYPE);
   menuTexts.push_back(new Text(getViewStatus(gameState->ship->getCurrentView()), fontName, position, 24));
   types.push_back(RIGHT_TYPE);
   menuTexts.push_back(new Text("Return", fontName, position, 24));
   types.push_back(SINGLE_SELECTABLE_TYPE);

   for (int i = 0; i < menuTexts.size(); i++) {
      switch (types[i]) {
      case TITLE_TYPE:
         menuTexts[i]->alignment = CENTERED;
         break;
      case LEFT_TYPE:
         menuTexts[i]->selectable = false;
         menuTexts[i]->alignment = LEFT_ALIGN;
         break;
      case RIGHT_TYPE:
         menuTexts[i]->selectable = true;
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
   for(int i = 0; i < menuTexts.size(); i++) {
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
}

void SettingsMenu::draw() {
   menuTexts[BLOOM_INDEX]->textToDisplay = getStatus(bloom);
   menuTexts[MOUSE_CAPTURE_INDEX]->textToDisplay = getStatus(SDL_WM_GrabInput(SDL_GRAB_QUERY) == SDL_GRAB_ON);
   menuTexts[FULLSCREEN_INDEX]->textToDisplay = getStatus(fullscreen);
   menuTexts[MINIMAP_INDEX]->textToDisplay = getStatus(gameState->minimapOn());
   menuTexts[CAMERA_VIEW_INDEX]->textToDisplay = getViewStatus(gameState->ship->getCurrentView());
   SDL_Rect position;
   //position.y = GH/2.3;
   position.y = GH/10;
   for(int i = 0; i < menuTexts.size(); i++) {
      switch (types[i]) {
      case TITLE_TYPE:
         position.y += GH/10;
         position.x = GW/2;
         menuTexts[i]->setPosition(position);
         position.y += GH/10;
         break;
      case LEFT_TYPE:
         position.x = GW/8;
         menuTexts[i]->setPosition(position);
         break;
      case RIGHT_TYPE:
         position.x = GW * 7/8;
         menuTexts[i]->setPosition(position);
         position.y += GH/10;
         break;
      case SINGLE_SELECTABLE_TYPE:
         position.x = GW/2;
         menuTexts[i]->setPosition(position);
         position.y += GH/10;
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

   for(int i = 0; i < menuTexts.size(); i++) {
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
void SettingsMenu::keyDown(int key) {
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
      bloom = !bloom;
   } else if(menuTexts[MOUSE_CAPTURE_INDEX]->mouseSelect(x,y)) {
      toggleGrabMode();
   } else if(menuTexts[FULLSCREEN_INDEX]->mouseSelect(x,y)) {
      toggleFullScreen();
   } else if(menuTexts[MINIMAP_INDEX]->mouseSelect(x,y)) {
      gameState->toggleMinimap();
   } else if(menuTexts[CAMERA_VIEW_INDEX]->mouseSelect(x,y)) {
      gameState->ship->nextView();
   } else if(menuTexts[RETURN_INDEX]->mouseSelect(x,y)) {
      menuActive = false;
      mainMenu->menuActive = true;
      x = y = -1;
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
   menuTexts[MOUSE_CAPTURE_INDEX]->mouseHighlight(x,y);
   menuTexts[FULLSCREEN_INDEX]->mouseHighlight(x,y);
   menuTexts[MINIMAP_INDEX]->mouseHighlight(x,y);
   menuTexts[CAMERA_VIEW_INDEX]->mouseHighlight(x,y);
   menuTexts[RETURN_INDEX]->mouseHighlight(x,y);

}

void SettingsMenu::activate() {
   SDL_ShowCursor(SDL_ENABLE);
   menuActive = true;
   SoundEffect::stopAllSoundEffect();
   Music::stopMusic();
   Music::playMusic("8-bit3.ogg");
}

void SettingsMenu::deactivate() {
   SDL_ShowCursor(SDL_DISABLE);
   menuActive = false;
   Music::stopMusic();
   Music::playMusic("Asteroids2.ogg");
}


