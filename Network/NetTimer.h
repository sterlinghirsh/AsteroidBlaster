
#ifndef __NETTIMER_H__
#define __NETTIMER_H__

#include "Utility/Timer.h"
#include "boost/serialization/base_object.hpp"
#include "boost/serialization/extended_type_info.hpp"
#include "boost/serialization/export.hpp"

struct NetTimer {
   double timeStarted;
   double countDownTime;
   double timePaused;
   bool isPaused;
   bool isRunning;

   NetTimer();

   Timer toObject(NetTimer item);
   void fromObject(Timer in);

   template <class Archive>
    void serialize(Archive & ar, const unsigned int version) {
      ar & timeStarted;
      ar & countDownTime;
      ar & timePaused;
      ar & isPaused;
      ar & isRunning;
   }

};

BOOST_CLASS_EXPORT_KEY(NetTimer);

#endif
