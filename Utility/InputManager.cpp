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
         (*it)->keyDown(e.key.keysym.sym);
      break;

   case SDL_KEYUP:
      for (it = receivers.begin(); it != receivers.end(); it++)
         (*it)->keyUp(e.key.keysym.sym);
      break;
   }
   /*
   if (e.type == SDL_KEYDOWN &&  e.key.keysym.sym == SDLK_F1) {
      if(!fullScreen) {
         oldGW = GW;
         oldGH = GH;
         GW = 1280;
         GH = 1024;
         gDrawSurface = SDL_SetVideoMode(GW, GH, vidinfo->vfmt->BitsPerPixel, SDL_OPENGL);
         SDL_WM_ToggleFullScreen( gDrawSurface );
         fullScreen = !fullScreen;
      }
      else {
         GW = oldGW;
         GH = oldGH;
         SDL_WM_ToggleFullScreen( gDrawSurface );
         gDrawSurface = SDL_SetVideoMode(GW, GH, vidinfo->vfmt->BitsPerPixel, SDL_OPENGL);
         fullScreen = !fullScreen;
      }
   } else if (e.type == SDL_KEYDOWN &&  e.key.keysym.sym == SDLK_BACKQUOTE) {
   */
   if (e.type == SDL_KEYDOWN &&  e.key.keysym.sym == SDLK_BACKQUOTE) {
      toggleGrabMode();
   } else if (e.type == SDL_KEYDOWN &&  e.key.keysym.sym == SDLK_ESCAPE) {
      exit(0);
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
