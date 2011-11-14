/**
 * Store info about a client here. We can point the client's data pointer here.
 * @author Sterling Hirsh
 * @date 10-24-11
 */

#include "Network/ClientInfo.h"

ClientInfo::ClientInfo(unsigned _shipid, _ENetPeer* _client) :
   shipid(_shipid), 
   ackGameState(0),
   client(_client),
   connected(true) {
   // Nothing to do.
}
