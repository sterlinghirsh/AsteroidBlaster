/**
 * @file
 * UDP_Client class implementation.
 * <pre>
 * Inherits from UDP_Connection class. Handles Client side connection stuff.
 * </pre>
 *
 * @author Ryuho Kudo
 */

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

#include "Network/UDP_Client.h"
#include "Network/ClientCommand.h"
#include "Utility/GameState.h"
#include "Items/AsteroidShip.h"

//Constructor
UDP_Client::UDP_Client(boost::asio::io_service& io_service, GameState* _GameState)
: UDP_Connection(io_service, _GameState) {
   //std::cout << "UDP_Client running constructor..." << std::endl;
   start_receive();
}

UDP_Client::~UDP_Client() {
   //std::cout << "UDP_Client destructing..." << std::endl;
}


void UDP_Client::start_receive() {
   socket_.async_receive_from(
      boost::asio::buffer(recv_buffer_), tempEndPoint,
      boost::bind(&UDP_Client::handle_receive, this,
      boost::asio::placeholders::error,
      boost::asio::placeholders::bytes_transferred));
   //std::cout << "UDP_Client running start_receive()..." << std::endl;
}

void UDP_Client::handle_receive(const boost::system::error_code& error, std::size_t ) {
   //std::cout << "UDP_Client running handle_receive..." << std::endl;
   // catch error
   if (error && error != boost::asio::error::message_size) {
      std::cout << "UDP_Client::handle_receive FAILED stopping..." << std::endl;
      return;
   }


   start_receive();

}

//The parameters are message being handled, error code, and bytes transfered
void UDP_Client::handle_send(boost::shared_ptr<std::string>,
                  const boost::system::error_code&,
                  std::size_t){
   //std::cout << "UDP_Client running handle_send..." << std::endl;
}


