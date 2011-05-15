
#include <iostream>
#include "SDL.h"
#include "Utility/GlobalUtility.h"
#include "Utility/GameState.h"
#include "Menus/HelpMenu.h"
#include "Graphics/Texture.h"
#include "Text/Text.h"
#include "Utility/Music.h"
#include "Utility/SoundEffect.h"
#include "Menus/MainMenu.h"

#define TITLE_INDEX 0
#define RETURN_INDEX (menuTexts.size() - 1)

#define TITLE_TYPE 0
#define SINGLE_SELECTABLE_TYPE 1
#define INST_TYPE 2
#define RIGHT_TYPE 3

HelpMenu::HelpMenu() {
   menuActive = false;
   x = y = -1;

   SDL_Rect position = {0,0};
   std::string fontName = DEFAULT_FONT;

   //menuTexts.push_back(new Text("Help",  fontName, position, 48));
   //types.push_back(TITLE_TYPE);
   menuTexts.push_back(new Text("Controls", menuFont, position));
   types.push_back(TITLE_TYPE);
   menuTexts.push_back(new Text("Ship Movement", menuFont, position));
   types.push_back(TITLE_TYPE);
   menuTexts.push_back(new Text("W: Accelerate forward", menuFont, position));
   types.push_back(INST_TYPE);
   menuTexts.push_back(new Text("S: Accelerate backwards", menuFont, position));
   types.push_back(INST_TYPE);
   menuTexts.push_back(new Text("A: Yaw left", menuFont, position));
   types.push_back(INST_TYPE);
   menuTexts.push_back(new Text("D: Yaw right", menuFont, position));
   types.push_back(INST_TYPE);
   menuTexts.push_back(new Text("B: Brake", menuFont, position));
   types.push_back(INST_TYPE);
   menuTexts.push_back(new Text("Mouse left/right: Roll left/right", menuFont, position));
   types.push_back(INST_TYPE);
   menuTexts.push_back(new Text("Mouse up/down: Pitch up/down", menuFont, position));
   types.push_back(INST_TYPE);
   menuTexts.push_back(new Text("Left mouse click: Lock ship's roll axis",  menuFont, position));
   types.push_back(INST_TYPE);
   menuTexts.push_back(new Text("Weapons", menuFont, position));
   types.push_back(TITLE_TYPE);
   menuTexts.push_back(new Text("Z or mouse wheel up: Previous weapon", menuFont, position));
   types.push_back(INST_TYPE);
   menuTexts.push_back(new Text("C or mouse wheel down: Next weapon", menuFont, position));
   types.push_back(INST_TYPE);
   menuTexts.push_back(new Text("Left mouse click: Fire weapon",  menuFont, position));
   types.push_back(INST_TYPE);
   menuTexts.push_back(new Text("Miscellaneous",  menuFont, position));
   types.push_back(TITLE_TYPE);
   menuTexts.push_back(new Text("1: Decrease minimap size", menuFont, position));
   types.push_back(INST_TYPE);
   menuTexts.push_back(new Text("2: Increase minimap size", menuFont, position));
   types.push_back(INST_TYPE);
   menuTexts.push_back(new Text("3: Decrease minimap zoom", menuFont, position));
   types.push_back(INST_TYPE);
   menuTexts.push_back(new Text("4: Increase minimap zoom", menuFont, position));
   types.push_back(INST_TYPE);
   menuTexts.push_back(new Text("Return", menuFont, position));
   types.push_back(SINGLE_SELECTABLE_TYPE);

   for (unsigned i = 0; i < menuTexts.size(); i++) {
      switch (types[i]) {
      case TITLE_TYPE:
         menuTexts[i]->alignment = CENTERED;
         break;
      case INST_TYPE:
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

HelpMenu::~HelpMenu() {
   for(unsigned i = 0; i < menuTexts.size(); i++) {
      delete menuTexts[i];
   }
}

std::string HelpMenu::getStatus(bool status) {
   if (status) {
      return "Enabled";
   }
   return "Disabled";
}

void HelpMenu::draw() {
   SDL_Rect position;
   position.y = (Sint16) (gameSettings->GH/10);
   for(unsigned i = 0; i < menuTexts.size(); i++) {
      switch (types[i]) {
      case TITLE_TYPE:
         position.x = (Sint16) (gameSettings->GW/2);
         position.y = (Sint16) (position.y + (gameSettings->GH/40));
         menuTexts[i]->setPosition(position);
         //if (i > 1 && types[i - 1] != TITLE_TYPE) {
         if (i > 0) {
            position.y = (Sint16) (position.y + (gameSettings->GH/20));
         }
         break;
      case INST_TYPE:
         position.x = (Sint16) (gameSettings->GW/3);
         menuTexts[i]->setPosition(position);
         position.y = (Sint16) (position.y + (gameSettings->GH/30));
         break;
      case SINGLE_SELECTABLE_TYPE:
         position.x = (Sint16) (gameSettings->GW/2);
         position.y = (Sint16) (position.y + (gameSettings->GH/30));
         menuTexts[i]->setPosition(position);
         position.y = (Sint16) (position.y + (gameSettings->GH/30));
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
void HelpMenu::keyDown(int key, int unicode) {
   return;
}

/**
 * Handles the player letting go of a key
 */
void HelpMenu::keyUp(int key) {
   if (!menuActive) { return; }

}

/**
 * Handles the player clicking the mouse
 */
void HelpMenu::mouseDown(int button) {
   if (!menuActive) { return; }

   if(menuTexts[RETURN_INDEX]->mouseSelect(x,y)) {
      menuActive = false;
      mainMenu->menuActive = true;
      x = y = -1;
   }
}

/**
 * Handles the player letting go of a mouse click
 */
void HelpMenu::mouseUp(int button) {
   if (!menuActive) { return; }
}

void HelpMenu::mouseMove(int dx, int dy, int _x, int _y) {
   //std::cout << "mouseMove=(" << _x << "," << _y << ")" << std::endl;
   if (!menuActive) { return; }
   x = _x;
   y = _y;
   //decide the color for each menu text
   for (unsigned i = 0; i < menuTexts.size(); i++) {
      if (types[i] != TITLE_TYPE) {
         menuTexts[i]->mouseHighlight(x,y);
      }
   }

}

void HelpMenu::activate() {
   SDL_ShowCursor(SDL_ENABLE);
   menuActive = true;
   SoundEffect::stopAllSoundEffect();
   Music::playMusic("8-bit3.ogg");
}

void HelpMenu::deactivate() {
   SDL_ShowCursor(SDL_DISABLE);
   menuActive = false;
   Music::playMusic("Asteroids2.ogg");
}
