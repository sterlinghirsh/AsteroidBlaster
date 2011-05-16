/**
 * @file
 * UDP_Client class header.
 * <pre>
 * 
 * </pre>
 *
 * @author Ryuho Kudo
 */

#ifndef UDP_CLIENT_H
#define UDP_CLIENT_H

#include <list>
#include <map>
#include <boost/array.hpp>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/asio.hpp>

class GameState;

class UDP_Client {
   //public variables
   public:
      int clientID;
      boost::asio::ip::udp::endpoint serverEndPoint;
      boost::asio::ip::udp::endpoint clientEndPoint;
      GameState* gameState;

      boost::asio::ip::udp::socket socket_;
      boost::array<char, 1400> recv_buffer_;

   //private variables
   private:

   //public functions
   public:
      //constructor
      //UDP_Client(boost::asio::io_service& io_service, GameState* gameState, std::string _ip, unsigned _portNum);
      UDP_Client(boost::asio::io_service& io_service, GameState* _GameState, std::string _ip, std::string _portNum);
      ~UDP_Client();

      void send(std::string msg, boost::asio::ip::udp::endpoint dest);

   //private functions
   private:
      void start_receive();
      void handle_receive(const boost::system::error_code& error, std::size_t );
      void handle_send(boost::shared_ptr<std::string>,
                              const boost::system::error_code&,
                              std::size_t);
};

#endif
