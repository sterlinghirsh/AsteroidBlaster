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
#include "Utility/Music.h"

#include "Text/Text.h"

#include "Items/AsteroidShip.h"

#include "Menus/StoreMenu.h"

#include "Network/NetShard.h"
#include "Network/NetAsteroid.h"
#include "Network/NetShip.h"
#include "Network/NetBlasterShot.h"
#include "Network/NetBeamShot.h"

//Constructor
UDP_Client::UDP_Client(boost::asio::io_service& io_service, GameState* _GameState, std::string _ip, std::string _portNum)
: socket_(io_service, boost::asio::ip::udp::endpoint(boost::asio::ip::udp::v4(), 0)), gameState(_GameState) {

   gameState = _GameState;
   //std::cout << "UDP_Client running constructor..." << std::endl;
   std::cout << "UDP_Client being constructed which will try to connect IP:" 
             << _ip << " and port #" << _portNum << std::endl;
   clientID = -1;
   shipID = -1;

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
      socket_.receive_from(boost::asio::buffer(recv_buffer_), serverEndPoint);

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
      socket_.receive_from(boost::asio::buffer(recv_buffer_), serverEndPoint);

      // decode the answer, and make sure that the packet id is 2
      std::istringstream iss(recv_buffer_.data());
      boost::archive::text_iarchive ia(iss);
      int temp;
      ia >> temp;
      if (temp == NET_HS_FIN) {
         std::cout << "Handshake Complete! I am now a client with ID of " << clientID << std::endl;
      }
   }

   {
      // ask for the ship's ID
      std::ostringstream oss;
      boost::archive::text_oarchive oa(oss);
      int packID = NET_SHIPID_REQ;
      oa << packID;
      socket_.send_to(boost::asio::buffer(oss.str()), serverEndPoint);
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

   //socket_.cancel();
   socket_.close();
}


void UDP_Client::start_receive() {
   memset(&recv_buffer_[0], 0, PACK_SIZE);
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

   Object3D* newObject = NULL;
   bool created = false;

   if (receivedPackID == NET_OBJ_REMOVE) {
      //std::cout << "got object remove packet!" << std::endl;
      int removeID;
      ia >> removeID;
      Object3D* removeObj = gameState->custodian[removeID];
      if (removeObj) {
         removeObj->shouldRemove = true;
      } else {
         std::cout << "id:" << removeID << " not there!" << std::endl;
      }
   } 

   else if (receivedPackID == NET_OBJ_SHARD) {
      //std::cout << "got shard!" << std::endl;
      NetShard newTestNetShard;
      ia >> newTestNetShard;
      created = newTestNetShard.toObject(gameState, newObject);
   }

   else if (receivedPackID == NET_OBJ_ASTEROID) {
      //std::cout << "got asteroid!" << std::endl;
      NetAsteroid newNetAsteroid;
      ia >> newNetAsteroid;
      created = newNetAsteroid.toObject(gameState, newObject);
   }

   else if (receivedPackID == NET_OBJ_SHIP) {
      NetShip newNetShip;
      ia >> newNetShip;
      created = newNetShip.toObject(gameState, newObject);
   } 

   else if (receivedPackID == NET_OBJ_BLASTERSHOT) {
      NetBlasterShot newNetBlasterShot;
      ia >> newNetBlasterShot;
      created = newNetBlasterShot.toObject(gameState, newObject);
   } 

   else if (receivedPackID == NET_OBJ_BEAMSHOT) {
      NetBeamShot newNetBeamShot;
      ia >> newNetBeamShot;
      created = newNetBeamShot.toObject(gameState, newObject);
   } 

   else if (receivedPackID == NET_LEVEL_UPDATE) {
      double currTimer;
      int tempLevel;
      ia >> currTimer >> tempLevel;
      gameState->levelTimer.setCountDown(currTimer);
      gameState->curLevel = tempLevel;
      gameState->curLevelText->updateBody(gameState->curLevel);
   } 

   else if (receivedPackID == NET_ACTIVATE_STOREMENU) {
      storeMenu->menuActive = true;
      Music::stopMusic();
      Music::playMusic("8-bit3.ogg");
   } 



   else if (receivedPackID == NET_SHIPID_RES) {
      ia >> shipID;
      std::cout << "My ship ID is " << shipID << std::endl;
   }

   else {
      std::cerr << "unknown packet ID revieved: " << receivedPackID << std::endl;
   }

   if (created) {
      gameState->custodian.add(newObject);
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

