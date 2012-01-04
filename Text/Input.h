
#ifndef __CHAT_H__
#define __CHAT_H__

#include "Utility/InputManager.h"
#include <string>

class Text;

class Input : public InputReceiver {
   public:
      Input(GameState*& _gameState);
      virtual ~Input();

      GameState*& gameState;

      void update(double timeDiff);

      void draw();

      void keyUp(int key);
      void keyDown(int key, int unicode);
      void mouseDown(int button);
      void mouseMove(int dx, int dy, int x, int y);
      void mouseUp(int button);

      void activate(bool _ignoreNext = false);
      void deactivate();

      //the current mouse location
      double x, y;

      bool active;
      bool ignoreNext;

      Text* text; 

      std::string line;
};


#endif

