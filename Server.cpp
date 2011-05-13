/**
 * @file
 * Server class implementation.
 * <pre>
 * This class is multithreaded. It uses UDP_Connection to handle the networking.
 * </pre>
 *
 * @author Ryuho Kudo
 */


#include "Server.h"

UDP_Connection* udpConnection;

Server::Server(boost::asio::io_service& io) : 
      mainStrand(io), 
      gameThread(io, boost::posix_time::seconds(0)), 
      networkThread(io, boost::posix_time::seconds(0)),
      mainCount(0),
      count1(0),
      count2(0) {
   gameThread.async_wait(mainStrand.wrap(boost::bind(&Server::gameFunction, this)));
   networkThread.async_wait(mainStrand.wrap(boost::bind(&Server::networkFunction, this)));
}

Server::~Server() {
   std::cout << "Final count is " << mainCount << "\n";
}

void Server::gameFunction() {
   return;
   std::cout << "Timer 1: " << mainCount << "\n";
   ++mainCount;
   ++count1;

   gameThread.expires_at(gameThread.expires_at() + boost::posix_time::seconds(1));
   gameThread.async_wait(mainStrand.wrap(boost::bind(&Server::gameFunction, this)));
}

void Server::networkFunction() {
   return;
   if (count2 < 5) {
      std::cout << "Timer 2: " << mainCount << "\n";
      ++mainCount;
      ++count2;

      networkThread.expires_at(networkThread.expires_at() + boost::posix_time::seconds(0));
      networkThread.async_wait(mainStrand.wrap(boost::bind(&Server::networkFunction, this)));
   }
}

int main() {
   boost::asio::io_service io;

   Server p(io);
   udpConnection = new UDP_Connection(io);
   boost::thread t(boost::bind(&boost::asio::io_service::run, &io));
   io.run();
   t.join();

   return 0;
}

