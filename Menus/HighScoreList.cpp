/**
 * High score list for AsteroidBlaster.
 * @author Sterling Hirsh
 * @date 1-14-2012
 */

#include "Menus/HighScoreList.h"
#include "Network/gamestate.pb.h"
#include "Utility/GlobalUtility.h"
#include "Text/Table.h"
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>

#define HIGH_SCORE_FILENAME "asteroidsHighScores.ahs"
#define MAX_HIGH_SCORES 10

using namespace std;

ast::HighScoreList highScores;

Table* highScoreTable = NULL;

string getStringFromDate(time_t date) {
   ostringstream oss;
   struct tm * now = localtime(& date);
   oss << (now->tm_year + 1900) << '/' 
      << (now->tm_mon + 1) << '/'
      <<  now->tm_mday
      << ' '
      << now->tm_hour
      << ':' << now->tm_min;
   return oss.str();
}

HighScoreList::HighScoreList() {
   load();

   if (highScoreTable == NULL) {
      vector<string> headers;
      headers.push_back("Name");
      headers.push_back("Score");
      headers.push_back("Level");
      headers.push_back("Date");

      highScoreTable = new Table(headers, "High Scores");
      highScoreTable->setColWidth(0, 3);
      highScoreTable->setColWidth(3, 2);
   }

   highlightMostRecentScore = false;
}

HighScoreList::~HighScoreList() {

}

/**
 * Return true when the new score is a new high score.
 */
bool HighScoreList::addNewScore(int score, int level) {
   int lowestScore = 0;
   bool isNewHighScore = false;

   if (highScores.entry_size() > 0) {
      lowestScore = highScores.entry(highScores.entry_size() - 1).score();
   }

   if (highScores.entry_size() < MAX_HIGH_SCORES) {
      isNewHighScore = true;
   } else if (score > lowestScore) {
      isNewHighScore = true;
   }

   // Something
   ast::HighScoreEntry* entry = highScores.add_entry();
   entry->set_score(score);
   entry->set_name(gameSettings->name);
   entry->set_date(time(0));
   entry->set_level(level);


   sort();
   save();
   return isNewHighScore;
}

void HighScoreList::load() {
   // Load.
   ifstream highScoreFile(HIGH_SCORE_FILENAME);

   if (!highScoreFile.fail()) {
      highScores.ParseFromIstream(&highScoreFile);
      highScoreFile.close();
   }
}

void HighScoreList::save() {
   ofstream highScoreFile(HIGH_SCORE_FILENAME);
   highScores.SerializeToOstream(&highScoreFile);
   highScoreFile.close();
}

void HighScoreList::sort() {
   // Inefficient sort, but it's a max of 10 elements so whatever.
   for (int i = 0; i < highScores.entry_size() - 1; ++i) {
      for (int j = i + 1; j < highScores.entry_size(); ++j) {
         if (highScores.entry(i).score() < highScores.entry(j).score()) {
            highScores.mutable_entry()->SwapElements(i, j);
         }
      }
   }

   while (highScores.entry_size() > MAX_HIGH_SCORES) {
      // Because they're sorted.
      highScores.mutable_entry()->RemoveLast();
   }
}

void HighScoreList::draw() {
   // Nothing yet, too.
   const ast::HighScoreEntry* entry;

   highScoreTable->setRows(highScores.entry_size());

   unsigned mostRecentScoreDate = 0;
   unsigned mostRecentRow = 0;

   for (int i = 0; i < highScores.entry_size(); ++i) {
      entry = &(highScores.entry(i));
      if (highlightMostRecentScore && entry->date() > mostRecentScoreDate) {
         mostRecentScoreDate = entry->date();
         mostRecentRow = i;
      }
      
      highScoreTable->setCell(i, 0, entry->name());
      highScoreTable->setCell(i, 1, entry->score());
      highScoreTable->setCell(i, 2, entry->level());
      highScoreTable->setCell(i, 3, getStringFromDate(entry->date()));
   }
   
   if (highlightMostRecentScore) {
      highScoreTable->setHighlight(mostRecentRow);
   }

   highScoreTable->draw();


}
