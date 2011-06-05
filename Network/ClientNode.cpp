
#include "Network/ClientNode.h"
#include "Utility/Timer.h"

ClientNode::ClientNode(boost::asio::ip::udp::endpoint _endpoint,
                        int _clientID){
   endpoint = _endpoint;
   clientID = _clientID;
   shipID = -1;
   lastAccessed = Timer();

}

