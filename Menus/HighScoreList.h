/**
 * High score list for AsteroidBlaster.
 * @author Sterling Hirsh
 * @date 1-14-2012
 */

#ifndef __HIGHSCORELIST_H__
#define __HIGHSCORELIST_H__

class HighScoreList {
   public:
      HighScoreList();
      ~HighScoreList();
      bool addNewScore(int score, int level);
      void load();
      void save();
      void draw();
      void sort();
      bool highlightMostRecentScore;
};

#endif
