/**
 * @file
 * UDP_Server class implementation.
 * <pre>
 * Inherits from UDP_Connection class. Handles Server side connection stuff.
 * </pre>
 *
 * @author Ryuho Kudo
 */

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

#include "Network/UDP_Server.h"
#include "Network/ClientCommand.h"
#include "Utility/GameState.h"
#include "Items/AsteroidShip.h"

//Constructor
UDP_Server::UDP_Server(boost::asio::io_service& io_service, GameState* _GameState)
: UDP_Connection(io_service, _GameState) {
   //std::cout << "UDP_Server running constructor..." << std::endl;
   clientIDCounter = 0;
   start_receive();
}

UDP_Server::~UDP_Server() {
   //std::cout << "UDP_Server destructing..." << std::endl;
}


void UDP_Server::start_receive() {
   std::cout << "UDP_Server running start_receive..." << std::endl;
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
   std::map<boost::asio::ip::udp::endpoint, unsigned>::iterator realIter = remoteClients.find(tempEndPoint);
   
   int receivedPackID;

   // It is not in the main list
   if (realIter == remoteClients.end()) {
      // It is not in the temp list
      std::map<boost::asio::ip::udp::endpoint, unsigned>::iterator tempIter = tempRemoteClients.find(tempEndPoint);
      if (tempIter == tempRemoteClients.end()) {
         std::cout << "new client found: " << tempEndPoint << std::endl;
         // test to see if it's an init packet, if it is not, well then throw it away!
         {
            std::istringstream iss(recv_buffer_.data());
            boost::archive::text_iarchive ia(iss);
            ia >> receivedPackID;
         }
         if (receivedPackID == 0) {
            std::cout << "init packet found, adding with ID of " << clientIDCounter << std::endl;
            tempRemoteClients.insert( std::pair<boost::asio::ip::udp::endpoint, unsigned>(boost::asio::ip::udp::endpoint(tempEndPoint),clientIDCounter) );
            

            // now send the client handshake1 back
            {
               std::ostringstream oss;
               boost::archive::text_oarchive oa(oss);
               oa << clientIDCounter;

               send(oss.str(), tempEndPoint);
            }
            clientIDCounter++;
         } else {
            std::cout << "Error! this packet is not init packet:" <<  receivedPackID << ". |||" << recv_buffer_.data() << std::endl;
         }
      } else {
         int clientIDCounter = realIter->second;
         std::cout << "handshake2 initiated with client id client found, id:" << clientIDCounter << std::endl;
         std::istringstream iss(recv_buffer_.data());
         boost::archive::text_iarchive ia(iss);
         ia >> receivedPackID;
         if (receivedPackID == 1) {
            std::cout << "Got handshake2 packet!" << std::endl;
            int tempClientID;
            ia >> tempClientID;
            if (tempClientID == clientIDCounter) {
               std::cout << "The ID is correct! Adding it to the game for good!" << std::endl;
               // now send the client handshake3 back
               {
                  int i = 2;
                  std::ostringstream oss;
                  boost::archive::text_oarchive oa(oss);
                  oa << i;
                  send(oss.str(), tempEndPoint);
               }
               remoteClients.insert( std::pair<boost::asio::ip::udp::endpoint, unsigned>(boost::asio::ip::udp::endpoint(tempEndPoint), clientIDCounter) );
               tempRemoteClients.erase (tempEndPoint);
               gameState->addNetworkPlayer(clientIDCounter);
            } else {
               std::cout << "Wrong ID? Removing this ID/Client. tempClientID=" << tempClientID << "|realIter->second=" << clientIDCounter << std::endl;
               tempRemoteClients.erase (tempEndPoint);
            }
         } else {
            std::cout << "Error! Handshake failed! receivedPackID=" <<  receivedPackID << std::endl;
         }
      }
   // It's a client that already has send packets before
   } else {
      int clientIDCounter = realIter->second;
      std::cout << "old client found, id:" << clientIDCounter << "| address:" << tempEndPoint << std::endl;
      std::istringstream iss(recv_buffer_.data());
      boost::archive::text_iarchive ia(iss);
      ia >> receivedPackID;

      if (receivedPackID == 3) {
         std::cout << "Got ClientCommand packet! Applying it to client id: " << clientIDCounter << std::endl;
         ClientCommand tempCommand;
         ia >> tempCommand;
         tempCommand.print();
         std::map<unsigned, AsteroidShip*>::iterator iterShip = gameState->custodian.shipsByClientID.find(clientIDCounter);
         if (iterShip == gameState->custodian.shipsByClientID.end()) {
            std::cout << "umm something went wrong.. client id is invalid?" << std::endl;
         } else {
            iterShip->second->readCommand(tempCommand);
         }

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

