#include <cassert>
#include <exception>
#include <pthread.h>
#include <stdexcept>
#include "table.h"
#include "exceptions.h"
#include "guard.h"

Table::Table( const std::string &name )
  : m_name( name )
  // TODO: initialize additional member variables
{
  // Nothing to initialize in the first place
  pthread_mutex_init(&lockStatus, NULL);
}

Table::~Table()
{
  // Nothing to manually deconstruct
}

void Table::lock()
{
  // TODO: implement
  pthread_mutex_lock(&lockStatus);
}

void Table::unlock()
{
  // TODO: implement
  pthread_mutex_unlock(&lockStatus);
}

bool Table::trylock()
{
  // Try to acquire the lock without blocking
    int result = pthread_mutex_trylock(&lockStatus);

    if (result == 0) {
        pthread_mutex_lock(&lockStatus);
        // Do work while the mutex is locked
        return true; // Indicate success
    } else if (result == EBUSY) {
        // The mutex could not be acquired because it was already locked
        return false;
    } else {
        // An error occurred
        throw std::runtime_error("pthread_mutex_trylock failed");
    }

    return false; // Indicate failure
}

void Table::set( const std::string &key, const std::string &value )
{
  // TODO: implement
  temp[key] = value;
}

std::string Table::get( const std::string &key )
{
  // TODO: implement
  if (temp.find(key) != temp.end()) {
    return temp[key];
  }
  throw InvalidMessage("Error: no such element");
}

bool Table::has_key( const std::string &key )
{
  // TODO: implement
  return temp.find(key) != temp.end();
}

void Table::commit_changes()
{
  // TODO: implement
  actual = temp;
}

void Table::rollback_changes()
{
  // TODO: implement
  temp = actual;
}
