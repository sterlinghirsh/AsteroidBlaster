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


KeyboardManager::KeyboardManager(KeyboardReceiver* kbReceiver) {
  Uint8* tempKeys;
  tempKeys = SDL_GetKeyState(&numKeys);
  prevKeys = new Uint8[numKeys];
  memcpy(prevKeys, tempKeys, sizeof(Uint8) * numKeys);
  receiver = kbReceiver;
}

KeyboardManager::~KeyboardManager() {
   delete prevKeys;
}

void KeyboardManager::update() {
   Uint8* newKeys = SDL_GetKeyState(&numKeys);

   if (receiver) {
      for (int i = 0; i < numKeys; i++) {
         if (prevKeys[i] != newKeys[i]) {
            if (!newKeys[i])
               receiver->keyUp(i);
            }

         prevKeys[i] = newKeys[i];
         if (prevKeys[i])
            receiver->keyDown(i);
      }
   }
   else {
      memcpy(prevKeys, newKeys, numKeys * sizeof(Uint8));
   }
}

int KeyboardManager::poll(int key) {
   return prevKeys[key];
}

void KeyboardManager::setReceiver(KeyboardReceiver *kbReceiver) {
   receiver = kbReceiver;
}
