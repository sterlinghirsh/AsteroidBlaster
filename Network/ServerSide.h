/**
 * ServerSide.h - The server side of a network connection.
 * @author Sterling Hirsh
 * @date 10-22-11
 */

#ifndef __SERVERSIDE_H__
#define __SERVERSIDE_H__

// This errors if we don't include it here.
#include <map>

class GameState;

namespace ast {
   class Frame;
}

class ClientInfo;
class _ENetHost;
class _ENetEvent;
class _ENetAddress;
class _ENetPeer;

class ServerSide {
   public:
      ServerSide(GameState* _gameState);
      ~ServerSide();

      void receive();
      void send(_ENetPeer* client, const ast::Frame& frame, bool reliable);

      std::map<unsigned, ClientInfo*>& getClients();

   private:
      _ENetHost* server;
      _ENetEvent* event;
      _ENetAddress* serverAddress;
      // TODO: Change this to a map or something.
      GameState* gameState;
      std::map<unsigned, ClientInfo*> clients;
};

#endif

