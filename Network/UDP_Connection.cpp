/**
 * @file
 * UDP_Connection class implementation.
 * <pre>
 * This class is an aiso::io_service. It takes care of getting UDP packets and
 * sending UDP packets.
 * </pre>
 *
 * @author Ryuho Kudo
 */

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

#include "Network/UDP_Connection.h"
#include "Network/ClientCommand.h"
#include "Utility/GameState.h"
#include "Items/AsteroidShip.h"

//Constructor
UDP_Connection::UDP_Connection(boost::asio::io_service& io_service, GameState* _GameState)
: socket_(io_service, boost::asio::ip::udp::endpoint(boost::asio::ip::udp::v4(), 5000)), gameState(_GameState) {
   std::cout << "UDP_Connection running constructor..." << std::endl;
}

UDP_Connection::~UDP_Connection() {
   std::cout << "UDP_Connection destructing..." << std::endl;
}


void UDP_Connection::start_receive() {
   socket_.async_receive_from(
      boost::asio::buffer(recv_buffer_), tempEndPoint,
      boost::bind(&UDP_Connection::handle_receive, this,
      boost::asio::placeholders::error,
      boost::asio::placeholders::bytes_transferred));
   std::cout << "UDP_Connection running start_receive()..." << std::endl;
}

void UDP_Connection::handle_receive(const boost::system::error_code& error, std::size_t ) {
   std::cout << "UDP_Connection running handle_receive..." << std::endl;
   // catch error
   if (error && error != boost::asio::error::message_size) {
      std::cout << "UDP_Connection::handle_receive FAILED stopping..." << std::endl;
      return;
   }
   start_receive();
}

//The parameters are message being handled, error code, and bytes transfered
void UDP_Connection::handle_send(boost::shared_ptr<std::string>,
                  const boost::system::error_code&,
                  std::size_t){
   std::cout << "UDP_Connection running handle_send..." << std::endl;
}

void UDP_Connection::send(std::string msg, boost::asio::ip::udp::endpoint dest) {
      boost::shared_ptr<std::string> message(
          new std::string(msg));
      socket_.async_send_to(boost::asio::buffer(*message), dest,
          boost::bind(&UDP_Connection::handle_send, this, message,
            boost::asio::placeholders::error,
            boost::asio::placeholders::bytes_transferred));
}





