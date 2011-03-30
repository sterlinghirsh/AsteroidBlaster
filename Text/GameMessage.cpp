/**
 * Game Message Class
 * This info gets displayed in top middle of the screen.
 * @author Sterling Hirsh <shirsh@calpoly.edu>
 * @date 3/15/11
 */

#include "Text/GameMessage.h"
#include "Utility/GlobalUtility.h"

std::list<GameMessage*> GameMessage::activeMessages;
SDL_Rect DefaultSDLRect = {0, 0};

GameMessage::GameMessage(std::string _text, double _size, double _lifetime) : Text(_text, menuFont, DefaultSDLRect) {
   lifetime = _lifetime;
   alignment = CENTERED;
   timeCreated = doubleTime();
   shouldRemove = false;
}

GameMessage::~GameMessage() {
   // Do nothing.
}

void GameMessage::update(double timeDiff) {
   shouldRemove = doubleTime() > (timeCreated + lifetime);
}

void GameMessage::drawAllMessages() {
   SDL_Rect position = {(Sint16) (GW / 2), (Sint16) (GH / 2)};
   int lineHeight = 30;
   std::list<GameMessage*>::iterator listIter = activeMessages.begin();

   while(listIter != activeMessages.end()) {
      (*listIter)->setPosition(position);
      (*listIter)->draw();
      position.y = (Sint16) (position.y + lineHeight);
      ++listIter;
   }
}

void GameMessage::updateAllMessages(double timeDiff) {
   std::list<GameMessage*>::iterator listIter = activeMessages.begin();

   while(listIter != activeMessages.end()) {
      (*listIter)->update(timeDiff);
      if ((*listIter)->shouldRemove) {
         delete *listIter;
         listIter = activeMessages.erase(listIter);

      } else {
         ++listIter;
      }
   }
}

void GameMessage::Add(std::string _text, double _size, double _lifetime) {
   activeMessages.push_back(new GameMessage(_text, _size, _lifetime));
}

void GameMessage::Clear() {
   std::list<GameMessage*>::iterator listIter = activeMessages.begin();
   while(listIter != activeMessages.end()) {
      delete *listIter;
      ++listIter;
   }
   activeMessages.erase(activeMessages.begin(), activeMessages.end());
}
