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


#include "Utility/GameState.h"
#include "Items/AsteroidShip.h"

#include "Network/NetShard.h"

//Constructor
UDP_Client::UDP_Client(boost::asio::io_service& io_service, GameState* _GameState, std::string _ip, std::string _portNum)
: socket_(io_service, boost::asio::ip::udp::endpoint(boost::asio::ip::udp::v4(), 0)), gameState(_GameState) {

   gameState = _GameState;
   //std::cout << "UDP_Client running constructor..." << std::endl;
   std::cout << "UDP_Client being constructed which will try to connect IP:" 
             << _ip << " and port #" << _portNum << std::endl;
   clientID = 0;

   boost::asio::ip::udp::resolver resolver(io_service);
   boost::asio::ip::udp::resolver::query query(boost::asio::ip::udp::v4(), _ip, _portNum.c_str());
   serverEndPoint = *resolver.resolve(query);
   
   // Start handshake-------------------------
   {
      // First construct an init packet with packet id of 0 then send it
      std::ostringstream oss;
      boost::archive::text_oarchive oa(oss);
      int packID = NET_HS_REQ;
      oa << packID;
      socket_.send_to(boost::asio::buffer(oss.str()), serverEndPoint);
   }

   {
      // then wait for the answer back...
      size_t len = socket_.receive_from(boost::asio::buffer(recv_buffer_), serverEndPoint);
      int i;
      // decode the answer, and grab the assigned clientID
      std::istringstream iss(recv_buffer_.data());
      boost::archive::text_iarchive ia(iss);
      ia >> i;
      if (i == NET_HS_RES) {
         ia >> clientID;
         std::cout << "my clientID is " << clientID << std::endl;
      } else {
         std::cerr << "Wrong packet ID! iss.str()=" << iss.str() << std::endl;
         return;
      }
   }

   {
      // then do handshake 2
      std::ostringstream oss;
      boost::archive::text_oarchive oa(oss);
      int packID = NET_HS_ACK;
      oa << packID << clientID;
      socket_.send_to(boost::asio::buffer(oss.str()), serverEndPoint);
   }

   {
      // then wait for the answer back for handshake3...
      size_t len = socket_.receive_from(boost::asio::buffer(recv_buffer_), serverEndPoint);
      // decode the answer, and make sure that the packet id is 2
      std::istringstream iss(recv_buffer_.data());
      boost::archive::text_iarchive ia(iss);
      int temp;
      ia >> temp;
      if (temp == NET_HS_FIN) {
         std::cout << "Handshake Complete! I am now a client with ID of " << clientID << std::endl;
      }
   }
   // End handshake-------------------------

   start_receive();
}

UDP_Client::~UDP_Client() {
   //std::cout << "UDP_Client destructing..." << std::endl;

   // Send remvoe signal to the server
   std::cout << "Sending remove signal to the server, clientID:" << clientID << std::endl;
   std::ostringstream oss;
   int i = NET_KILL;
   boost::archive::text_oarchive oa(oss);
   oa << i;
   socket_.send_to(boost::asio::buffer(oss.str()), serverEndPoint);

   socket_.cancel();
   socket_.close();
}


void UDP_Client::start_receive() {
   socket_.async_receive_from(
      boost::asio::buffer(recv_buffer_), serverEndPoint,
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

   int receivedPackID = -1;
   std::istringstream iss(recv_buffer_.data());
   boost::archive::text_iarchive ia(iss);
   ia >> receivedPackID;

   if (receivedPackID == 100) {
      std::cout << "got shard!" << std::endl;
      NetShard newTestNetShard;
      ia >> newTestNetShard;
      Object3D* newObject = NULL;
      newTestNetShard.toObject(gameState, newObject);
      if (newObject != NULL) {
         gameState->custodian.add(newObject);
      } else {
         std::cerr << "Unserialization failed!" << std::endl;
      }
   } else {
      std::cerr << "unknown packet ID revieved: " << receivedPackID << std::endl;
      std::cerr << "iss.str()=" << iss.str() << std::endl;
   }

   start_receive();

}

//The parameters are message being handled, error code, and bytes transfered
void UDP_Client::handle_send(boost::shared_ptr<std::string>,
                  const boost::system::error_code&,
                  std::size_t){
   //std::cout << "UDP_Client running handle_send..." << std::endl;
}

void UDP_Client::send(std::string msg, boost::asio::ip::udp::endpoint dest) {
      boost::shared_ptr<std::string> message(
          new std::string(msg));

      socket_.async_send_to(boost::asio::buffer(*message), dest,
          boost::bind(&UDP_Client::handle_send, this, message,
            boost::asio::placeholders::error,
            boost::asio::placeholders::bytes_transferred));
}

