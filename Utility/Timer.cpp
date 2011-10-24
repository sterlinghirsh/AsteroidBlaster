
#include "Utility/Timer.h"
#include "Utility/GlobalUtility.h"
#include "Utility/GameState.h"
#include "Network/gamestate.pb.h"


Timer::Timer(GameState* _gameState) : timeStarted(0), 
                  countDownTime(0), 
                  timePaused(0), 
                  isPaused(false), 
                  isRunning(false),
                  gameState(_gameState) {

}

void Timer::setGameState(GameState* _gameState) {
   gameState = _gameState;
}

void Timer::countUp() {
   isRunning = true;
   timeStarted = getCurTime();
   isPaused = false;
   countDownTime = 0;
   timePaused = 0;
}

void Timer::reset() {
   timeStarted = getCurTime();
   countDownTime = 0;
   timePaused = 0;
   isPaused = false;
   isRunning = false;
}

void Timer::setCountDown(double _countDownTime) {
   timeStarted = getCurTime();
   timePaused = 0;
   isPaused = false;
   countDownTime = _countDownTime;
   isRunning = true;
}

/**
 * Set the count down to its last value.
 */
void Timer::restartCountDown() {
   timeStarted = getCurTime();
   isRunning = true;
}

double Timer::getTimeLeft() {
   return countDownTime - getTimeRunning();
}

double Timer::getTimeRunning() {
   return getCurTime() - timeStarted;
}

/**
 * Return a number representing how long the timer has
 * been counting in units of countDownTime.
 * After just starting, this will return 0.
 * After countDownTime seconds, this will return 1.
 * After 2 * countDownTime seconds, this will return 2.
 */
double Timer::getAmountComplete() {
   if (countDownTime == 0)
      return 0;

   return 1 - (getTimeLeft() / countDownTime);
}

/**
 * The Timer will remain at its current timeleft until it is resumed again.
 * Pausing a timer when it is already paused will do nothing.
 */
void Timer::pause() {
   // As long as the game's not already paused, pause it.
   if (!getIsPaused()) {
      isPaused = true;
      isRunning = false;
      timePaused = getCurTime();
   }
}

/**
 * Resuming a timer will only work if it is already paused.
 */
void Timer::resume() {
   // As long as the game is already paused, resume it.
   if (getIsPaused()) {
      isPaused = false;
      isRunning = true;
      timeStarted += getCurTime() - timePaused;
   }
}

bool Timer::getIsPaused() {
   return isPaused;
}

double Timer::getCurTime() {
   if (gameState == NULL) {
      return doubleTime();
   } else {
      return gameState->getGameTime();
   }
}

void Timer::load(const ast::Timer& t) {
   if (t.has_timestarted()) {
      timeStarted = t.timestarted();
   }

   if (t.has_countdowntime()) {
      countDownTime = t.countdowntime();
   }

   if (t.has_timepaused()) {
      timePaused = t.timepaused();
   }

   if (t.has_ispaused()) {
      isPaused = t.ispaused();
   }

   if (t.has_isrunning()) {
      isRunning = t.isrunning();
   }
}

bool Timer::saveDiff(const ast::Timer& old, ast::Timer* t) {
   bool somethingChanged = false;
   if (old.timestarted() != timeStarted) {
      somethingChanged = true;
      t->set_timestarted(timeStarted);
   }

   if (old.countdowntime() != countDownTime) {
      somethingChanged = true;
      t->set_countdowntime(countDownTime);
   }

   if (old.timepaused() != timePaused) {
      somethingChanged = true;
      t->set_timepaused(timePaused);
   }

   if (old.ispaused() != isPaused) {
      somethingChanged = true;
      t->set_ispaused(isPaused);
   }

   if (old.isrunning() != isRunning) {
      somethingChanged = true;
      t->set_isrunning(isRunning);
   }

   return somethingChanged;
}

void Timer::save(ast::Timer* t) {
   t->set_timestarted(timeStarted);
   t->set_countdowntime(countDownTime);
   t->set_timepaused(timePaused);
   t->set_ispaused(isPaused);
   t->set_isrunning(isRunning);
}

void Timer::debug() {
   std::cout << "Timer " << timeStarted << " " << countDownTime << " "
      << timePaused << " " << isPaused << " " << isRunning << std::endl;
}
