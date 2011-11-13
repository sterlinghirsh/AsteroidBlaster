/**
 * ClientSide.h - The client side of a network connection.
 * @author Sterling Hirsh
 * @date 10-22-11
 */

#include "Network/ClientSide.h"
#include "Network/NetUtility.h"
#include "Network/gamestate.pb.h"
#include "Utility/GlobalUtility.h"
#include "Utility/GameState.h"
#include <iostream>
#include <string>
// enet is included from ClientSide.h

using namespace std;

static string outgoing;

ClientSide::ClientSide(GameState* _gameState) :
 gameState(_gameState) {
   client = enet_host_create(NULL, // Client host
         1, // 1 Outgoing connection
         NET_CHANNELS,
         0, // 
         0);
   if (client == NULL) {
      cerr << "Could not create client!" << endl;
      exit(EXIT_FAILURE);
   }

   event = new ENetEvent();
   serverAddress = new ENetAddress();
   server = NULL;
}

ClientSide::~ClientSide() {
   if (client != NULL)
      enet_host_destroy(client);
   
   if (server != NULL) {
      enet_peer_reset(server);
      server->data = NULL;
   }

   delete event;
   delete serverAddress;
}

void ClientSide::receive() {
   // 0 timeout for no wait.
   int result;
   do {
      result= enet_host_service(client, event, 0);

      if (result < 0) {
         cerr << "Error receiving!" << endl;
         exit(EXIT_FAILURE);
      }

      if (result > 0) {
         // If a new packet came in.
         if (event->type == ENET_EVENT_TYPE_DISCONNECT) {
            cout << "Server disconnected. Exiting." << endl;
            exit(EXIT_SUCCESS);
         }

         if (event->type == ENET_EVENT_TYPE_CONNECT) {
            cout << "Server... connecting? That's weird." << endl;
            exit(EXIT_FAILURE);
         }

         if (event->type == ENET_EVENT_TYPE_RECEIVE) {
            //cout << "Packet received from server: " << (char*) event->packet->data << endl;
            ast::Frame frame;
            frame.ParseFromArray(event->packet->data, event->packet->dataLength);
            gameState->handleFrame(&frame);
         }

      }

      if (event->packet != NULL) {
         enet_packet_destroy(event->packet);
      }
   } while (result > 0);
}

void ClientSide::disconnect() {
   // 0 is "data" to send with the disconnection.
   enet_peer_disconnect(server, 0);
   enet_host_flush(client);
}

void ClientSide::connect(char* stringAddr) {
   enet_address_set_host(serverAddress, stringAddr);
   serverAddress->port = NET_PORT;

   // The 0 is "data to send to the server."
   server = enet_host_connect(client, serverAddress, NET_CHANNELS, 0);
   if (server == NULL) {
      cerr << "Could not connect to server." << endl;
      exit(EXIT_FAILURE);
   }

   int result = enet_host_service(client, event, NET_TIMEOUT);
   if (result == 0) {
      cerr << "Connection to server timed out!" << endl;
      exit(EXIT_FAILURE);
   } else if (result < 0) {
      cerr << "An error occurred while trying to connect." << endl;
      exit(EXIT_FAILURE);
   }

   if (event->type != ENET_EVENT_TYPE_CONNECT) {
      cerr << "Got a different response than connect." << endl;
      exit(EXIT_FAILURE);
   }

   // We're not doing the event->peer->data = (void*) "Server Info"; thing.
   // Not even sure what it's for.
   
   // Looks like the connection worked!
   cout << "Connected!" << endl;
}

void ClientSide::send(const ast::ClientCommand& clientCommand, bool reliable) {
   if (!clientCommand.has_shipid()) {
      cerr << "No shipid. Not sending." << endl;
      return;
   }

   if (!clientCommand.SerializeToString(&outgoing)) {
      cerr << "Failed to serialize clientCommand!" << endl;
      return;
   }
   
   ENetPacket* packet = enet_packet_create(outgoing.c_str(), outgoing.length(), 
    reliable ? ENET_PACKET_FLAG_RELIABLE : 0);
   // The 0 here is the channel. 
   enet_peer_send(server, 0, packet);
   enet_host_flush(client);
}

