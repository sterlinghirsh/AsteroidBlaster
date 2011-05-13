/**
 * @file
 * UDP_Connection class header.
 * <pre>
 * This class is an aiso::io_service. It takes care of getting UDP packets and
 * sending UDP packets.
 * </pre>
 *
 * @author Ryuho Kudo
 */

#ifndef UDP_CONNECTION_H
#define UDP_CONNECTION_H

#include <list>
#include <map>
#include <boost/array.hpp>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/asio.hpp>

class UDP_Connection {
   //public variables
   public:
      unsigned currClientID;
      std::map<boost::asio::ip::udp::endpoint, unsigned> tempRemoteClients;
      std::map<boost::asio::ip::udp::endpoint, unsigned> remoteClients;
      boost::asio::ip::udp::endpoint tempEndPoint;
      

   //private variables
   private:
      boost::asio::ip::udp::socket socket_;
      
      boost::array<char, 1400> recv_buffer_;

   //public functions
   public:
      //constructor
      UDP_Connection(boost::asio::io_service& io_service);
      ~UDP_Connection();

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
