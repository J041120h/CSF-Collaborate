#include "value_stack.h"
#include "exceptions.h"
#include <stdexcept>

ValueStack::ValueStack()
  // TODO: initialize member variable(s) (if necessary)
{
}

ValueStack::~ValueStack()
{
}

bool ValueStack::is_empty() const
{
  // TODO: implement
  return storage.empty();
}

void ValueStack::push( const std::string &value )
{
  // TODO: implement
  storage.push(value);
}

std::string ValueStack::get_top() const
{
  // TODO: implement
  if (storage.empty()) {
    throw OperationException("Error: empty stack");
  } else {
    return storage.top();
  }
}

void ValueStack::pop()
{
  // TODO: implement
  if (storage.empty()) {
    throw OperationException("Error: empty stack");
  } else {
    storage.pop();
  }
}
