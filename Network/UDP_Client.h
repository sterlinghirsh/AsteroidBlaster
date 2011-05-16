/**
 * @file
 * UDP_Client class header.
 * <pre>
 * Inherits from UDP_Connection
 * </pre>
 *
 * @author Ryuho Kudo
 */

#ifndef UDP_CLIENT_H
#define UDP_CLIENT_H

#include "Network/UDP_Connection.h"

class UDP_Client: public UDP_Connection {
   //public variables
   public:
      int clientID;

   //private variables
   private:

   //public functions
   public:
      //constructor
      UDP_Client(boost::asio::io_service& io_service, GameState* gameState);
      ~UDP_Client();

   //private functions
   private:
      virtual void start_receive();
      virtual void handle_receive(const boost::system::error_code& error, std::size_t );
      virtual void handle_send(boost::shared_ptr<std::string>,
                              const boost::system::error_code&,
                              std::size_t);
};

#endif
