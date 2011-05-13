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

//Constructor
UDP_Connection::UDP_Connection(boost::asio::io_service& io_service)
: socket_(io_service, boost::asio::ip::udp::endpoint(boost::asio::ip::udp::v4(), 5000)) {
   //std::cout << "UDP_Connection running constructor..." << std::endl;
   currClientID = 0;
   start_receive();
}

UDP_Connection::~UDP_Connection() {
   //std::cout << "UDP_Connection destructing..." << std::endl;
}


void UDP_Connection::start_receive() {
   socket_.async_receive_from(
      boost::asio::buffer(recv_buffer_), tempEndPoint,
      boost::bind(&UDP_Connection::handle_receive, this,
      boost::asio::placeholders::error,
      boost::asio::placeholders::bytes_transferred));
   //std::cout << "UDP_Connection running start_receive()..." << std::endl;
}

void UDP_Connection::handle_receive(const boost::system::error_code& error, std::size_t ) {
   //std::cout << "UDP_Connection running handle_receive..." << std::endl;
   // catch error
   if (error && error != boost::asio::error::message_size) {
      std::cout << "UDP_Connection::handle_receive FAILED stopping..." << std::endl;
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
            std::cout << "init packet found, adding with ID of " << currClientID << std::endl;
            tempRemoteClients.insert( std::pair<boost::asio::ip::udp::endpoint, unsigned>(boost::asio::ip::udp::endpoint(tempEndPoint),currClientID) );
            

            // now send the client handshake1 back
            {
               std::ostringstream oss;
               boost::archive::text_oarchive oa(oss);
               oa << currClientID;

               send(oss.str(), tempEndPoint);
            }
            currClientID++;
         } else {
            std::cout << "Error! this packet is not init packet:" <<  receivedPackID << ". |||" << recv_buffer_.data() << std::endl;
         }
      } else {
         std::cout << "handshake2 initiated with client id client found, id:" << tempIter->second << std::endl;
         std::istringstream iss(recv_buffer_.data());
         boost::archive::text_iarchive ia(iss);
         ia >> receivedPackID;
         if (receivedPackID == 1) {
            std::cout << "Got handshake2 packet!" << std::endl;
            int tempClientID;
            ia >> tempClientID;
            if (tempClientID == tempIter->second) {
               std::cout << "Is the correct ID!" << std::endl;
               // now send the client handshake3 back
               {
                  int i = 2;
                  std::ostringstream oss;
                  boost::archive::text_oarchive oa(oss);
                  oa << i;
                  send(oss.str(), tempEndPoint);
               }
               remoteClients.insert( std::pair<boost::asio::ip::udp::endpoint, unsigned>(boost::asio::ip::udp::endpoint(tempEndPoint),tempIter->second) );
               tempRemoteClients.erase (tempEndPoint);
            } else {
               std::cout << "Wrong ID? Removing this ID/Client. tempClientID=" << tempClientID << "|realIter->second=" << tempIter->second << std::endl;
               tempRemoteClients.erase (tempEndPoint);
            }
         } else {
            std::cout << "Error! Handshake failed! receivedPackID=" <<  receivedPackID << std::endl;
         }
      }
   // It's a client that already has send packets before
   } else {
      std::cout << "old client found, id:" << realIter->second << "| address:" << tempEndPoint << std::endl;
      std::istringstream iss(recv_buffer_.data());
      boost::archive::text_iarchive ia(iss);
      ia >> receivedPackID;

      if (receivedPackID == 3) {
         std::cout << "Got ClientCommand packet!" << std::endl;
         ClientCommand temp;
         ia >> temp;
         temp.print();
      } else {
         std::cout << "Error! this packet id is unknown:" <<  receivedPackID << ". |||" << recv_buffer_.data() << std::endl;
      }
   }

   start_receive();

}

//The parameters are message being handled, error code, and bytes transfered
void UDP_Connection::handle_send(boost::shared_ptr<std::string>,
                  const boost::system::error_code&,
                  std::size_t){
   //std::cout << "UDP_Connection running handle_send..." << std::endl;
}

void UDP_Connection::send(std::string msg, boost::asio::ip::udp::endpoint dest) {
      boost::shared_ptr<std::string> message(
          new std::string(msg));

      socket_.async_send_to(boost::asio::buffer(*message), dest,
          boost::bind(&UDP_Connection::handle_send, this, message,
            boost::asio::placeholders::error,
            boost::asio::placeholders::bytes_transferred));
}





