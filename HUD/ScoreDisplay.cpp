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
#include "Text/Table.h"

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
Table* scoreDisplayTable = NULL;
static SDL_Rect textPosition = {0, 0};

ScoreDisplay::ScoreDisplay(GameState* _gameState) :
 gameState(_gameState) {
   if (scoreDisplayText == NULL)
      scoreDisplayText = new Text("ScoreDisplay", hudFont, textPosition);


   if (scoreDisplayTable == NULL) {
      vector<string> headers;
      headers.push_back("Name");
      headers.push_back("Score");
      headers.push_back("Kills");
      headers.push_back("Deaths");

      scoreDisplayTable = new Table(headers, "Scores");
      scoreDisplayTable->setColWidth(0, 3);
   }

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

   scoreDisplayTable->setRows(ships.size());
   unsigned curRow = 0;
   for (; curShip != ships.end(); ++curShip) {
      ship = *curShip;
      if (ship == gameState->ship) {
         scoreDisplayTable->setHighlight(curRow);
      }
      scoreDisplayTable->setCell(curRow, 0, ship->name);
      scoreDisplayTable->setCell(curRow, 1, ship->score);
      scoreDisplayTable->setCell(curRow, 2, ship->kills);
      scoreDisplayTable->setCell(curRow, 3, ship->deaths);
      ++curRow;
   }

   scoreDisplayTable->draw();
}
