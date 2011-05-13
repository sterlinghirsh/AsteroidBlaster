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

      boost::asio::io_service io_service;

      udp::resolver resolver(io_service);
      udp::resolver::query query(udp::v4(), argv[1], "5000");
      udp::endpoint receiver_endpoint = *resolver.resolve(query);

      udp::socket socket(io_service);
      socket.open(udp::v4());

      int i = 1000;
      ClientCommand test;


      while (1) {
         std::ostringstream oss;
         
         {
            boost::archive::text_oarchive oa(oss);
            oa << i << (const ClientCommand)test;
         }

         
         std::cout << "oss.str()=" << oss.str() << std::endl;

         socket.send_to(boost::asio::buffer(oss.str()), receiver_endpoint);

         boost::array<char, 80> recv_buf;
         udp::endpoint sender_endpoint;
         size_t len = socket.receive_from(boost::asio::buffer(recv_buf), sender_endpoint);

         std::cout.write(recv_buf.data(), len);
         std::cout << "recv_buf.data()=|" << recv_buf.data() << "| with length of " << len << std::endl;
         //std::cout << recv_buf[0] << "| with length of " << len << std::endl;
         i++;
         mySleep(100000);
      }
   } catch (std::exception& e) {
      std::cerr << e.what() << std::endl;
   }

   return 0;
}

