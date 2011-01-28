/**
 * @file
 * KeyboardManager class implementation.
 * <pre>
 * A simple keyboard manager.
 * </pre>
 *
 * @author Mike Smith
 */
#include "KeyboardManager.h"
#include <stdlib.h>


KeyboardManager::KeyboardManager() {
  curKeys = SDL_GetKeyState(&numKeys);
  prevKeys = new Uint8[numKeys];
  memcpy(prevKeys, curKeys, sizeof(Uint8) * numKeys);
}

KeyboardManager::~KeyboardManager() {
   delete prevKeys;
}

void KeyboardManager::update() {
   memcpy(prevKeys, curKeys, sizeof(Uint8) * numKeys);

   curKeys = SDL_GetKeyState(&numKeys);
}

int KeyboardManager::keyUp(int key) {
   return !curKeys[key] && prevKeys[key];
}

int KeyboardManager::keyDown(int key) {
   return curKeys[key] && !prevKeys[key];
}

int KeyboardManager::isKeyDown(int key) {
   return curKeys[key];
}

int KeyboardManager::isKeyUp(int key) {
   return !curKeys[key];
}
