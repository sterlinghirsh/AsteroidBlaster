/**
 * @file
 * A simple keyboard manager.
 *
 * <pre>
 * This class is a simple keyboard manager. It supplies a few functions
 * for querying the state of the keyboard, and an update function to be
 * called once each frame.
 *
 * </pre>
 *
 * @author Mike Smith
 */
#ifndef KEYBOARDMANAGER_H
#define KEYBOARDMANAGER_H

#include <SDL.h>

class KeyboardManager {
   private:
      Uint8 *curKeys, *prevKeys;
      int numKeys;

   public:
      KeyboardManager();
      ~KeyboardManager();

      void update();

      int keyUp(int key);
      int keyDown(int key);
      
      int isKeyUp(int key);
      int isKeyDown(int key);
};

#endif
