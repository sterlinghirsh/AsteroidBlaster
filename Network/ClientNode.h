/**
 * @file
 * Header for ClientNode class
 * @author Ryuho Kudo
 */

#ifndef __CLIENTNODE_H__
#define __CLIENTNODE_H__

#include <boost/asio.hpp>
#include "Utility/Timer.h"

class ClientNode {
   //public variables------------------------------
   public:
      boost::asio::ip::udp::endpoint endpoint;
      int clientID;
      int shipID;
      Timer lastAccessed;

   //private variables------------------------------
   private:


   //public functions------------------------------
   public:
      //Constructor
      ClientNode(boost::asio::ip::udp::endpoint _endpoint, int _clientID);

   //private functions------------------------------
   private:


};

#endif
