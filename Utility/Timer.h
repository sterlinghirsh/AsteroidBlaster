/**
 * @file
 * Header for Timer class
 * @author Taylor Arnicar
 */

#ifndef __TIMER_H__
#define __TIMER_H__

class GameState;

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

   //private functions------------------------------
   private:
      double getCurTime();
};

#endif
