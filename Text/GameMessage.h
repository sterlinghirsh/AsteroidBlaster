/**
 * Game Message Class
 * This info gets displayed in top middle of the screen.
 * @author Sterling Hirsh <shirsh@calpoly.edu>
 * @date 3/15/11
 */

#ifndef __GAMEMESSAGE_H__
#define __GAMEMESSAGE_H__

#include "Text/Text.h"
#include <list>

class GameState;

class GameMessage : public Text {
   public:
      GameMessage(std::string _text, double _size, double _lifetime, 
       GameState* _gameState, bool _inSidebar = false);
      virtual ~GameMessage();
      virtual void update(double timeDiff);

      bool inSidebar;
      bool shouldRemove;
      double lifetime; // Secs
      double timeCreated;

      GameState* gameState;

      static std::list<GameMessage*> activeMessages;
      static void drawAllMessages();
      static void updateAllMessages(double timeDiff);
      static void Add(std::string _text, double _size, double _lifetime, GameState* _gameState, bool _inSidebar = false);
      static void Clear();
};

#endif
