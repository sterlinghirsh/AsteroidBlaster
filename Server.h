/**
 * Server
 * 
 * @author Ryuho Kudo
 * @date 5-10-11
 */

#ifndef __SERVER_H__
#define __SERVER_H__

#include <iostream>
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <boost/bind.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

#include "Network/UDP_Connection.h"

class Server {
   //public variables
   public:

   //private variables
   private:
      boost::asio::strand mainStrand;
      boost::asio::deadline_timer gameThread;
      boost::asio::deadline_timer networkThread;
      int mainCount;
      int count1;
      int count2;

   //public functions
   public:
      //constructor
      Server(boost::asio::io_service& io);
      //destructor
      ~Server();

   //private functions
   private:
      void gameFunction();
      void networkFunction();

};

#endif
