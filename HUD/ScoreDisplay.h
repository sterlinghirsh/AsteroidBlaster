/**
 * ScoreDisplay.h
 * When you hit tab, this is what shows up.
 * @author Sterling Hirsh
 * @date 1/11/12
 */

#ifndef __SCOREDISPLAY_H__
#define __SCOREDISPLAY_H__

class GameState;

class ScoreDisplay {
   public:
      ScoreDisplay(GameState* _gameState);
      void update(double timeDiff);
      void draw();

   private:
      GameState* gameState;
};

#endif
