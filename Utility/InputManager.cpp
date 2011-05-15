/**
 * @file
 * InputManager class implementation.
 * <pre>
 * A simple input manager. Any class wishing to receive input notifications
 * should inherit the InputReceiver interface and add itself to the list of
 * receivers.
 * </pre>
 *
 * @author Mike Smith
 */
#include "InputManager.h"
#include "Utility/GlobalUtility.h"
#include <stdlib.h>

using namespace std;

InputManager::InputManager() {
}

InputManager::~InputManager() {

}

void InputManager::update(const SDL_Event& e) {
   if (receivers.empty())
      return;

   vector<InputReceiver*>::iterator it;
   switch(e.type) {
   case SDL_MOUSEMOTION:
      for (it = receivers.begin(); it != receivers.end(); it++)
         (*it)->mouseMove(e.motion.xrel, e.motion.yrel, e.motion.x, e.motion.y);
      break;

   case SDL_MOUSEBUTTONDOWN:
      for (it = receivers.begin(); it != receivers.end(); it++)
         (*it)->mouseDown(e.button.button);
      break;

   case SDL_MOUSEBUTTONUP:
      for (it = receivers.begin(); it != receivers.end(); it++)
         (*it)->mouseUp(e.button.button);
      break;

   case SDL_KEYDOWN:
      for (it = receivers.begin(); it != receivers.end(); it++)
         (*it)->keyDown(e.key.keysym.sym, e.key.keysym.unicode);
      break;

   case SDL_KEYUP:
      for (it = receivers.begin(); it != receivers.end(); it++)
         (*it)->keyUp(e.key.keysym.sym);
      break;

   case SDL_VIDEORESIZE:
      gameSettings->GW = e.resize.w;
      gameSettings->GH = e.resize.h;
      if (gameSettings->GW > texSize || gameSettings->GH > texSize) {
         initFbo();
      }
      gDrawSurface = SDL_SetVideoMode(gameSettings->GW, gameSettings->GH, vidinfo->vfmt->BitsPerPixel, SDL_OPENGL);
      break;
   }

   if (e.type == SDL_KEYDOWN &&  e.key.keysym.sym == SDLK_F1) {
      toggleFullScreen();
   } else if (e.type == SDL_KEYDOWN &&  e.key.keysym.sym == SDLK_BACKQUOTE) {
      toggleGrabMode();
   } else if (e.type == SDL_KEYDOWN &&  e.key.keysym.sym == SDLK_ESCAPE) {
      running = false;
   }
}

void InputManager::addReceiver(InputReceiver *kbReceiver) {
   receivers.push_back(kbReceiver);
}

void InputManager::removeReceiver(InputReceiver *inReceiver) {
   vector<InputReceiver*>::iterator it;
   for (it = receivers.begin(); it != receivers.end(); it++) {
      if ((*it) == inReceiver) {
         receivers.erase(it);
         break;
      }
   }
}
