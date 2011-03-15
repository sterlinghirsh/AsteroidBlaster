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

GameMessage::GameMessage(std::string _text, double _size, double _lifetime) : Text(_text, DEFAULT_FONT, DefaultSDLRect, _size) {
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
   std::list<GameMessage*>::iterator listIter;

   SDL_Rect position = {GW / 2, 160};
   int lineHeight = 30;

   for (listIter = activeMessages.begin(); listIter != activeMessages.end(); ++listIter) {
      (*listIter)->setPosition(position);
      (*listIter)->draw();
      position.y += lineHeight;
   }
}

void GameMessage::updateAllMessages(double timeDiff) {
   std::list<GameMessage*>::iterator listIter;
   for (listIter = activeMessages.begin(); listIter != activeMessages.end(); ++listIter) {
      (*listIter)->update(timeDiff);
      if ((*listIter)->shouldRemove) {
         delete *listIter;
         listIter = activeMessages.erase(listIter);
      }
   }
}

void GameMessage::Add(std::string _text, double _size, double _lifetime) {
   activeMessages.push_back(new GameMessage(_text, _size, _lifetime));
}

void GameMessage::Clear() {
   std::list<GameMessage*>::iterator listIter;
   for (listIter = activeMessages.begin(); listIter != activeMessages.end(); ++listIter) {
      delete *listIter;
      listIter = activeMessages.erase(listIter);
   }

}
