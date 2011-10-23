/**
 * ClientSide.h - The client side of a network connection.
 * @author Sterling Hirsh
 * @date 10-22-11
 */

#ifndef __CLIENTSIDE_H__
#define __CLIENTSIDE_H__

// This errors if we don't include it here.
#include <enet/enet.h>

class GameState;

class ClientSide {
   public:
      ClientSide(GameState* _gameState);
      ~ClientSide();

      void connect(char* stringAddr);
      
      void receive();
      void send(char* dataToSend, int length, bool reliable);

   private:
      ENetHost* client;
      ENetEvent* event;
      ENetAddress* serverAddress;
      ENetPeer* server;
      GameState* gameState;
};

#endif
