#include "Network/NetTimer.h"

BOOST_CLASS_EXPORT_IMPLEMENT(NetTimer);


NetTimer::NetTimer() : timeStarted(0), 
                        countDownTime(0), 
                        timePaused(0), 
                        isPaused(true), 
                        isRunning(false) {

}

Timer NetTimer::toObject(NetTimer item) {
   Timer answer;

   answer.timeStarted = timeStarted;
   answer.countDownTime = countDownTime;
   answer.timePaused = timePaused;
   answer.isPaused = isPaused;
   answer.isRunning = isRunning;

   return answer;
}

void NetTimer::fromObject(Timer in) {
   timeStarted = in.timeStarted;
   countDownTime = in.countDownTime;
   timePaused = in.timePaused;
   isPaused = in.isPaused;
   isRunning = in.isRunning;
}


