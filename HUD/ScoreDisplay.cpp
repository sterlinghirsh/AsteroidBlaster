/**
 * ScoreDisplay.h
 * When you hit tab, this is what shows up.
 * @author Sterling Hirsh
 * @date 1/11/12
 */

#include "HUD/ScoreDisplay.h"
#include "Utility/Custodian.h"
#include "Items/AsteroidShip.h"
#include "Text/Text.h"

#include <set>
#include <vector>
#include <algorithm>

using namespace std;

bool compareByScore(AsteroidShip* a, AsteroidShip* b) {
   if (a->score == b->score) {
      if (a->kills == b->kills) {
         return a->deaths < b->deaths;
      } else {
         return a->kills > b->kills;
      }
   } else {
      return a->score > b->score;
   }
}

Text* scoreDisplayText = NULL;
SDL_Rect textPosition = {0, 0};

ScoreDisplay::ScoreDisplay(GameState* _gameState) :
 gameState(_gameState) {
   if (scoreDisplayText == NULL)
      scoreDisplayText = new Text("ScoreDisplay", hudFont, textPosition);
}

void ScoreDisplay::update(double timeDiff) {
   // Nothing
}

void ScoreDisplay::draw() {
   // Copy the ships to a vector so we can sort it.
   vector<AsteroidShip*> ships(gameState->custodian.ships.begin(),
    gameState->custodian.ships.end());

   sort(ships.begin(), ships.end(), compareByScore);

   vector<AsteroidShip*>::iterator curShip = ships.begin();
   AsteroidShip* ship;
   glClear(GL_DEPTH_BUFFER_BIT);
   glDisable(GL_LIGHTING);
   glDisable(GL_CULL_FACE);

   float margin = 50; // px
   float leftSide = margin;
   float rightSide = gameSettings->GW - margin;
   float top = margin;
   float bottom = gameSettings->GH - margin;

   // Draw a quad.
   glColor4f(0, 0, 0, 0.85);
   glBegin(GL_QUADS);
      glVertex2f(p2wx(leftSide), p2wy(top));
      glVertex2f(p2wx(rightSide), p2wy(top));
      glVertex2f(p2wx(rightSide), p2wy(bottom));
      glVertex2f(p2wx(leftSide), p2wy(bottom));

   glEnd();

   float rowHeight = 30; // px
   float colPadding = 20; // px
   float topPadding = 10; // px
   float col1Start = leftSide + colPadding;

   float nameWidth = 200; // px
   float scoreWidth = 100;
   float killsWidth = 100;

   textPosition.x = col1Start;
   textPosition.y = top + topPadding + rowHeight;
   
   glColor3f(1, 1, 1);

   // Print Column Headers
   scoreDisplayText->setColor(SDL_RED);

   scoreDisplayText->updateBody("Name");
   scoreDisplayText->setPosition(textPosition);
   scoreDisplayText->draw();

   textPosition.x += nameWidth;

   scoreDisplayText->updateBody("Score");
   scoreDisplayText->setPosition(textPosition);
   scoreDisplayText->draw();

   textPosition.x += scoreWidth;

   scoreDisplayText->updateBody("Kills");
   scoreDisplayText->setPosition(textPosition);
   scoreDisplayText->draw();

   textPosition.x += killsWidth;
   
   scoreDisplayText->updateBody("Deaths");
   scoreDisplayText->setPosition(textPosition);
   scoreDisplayText->draw();

   scoreDisplayText->setColor(SDL_WHITE);


   for (; curShip != ships.end(); ++curShip) {
      ship = *curShip;
      //cout << ship->name << " score: " << ship->score << " kills: " << ship->kills << " deaths: " << ship->deaths << endl;

      textPosition.x = col1Start;
      textPosition.y += rowHeight;

      // Show Name
      scoreDisplayText->updateBody(ship->name);
      scoreDisplayText->setPosition(textPosition);
      scoreDisplayText->draw();

      // Show Score
      textPosition.x += nameWidth;
      scoreDisplayText->updateBody(ship->score);
      scoreDisplayText->setPosition(textPosition);
      scoreDisplayText->draw();
      
      // Show Kills
      textPosition.x += scoreWidth;
      scoreDisplayText->updateBody(ship->kills);
      scoreDisplayText->setPosition(textPosition);
      scoreDisplayText->draw();
      
      // Show Deaths
      textPosition.x += killsWidth;
      scoreDisplayText->updateBody(ship->deaths);
      scoreDisplayText->setPosition(textPosition);
      scoreDisplayText->draw();
   }
   
}
