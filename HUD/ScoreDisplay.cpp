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

   int margin;
   int leftSide;
   int rightSide;
   int eyeOffset;

   if (drawStereo_enabled) {
      margin = 25;
      rightSide = gameSettings->GW / 2;
      leftSide = 0;
      eyeOffset = stereo_eye_left ? 5 : -5;
      leftSide += eyeOffset;
      rightSide += eyeOffset;
   } else {
      margin = 50; // px
      leftSide = 0;
      rightSide = gameSettings->GW;
      eyeOffset = 0;
   }


   leftSide += margin;
   rightSide -= margin;

   int top = margin;
   int bottom = gameSettings->GH - margin;

   // Draw a quad.
   glColor4f(0, 0, 0, 0.75);
   glBegin(GL_QUADS);
      glVertex2f(p2wx(leftSide), p2wy(top));
      glVertex2f(p2wx(rightSide), p2wy(top));
      glVertex2f(p2wx(rightSide), p2wy(bottom));
      glVertex2f(p2wx(leftSide), p2wy(bottom));

   glEnd();

   const int rowHeight = 30; // px
   const int topPadding = 10; // px

   int colPadding = 20; // px
   int col1Start = leftSide + colPadding + eyeOffset;

   int numColumns = 4; // Name, Score, Kills, Deaths
   int tableWidth = rightSide - leftSide - (numColumns * colPadding);

   int baseColWidth = tableWidth / 6;


   int nameWidth = 3 * baseColWidth; // px
   int scoreWidth = baseColWidth;
   int killsWidth = baseColWidth;

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
