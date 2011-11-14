/**
 * Store info about a client here. We can point the client's data pointer here.
 * @author Sterling Hirsh
 * @date 10-24-11
 */

#ifndef __CLIENTINFO_H__
#define __CLINETINFO_H__

struct _ENetPeer;

class ClientInfo {
   public:
      ClientInfo(unsigned _shipid, _ENetPeer* _client);
      unsigned shipid;
      unsigned ackGameState;
      _ENetPeer* client;
      bool connected;
};

#endif
