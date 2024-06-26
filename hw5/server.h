#ifndef SERVER_H
#define SERVER_H

#include <iostream>
#include <map>
#include <string>
#include <pthread.h>
#include "table.h"
#include "client_connection.h"

class Server {
private:
  // TODO: add member variables
  std::vector<Table*> tables;
  int server_fd;
  // copy constructor and assignment operator are prohibited
  Server( const Server & );
  Server &operator=( const Server & );

public:
  Server();
  ~Server();

  void listen( const std::string &port );
  void server_loop();

  static void *client_worker( void *arg );

  void log_error( const std::string &what );

  // TODO: add member functions
  void addTable(Table* table) {
    tables.push_back(table);
  }

  Table* getTable(std::string& name) {
    for (std::vector<Table*>::iterator it = tables.begin(); it != tables.end(); it++) {
      if ((*it)->get_name() == name) {
        return *it;
      }
    }
    return nullptr;
  } 
  // Some suggested member functions:
/*
  void create_table( const std::string &name );
  Table *find_table( const std::string &name );
*/
};


#endif // SERVER_H
