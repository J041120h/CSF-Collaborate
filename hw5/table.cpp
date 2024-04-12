#include <cassert>
#include <exception>
#include <stdexcept>
#include "table.h"
#include "exceptions.h"
#include "guard.h"

Table::Table( const std::string &name )
  : m_name( name )
  // TODO: initialize additional member variables
{
  // Nothing to initialize in the first place
  lockStatus = true;
}

Table::~Table()
{
  // Nothing to manually deconstruct
}

void Table::lock()
{
  // TODO: implement
  lockStatus = true;
}

void Table::unlock()
{
  // TODO: implement
  lockStatus = false;
}

bool Table::trylock()
{
  // TODO: implement
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
