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
#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H

#include "SDL.h"
#include <vector>

struct InputReceiver {
   virtual void keyUp(int key)=0;
   virtual void keyDown(int key)=0;
   virtual void mouseDown(int button)=0;
   virtual void mouseMove(int dx, int dy, int x, int y)=0;
   virtual void mouseUp(int button)=0;
};

class InputManager {
   std::vector<InputReceiver*> receivers;

   public:
   InputManager();
   ~InputManager();

   void update(const SDL_Event& e);
   void addReceiver(InputReceiver *kbReceiver);
   void removeReceiver(InputReceiver* inReceiver);
};

#endif
