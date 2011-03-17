
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
#define BLOOM_INDEX 1
#define MOUSE_CAPTURE_INDEX 2
#define RETURN_INDEX 3

SettingsMenu::SettingsMenu() {
   menuActive = false;
   x = y = -1;

   SDL_Rect position = {0,0};
   std::string fontName = DEFAULT_FONT;

   menuTexts.push_back(new Text("Settings",  fontName, position, 48));
   menuTexts.push_back(new Text("Bloom: " + getStatus(bloom),  fontName, position, 24));
   menuTexts.push_back(new Text("Mouse Capture: " + getStatus(SDL_WM_GrabInput(SDL_GRAB_QUERY) == SDL_GRAB_ON),  fontName, position, 24));
   menuTexts.push_back(new Text("Return", fontName, position, 24));


   menuTexts[BLOOM_INDEX]->selectable = true;
   menuTexts[MOUSE_CAPTURE_INDEX]->selectable = true;
   menuTexts[RETURN_INDEX]->selectable = true;


   for (int i = 0; i < menuTexts.size(); i++) {
      //menuTexts[i]->centered = true;
      menuTexts[i]->alignment = CENTERED;
   }

}


SettingsMenu::~SettingsMenu() {
   for(int i = 0; i < menuTexts.size(); i++) {
      delete menuTexts[i];
   }
}

std::string SettingsMenu::getStatus(bool status) {
   if (status) {
      return "on";
   }
   return "off";
}

void SettingsMenu::draw() {
   menuTexts[BLOOM_INDEX]->textToDisplay = "Bloom: " + getStatus(bloom);
   menuTexts[MOUSE_CAPTURE_INDEX]->textToDisplay = "Mouse Capture: " + getStatus(SDL_WM_GrabInput(SDL_GRAB_QUERY) == SDL_GRAB_ON);
   SDL_Rect position;
   position.x = GW/2;
   position.y = GH/2.3;
   for(int i = 0; i < menuTexts.size(); i++) {
      menuTexts[i]->setPosition(position);
      position.y += GH/10;
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
      //mouseCapture = !mouseCapture;
      toggleGrabMode();
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


