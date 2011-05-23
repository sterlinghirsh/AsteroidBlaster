/**
 * @file
 * Header for Timer class
 * @author Taylor Arnicar
 */

#ifndef __TIMER_H__
#define __TIMER_H__


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


   //public functions------------------------------
   public:
      //Constructor
      Timer();


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


};

#endif
