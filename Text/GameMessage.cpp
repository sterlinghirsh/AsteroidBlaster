/**
 * Game Message Class
 * This info gets displayed in top middle of the screen.
 * @author Sterling Hirsh <shirsh@calpoly.edu>
 * @date 3/15/11
 */

#include "Text/GameMessage.h"
#include "Utility/GlobalUtility.h"
#include "Utility/GameState.h"

std::list<GameMessage*> GameMessage::activeMessages;
SDL_Rect DefaultSDLRect = {0, 0};

GameMessage::GameMessage(std::string _text, double _size, double _lifetime, 
 GameState* _gameState, bool _inSidebar) : 
 Text(_text, (_inSidebar ? hudFont : menuFont), DefaultSDLRect),
 inSidebar(_inSidebar),
 gameState(_gameState) {
   lifetime = _lifetime;
   alignment = inSidebar ? LEFT_ALIGN : CENTERED;
   // This is a good place to use doubletime.
   timeCreated = gameState->getGameTime();
   shouldRemove = false;
}

GameMessage::~GameMessage() {
   // Do nothing.
}

void GameMessage::update(double timeDiff) {
   // This is a good place to use doubletime.
   shouldRemove = gameState->getGameTime() > (timeCreated + lifetime);
}

void GameMessage::drawAllMessages() {
   int xPos;
   if (drawStereo_enabled) {
      xPos = gameSettings->GW / 4;
      if (stereo_eye_left) {
         xPos += 5;
      } else {
         xPos -= 5;
      }
   } else {
      xPos = gameSettings->GW / 2;
   }
   SDL_Rect position = {(Sint16) xPos, (Sint16) (gameSettings->GH / 4)};
   int lineHeight = 30;
   std::list<GameMessage*>::iterator listIter = activeMessages.begin();

   int messageCount = 0;
   const int maxMessages = 5;

   while(listIter != activeMessages.end()) {
      if (!(*listIter)->inSidebar) {
         if (++messageCount <= maxMessages) {
            (*listIter)->setPosition(position);
            (*listIter)->draw();
            position.y = (Sint16) (position.y + lineHeight);
         }
      }
      ++listIter;
   }

   position.x = 30; // px from left
   position.y = (gameSettings->GH * 3) / 4;
   listIter = activeMessages.begin();
   messageCount = 0;

   while(listIter != activeMessages.end()) {
      if ((*listIter)->inSidebar) {
         if (++messageCount <= maxMessages) {
            (*listIter)->setPosition(position);
            (*listIter)->draw();
            position.y = (Sint16) (position.y + lineHeight);
         }
      }
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

void GameMessage::Add(std::string _text, double _size, double _lifetime, GameState* _gameState, bool _inSidebar) {
   activeMessages.push_back(new GameMessage(_text, _size, _lifetime, _gameState, _inSidebar));
}

void GameMessage::Clear() {
   std::list<GameMessage*>::iterator listIter = activeMessages.begin();
   while(listIter != activeMessages.end()) {
      delete *listIter;
      ++listIter;
   }
   activeMessages.erase(activeMessages.begin(), activeMessages.end());
}
