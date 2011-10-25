/**
 * ServerSide.h - The client side of a network connection.
 * @author Sterling Hirsh
 * @date 10-22-11
 */

#include "Network/ServerSide.h"
#include "Network/NetUtility.h"
#include "Network/gamestate.pb.h"
#include "Network/ClientInfo.h"
#include "Utility/GlobalUtility.h"
#include "Utility/GameState.h"
#include <enet/enet.h>
#include <iostream>
// enet is included from ServerSide.h

using namespace std;

static string outgoing;

ServerSide::ServerSide(GameState* _gameState) :
 gameState(_gameState) {
   serverAddress = new ENetAddress();
   serverAddress->host = ENET_HOST_ANY;
   serverAddress->port = NET_PORT;

   server = enet_host_create(serverAddress, // Server host
         NET_MAX_CLIENTS, // 16 Outgoing connection
         NET_CHANNELS,
         0, // 
         0);
   if (server == NULL) {
      cerr << "Could not create server!" << endl;
      exit(EXIT_FAILURE);
   }

   event = new ENetEvent();
}

ServerSide::~ServerSide() {
   if (server != NULL)
      enet_host_destroy(server);
   
   delete event;
   delete serverAddress;
}

void ServerSide::receive() {
   // 0 timeout for no wait.
   int result;
   do {
      result = enet_host_service(server, event, 0);

      if (result < 0) {
         cerr << "Error receiving!" << endl;
         exit(EXIT_FAILURE);
      }

      if (result > 0) {
         // If a new packet came in.
         if (event->type == ENET_EVENT_TYPE_DISCONNECT) {
            cout << "Client disconnected. No big." << endl;
            // I guess this is how you do it?
            event->peer->data = NULL;
         }

         if (event->type == ENET_EVENT_TYPE_CONNECT) {
            cout << "Client connected! That's normal." << endl;

            unsigned shipid = gameState->addNetworkPlayer(event->peer->connectID);
            clients[event->peer->connectID] = new ClientInfo(shipid, event->peer);

            cout << "Adding ship with clientid " << event->peer->connectID << 
               " and id " << shipid << "." << endl;

            // Make sure the new ship is there.
            gameState->custodian.update();
            gameState->storeFullGameState();

            ast::Frame frame;
            frame.set_shipid(shipid);
            ast::GameState* gs = gameState->getLastSavedGameState();
            frame.mutable_gamestate()->CopyFrom(*gs);
            send(event->peer, frame, true);
         }

         if (event->type == ENET_EVENT_TYPE_RECEIVE) {
            ast::ClientCommand cc;
            cc.ParseFromArray(event->packet->data, event->packet->dataLength);
            gameState->handleCommand(cc);

            if (cc.has_lastreceivedgamestateid())
               clients[event->peer->connectID]->ackGameState = cc.lastreceivedgamestateid();

            //cout << "Packet received from client: " << (char*) event->packet->data << endl;
         }

      }
      if (event->packet != NULL) {
         enet_packet_destroy(event->packet);
      }
   } while (result > 0);
}

void ServerSide::send(_ENetPeer* client, const ast::Frame& frame, bool reliable) {
   if (client == NULL)
      return;

   if (!frame.SerializeToString(&outgoing)) {
      cerr << "Failed to serialize frame!" << endl;
   }

   ENetPacket* packet = enet_packet_create(outgoing.c_str(), outgoing.length(), 
    reliable ? ENET_PACKET_FLAG_RELIABLE : 0);
   // The 0 here is the channel. 
   enet_peer_send(client, 0, packet);
   enet_host_flush(server);
}


std::map<unsigned, ClientInfo*>& ServerSide::getClients() {
   return clients;
}
