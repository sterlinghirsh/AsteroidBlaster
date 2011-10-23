/**
 * ServerSide.h - The server side of a network connection.
 * @author Sterling Hirsh
 * @date 10-22-11
 */

#ifndef __SERVERSIDE_H__
#define __SERVERSIDE_H__

// This errors if we don't include it here.
#include <enet/enet.h>

class GameState;

namespace ast {
   class Frame;
}

class ServerSide {
   public:
      ServerSide(GameState* _gameState);
      ~ServerSide();

      void receive();
      void send(const ast::Frame& frame, bool reliable);

   private:
      ENetHost* server;
      ENetEvent* event;
      ENetAddress* serverAddress;
      // TODO: Change this to a map or something.
      ENetPeer* client;
      GameState* gameState;
};

#endif

