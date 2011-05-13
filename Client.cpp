//
// client.cpp
// ~~~~~~~~~~
//
// Copyright (c) 2003-2008 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <iostream>
#include <fstream>
#include <boost/array.hpp>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

#include "Network/ClientCommand.h"

using boost::asio::ip::udp;

int clientID;
ClientCommand localClientCommand;
udp::endpoint sender_endpoint;

void mySleep(unsigned int mseconds) {
    clock_t goal = mseconds + clock();
    while (goal > clock());
}

int main(int argc, char* argv[]) {
   try {
      if (argc != 2) {
         std::cerr << "Usage: client <host>" << std::endl;
         return 1;
      }
      localClientCommand.forwardAcceleration = 1;


      boost::asio::io_service io_service;

      udp::resolver resolver(io_service);
      udp::resolver::query query(udp::v4(), argv[1], "5000");
      udp::endpoint receiver_endpoint = *resolver.resolve(query);

      udp::socket socket(io_service);
      socket.open(udp::v4());

      int i = 0;

      // do the hand Shake!
      {
         // First construct an init packet with packet id of 0 then send it
         std::ostringstream oss;
         boost::archive::text_oarchive oa(oss);
         oa << i;
         socket.send_to(boost::asio::buffer(oss.str()), receiver_endpoint);
      }

      {
         // then wait for the answer back...
         boost::array<char, 80> recv_buf;
         size_t len = socket.receive_from(boost::asio::buffer(recv_buf), sender_endpoint);

         // decode the answer, and grab the assigned clientID
         std::istringstream iss(recv_buf.data());
         boost::archive::text_iarchive ia(iss);
         ia >> clientID;
         std::cout << "my clientID is " << clientID << std::endl;
      }

      {
         // then do handshake 2
         std::ostringstream oss;
         i = 1;
         boost::archive::text_oarchive oa(oss);
         oa << i << clientID;
         socket.send_to(boost::asio::buffer(oss.str()), receiver_endpoint);
      }

      {
         // then wait for the answer back for handshake3...
         boost::array<char, 80> recv_buf;
         size_t len = socket.receive_from(boost::asio::buffer(recv_buf), sender_endpoint);
         // decode the answer, and make sure that the packet id is 2
         std::istringstream iss(recv_buf.data());
         boost::archive::text_iarchive ia(iss);
         int temp;
         ia >> temp;
         if (temp == 2) {
            std::cout << "Handshake Complete! I am now a client with ID of " << clientID << std::endl;
         }
      }

      


      while (1) {
         std::ostringstream oss;
         
         {
            boost::archive::text_oarchive oa(oss);
            i = 3;
            oa << i << (const ClientCommand)localClientCommand;
         }

         std::cout << "sending packet: " << oss.str() << std::endl;

         socket.send_to(boost::asio::buffer(oss.str()), receiver_endpoint);

         sleep(1);
      }
   } catch (std::exception& e) {
      std::cerr << e.what() << std::endl;
   }

   return 0;
}

