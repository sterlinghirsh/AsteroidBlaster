/**
 * @file
 * Header for Timer class
 * @author Taylor Arnicar
 */

#ifndef __TIMER_H__
#define __TIMER_H__

class GameState;

namespace ast {
   class Timer;
}

class Timer {
   //public variables------------------------------
   public:
      double timeStarted;
      double countDownTime;
      double timePaused;
      bool isPaused;
      bool isRunning;

   //private variables------------------------------
   private:
      GameState* gameState;


   //public functions------------------------------
   public:
      //Constructor
      Timer(GameState* _gameState = 0); // Null default.

      void save(ast::Timer* t);
      void load(const ast::Timer& t);

      void setGameState(GameState* _gameState);
      void countUp();
      void reset();
      void setCountDown(double _countDownTime);
      void restartCountDown();
      double getTimeLeft();
      double getTimeRunning();
      double getAmountComplete();
      void pause();
      void resume();
      bool getIsPaused();
      void debug();

   //private functions------------------------------
   private:
      double getCurTime();
};

#endif
