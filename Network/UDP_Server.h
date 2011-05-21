/**
 * @file
 * UDP_Server class header.
 * <pre>
 * Has client counter and means of communicating to clients
 * </pre>
 *
 * @author Ryuho Kudo
 */

#ifndef UDP_SERVER_H
#define UDP_SERVER_H

#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0501
#endif 

#include <list>
#include <map>
#include <boost/array.hpp>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/asio.hpp>

#include "Utility/Constants.h"

class GameState;

class UDP_Server {
   //public variables
   public:
      std::map<boost::asio::ip::udp::endpoint, unsigned> tempEndpointToClientID;
      std::map<boost::asio::ip::udp::endpoint, unsigned> endpointToClientID;
      boost::asio::ip::udp::endpoint tempEndPoint;
      unsigned clientIDCounter;

      boost::asio::ip::udp::socket socket_;
      boost::array<char, PACK_SIZE> recv_buffer_;
      
      GameState* gameState;

   //private variables
   private:


   //public functions
   public:
      //constructor
      UDP_Server(boost::asio::io_service& io_service, GameState* gameState, unsigned _portNumber);
      ~UDP_Server();

      void send(std::string msg, boost::asio::ip::udp::endpoint dest);
      void sendAll(std::string msg);

   //private functions
   private:
      void start_receive();
      void handle_receive(const boost::system::error_code& error, std::size_t );
      void handle_send(boost::shared_ptr<std::string>,
                              const boost::system::error_code&,
                              std::size_t);
};

#endif
