#include <iostream>
#include <cassert>
#include "csapp.h"
#include "message.h"
#include "message_serialization.h"
#include "server.h"
#include "exceptions.h"
#include "client_connection.h"

ClientConnection::ClientConnection( Server *server, int client_fd )
  : m_server( server )
  , m_client_fd( client_fd )
{
  rio_readinitb( &m_fdbuf, m_client_fd );
}

ClientConnection::~ClientConnection()
{
  delete m_server;
}

void ClientConnection::chat_with_client()
{
  while (true) {
    char buf[1024];
    
  }
}

// TODO: additional member functions
