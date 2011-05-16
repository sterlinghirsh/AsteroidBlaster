/**
 * @file
 * UDP_Server class header.
 * <pre>
 * Inherits from UDP_Connection
 * </pre>
 *
 * @author Ryuho Kudo
 */

#ifndef UDP_SERVER_H
#define UDP_SERVER_H

#include "Network/UDP_Connection.h"

class UDP_Server: public UDP_Connection {
   //public variables
   public:
      unsigned clientIDCounter;
   //private variables
   private:

   //public functions
   public:
      //constructor
      UDP_Server(boost::asio::io_service& io_service, GameState* gameState);
      ~UDP_Server();

   //private functions
   private:
      virtual void start_receive();
      virtual void handle_receive(const boost::system::error_code& error, std::size_t );
      virtual void handle_send(boost::shared_ptr<std::string>,
                              const boost::system::error_code&,
                              std::size_t);
};

#endif
