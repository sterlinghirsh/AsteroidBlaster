/**
 * @file
 * UDP_Server class implementation.
 * <pre>
 * This class is an aiso::io_service. It takes care of getting UDP packets and
 * sending UDP packets.
 * </pre>
 *
 * @author Ryuho Kudo
 */

#include "Network/UDP_Server.h"

#include "Utility/GameState.h"
#include "Utility/Constants.h"

#include "Items/AsteroidShip.h"

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

using namespace boost::asio::ip;

//Constructor
UDP_Server::UDP_Server(boost::asio::io_service& io_service, GameState* _GameState, unsigned _portNumber)
: socket_(io_service, boost::asio::ip::udp::endpoint(boost::asio::ip::udp::v4(), (short unsigned int) _portNumber)), gameState(_GameState) {
   //std::cout << "UDP_Server running constructor..." << std::endl;
   std::cout << "UDP_Server being constructed with port #" << _portNumber << std::endl;
   clientIDCounter = 0;
   start_receive();
}

UDP_Server::~UDP_Server() {
   //std::cout << "UDP_Server destructing..." << std::endl;
   socket_.cancel();
   socket_.close();
}


void UDP_Server::start_receive() {
   memset(&recv_buffer_[0], 0, PACK_SIZE);
   socket_.async_receive_from(
      boost::asio::buffer(recv_buffer_), tempEndPoint,
      boost::bind(&UDP_Server::handle_receive, this,
      boost::asio::placeholders::error,
      boost::asio::placeholders::bytes_transferred));
   //std::cout << "UDP_Server running start_receive()..." << std::endl;
}

void UDP_Server::handle_receive(const boost::system::error_code& error, std::size_t ) {
   //std::cout << "UDP_Server running handle_receive..." << std::endl;
   // catch error
   if (error && error != boost::asio::error::message_size) {
      std::cout << "UDP_Server::handle_receive FAILED stopping..." << std::endl;
      return;
   }

   // See if this is a new client or not...
   std::map<boost::asio::ip::udp::endpoint, unsigned>::iterator realIter = endpointToClientID.find(tempEndPoint);
   
   int receivedPackID;

   // It is not in the main list
   if (realIter == endpointToClientID.end()) {
      // It is not in the temp list
      std::map<boost::asio::ip::udp::endpoint, unsigned>::iterator tempIter = tempEndpointToClientID.find(tempEndPoint);
      if (tempIter == tempEndpointToClientID.end()) {
         std::cout << "new client found: " << tempEndPoint << std::endl;
         // test to see if it's an init packet, if it is not, well then throw it away!
         {
            std::istringstream iss(recv_buffer_.data());
            boost::archive::text_iarchive ia(iss);
            ia >> receivedPackID;
         }
         if (receivedPackID == NET_HS_REQ) {
            std::cout << "init packet found, adding with ID of " << clientIDCounter << std::endl;
            tempEndpointToClientID.insert( std::pair<boost::asio::ip::udp::endpoint, unsigned>(boost::asio::ip::udp::endpoint(tempEndPoint),clientIDCounter) );
            

            // now send the client handshake1 back
            {
               std::ostringstream oss;
               boost::archive::text_oarchive oa(oss);
               int packID = NET_HS_RES;
               oa << packID << clientIDCounter;
               send(oss.str(), tempEndPoint);
            }
            clientIDCounter++;
         } else {
            std::cout << "Error! this packet is not init packet:" <<  receivedPackID << ". |||" << recv_buffer_.data() << std::endl;
         }
      } else {
         int currentClientID = tempIter->second;
         std::cout << "handshake2 initiated with client id client found, id:" << currentClientID << std::endl;
         std::istringstream iss(recv_buffer_.data());
         boost::archive::text_iarchive ia(iss);
         ia >> receivedPackID;
         if (receivedPackID == NET_HS_ACK) {
            std::cout << "Got handshake2 packet!" << std::endl;
            int tempClientID;
            ia >> tempClientID;
            if (tempClientID == currentClientID) {
               std::cout << "The ID is correct! Adding it to the game for good!" << std::endl;
               // now send the client handshake3 back
               {
                  std::ostringstream oss;
                  boost::archive::text_oarchive oa(oss);
                  int packID = NET_HS_FIN;
                  oa << packID;
                  send(oss.str(), tempEndPoint);
               }
               endpointToClientID.insert( std::pair<boost::asio::ip::udp::endpoint, unsigned>(boost::asio::ip::udp::endpoint(tempEndPoint), currentClientID) );
               tempEndpointToClientID.erase (tempEndPoint);
               gameState->addNetworkPlayer(currentClientID);
            } else {
               std::cout << "Wrong ID? Removing this ID/Client. tempClientID=" << tempClientID << "|currentClientID=" << currentClientID << std::endl;
               tempEndpointToClientID.erase (tempEndPoint);
            }
         } else {
            std::cout << "Error! Handshake failed! receivedPackID=" <<  receivedPackID << std::endl;
         }
      }
   // It's a client that already has send packets before
   } else {
      int currentClientID = realIter->second;
      //std::cout << "old client found, id:" << currentClientID << "| address:" << tempEndPoint << std::endl;
      std::istringstream iss(recv_buffer_.data());
      boost::archive::text_iarchive ia(iss);
      ia >> receivedPackID;

      AsteroidShip* curShip = NULL;
      //look for the ship associated with this client
      std::map<unsigned, AsteroidShip*>::iterator iterShip = gameState->custodian.shipsByClientID.find(currentClientID);
      if (iterShip == gameState->custodian.shipsByClientID.end()) {
         std::cout << "umm something went wrong.. client id is invalid?" << std::endl;
      } else {
         curShip = iterShip->second;
      }

      if (receivedPackID == NET_CLIENTCOMMAND) {
         //std::cout << "Got ClientCommand packet! Applying it to client id: " << currentClientID << std::endl;
         ClientCommand tempCommand;
         ia >> tempCommand;
         curShip->readCommand(tempCommand);

      } else if (receivedPackID == NET_KILL) {
         std::cout << "Client ID:" << currentClientID << " quit!" << std::endl;
         endpointToClientID.erase (tempEndPoint);
         curShip->shouldRemove = true;
      } else {
         std::cout << "Error! this packet id is unknown:" <<  receivedPackID << ". |||" << recv_buffer_.data() << std::endl;
      }
   }

   start_receive();

}

//The parameters are message being handled, error code, and bytes transfered
void UDP_Server::handle_send(boost::shared_ptr<std::string>,
                  const boost::system::error_code&,
                  std::size_t){
   //std::cout << "UDP_Server running handle_send..." << std::endl;
}

void UDP_Server::send(std::string msg, boost::asio::ip::udp::endpoint dest) {
   boost::shared_ptr<std::string> message(
       new std::string(msg));

   socket_.async_send_to(boost::asio::buffer(*message), dest,
       boost::bind(&UDP_Server::handle_send, this, message,
         boost::asio::placeholders::error,
         boost::asio::placeholders::bytes_transferred));
}

void UDP_Server::sendAll(std::string msg) {
   boost::shared_ptr<std::string> message(
       new std::string(msg));
   std::map<boost::asio::ip::udp::endpoint, unsigned>::iterator iter = endpointToClientID.begin();

   for ( ; iter != endpointToClientID.end(); iter++ ) {
      socket_.async_send_to(boost::asio::buffer(*message), iter->first,
          boost::bind(&UDP_Server::handle_send, this, message,
            boost::asio::placeholders::error,
            boost::asio::placeholders::bytes_transferred));
   }
}

