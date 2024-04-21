#include <fcntl.h>
#include <iostream>
#include <cassert>
#include <memory>
#include <stdexcept>
#include "csapp.h"
#include "exceptions.h"
#include "guard.h"
#include "server.h"

Server::Server()
  // TODO: initialize member variables
{
  // TODO: implement
}

Server::~Server()
{
  // TODO: implement
}

void Server::listen( const std::string &port )
{
  // TODO: implement
  int server_fd = open_listenfd(port.data());
  if (server_fd < 0) { this->log_error("Couldn't open server socket\n"); }
}

void Server::server_loop()
{
  // TODO: implement
  int keep_going = 1;
  while (keep_going) {
      int client_fd = Accept(server_fd, NULL, NULL);
      if (client_fd > 0) {
          ClientConnection *client = new ClientConnection( this, client_fd );
          pthread_t thr_id;
          if ( pthread_create( &thr_id, nullptr, client_worker, client ) != 0 ) {
            log_error( "Could not create client thread" );
          }
          pthread_join(thr_id, NULL);
          close(client_fd); // close the connection
      }
  }
  close(server_fd);
  // Note that your code to start a worker thread for a newly-connected
  // client might look something like this:

}


void *Server::client_worker( void *arg )
{
  // TODO: implement

  // Assuming that your ClientConnection class has a member function
  // called chat_with_client(), your implementation might look something
  // like this:
/*
  std::unique_ptr<ClientConnection> client( static_cast<ClientConnection *>( arg ) );
  client->chat_with_client();
  return nullptr;
*/
}

void Server::log_error( const std::string &what )
{
  std::cerr << "Error: " << what << "\n";
}

// TODO: implement member functions
