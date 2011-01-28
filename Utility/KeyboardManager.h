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

struct KeyboardReceiver {
   virtual void keyUp(int key)=0;
   virtual void keyDown(int key)=0;
};

class KeyboardManager {
      Uint8 *prevKeys;
      int numKeys;
      KeyboardReceiver* receiver;

   public:
      KeyboardManager(KeyboardReceiver *kbReceiever);
      ~KeyboardManager();

      void update();
      int poll(int key);
      void setReceiver(KeyboardReceiver *kbReceiver);
};

#endif
