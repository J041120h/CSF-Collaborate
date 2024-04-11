#include <set>
#include <map>
#include <regex>
#include <string>
#include <vector>
#include <cassert>
#include "message.h"
#include "exceptions.h"

Message::Message()
  : m_message_type(MessageType::NONE)
{
}

Message::Message( MessageType message_type, std::initializer_list<std::string> args )
  : m_message_type( message_type )
  , m_args( args )
{
}

Message::Message( const Message &other )
  : m_message_type( other.m_message_type )
  , m_args( other.m_args )
{
}

Message::~Message()
{
}

Message &Message::operator=( const Message &rhs )
{
  this->m_message_type = rhs.m_message_type;
  for (std::vector<std::string>::const_iterator it = rhs.m_args.begin(); it != rhs.m_args.end(); it++) {
    this->push_arg(*it);
  }
  return *this;
}

MessageType Message::get_message_type() const
{
  return m_message_type;
}

void Message::set_message_type(MessageType message_type)
{
  m_message_type = message_type;
}

std::string Message::get_username() const
{
  if (m_message_type != MessageType::LOGIN) {
    throw new std::invalid_argument("Only login message have username field\n");
  }
  if (!this->is_valid()) {
    throw new std::invalid_argument("Invalid argument value\n");
  }
  return get_arg(1);
}

std::string Message::get_table() const
{
  if (m_message_type != MessageType::SET && m_message_type != MessageType::GET && m_message_type != MessageType::CREATE) {
    throw new std::invalid_argument("Only set and get message have table field\n");
  }
  if (!this->is_valid()) {
    throw new InvalidMessage("Invalid argument value\n");
  }
  return get_arg(1);
}

std::string Message::get_key() const
{
  if (m_message_type != MessageType::SET && m_message_type != MessageType::GET) {
    throw new std::invalid_argument("Only set and get message have key field\n");
  }
  if (!this->is_valid()) {
    throw new InvalidMessage("Invalid argument value\n");
  }
  return get_arg(2);
}

std::string Message::get_value() const
{
  if (m_message_type != MessageType::PUSH && m_message_type != MessageType::DATA) {
    throw new std::invalid_argument("Only push and data message have value field\n");
  }
  if (!this->is_valid()) {
    throw new InvalidMessage("Invalid argument value\n");
  }
  return get_arg(1);
}

std::string Message::get_quoted_text() const
{
  if (m_message_type != MessageType::FAILED && m_message_type != MessageType::ERROR) {
    throw new std::invalid_argument("Only failed and error message have quoted_text field\n");
  }
  if (!this->is_valid()) {
    throw new InvalidMessage("Invalid argument value\n");
  }
  return get_arg(1);
}

void Message::push_arg( const std::string &arg )
{
  m_args.push_back( arg );
}

bool Message::is_valid() const
{
  if (!is_num_match()) {
    return false;
  }
  int length = 0;
  for (std::vector<std::string>::const_iterator it = m_args.begin(); it != m_args.end(); it++) {
    if (it->length() == 0) {
      return false;
    }
    length += it->length();
  }
  if (length + 1 > MAX_ENCODED_LEN) {
    return false;
  }
  if (m_message_type == MessageType::CREATE || m_message_type == MessageType::SET || m_message_type == MessageType::GET || m_message_type == MessageType::LOGIN) {
    for (int i = 1; i < m_args.size(); i++) {
      if(!Message::is_letter(m_args[i][0])) {
        return false;
      }
      for (int j = 1; j < m_args[i].length(); i++) {
        if(!Message::is_letter(m_args[i][j]) && !Message::is_number(m_args[i][j]) && m_args[i][j] != '_') {
          return false;
        }
      }
    }
  }
  if (m_message_type == MessageType::PUSH || m_message_type == MessageType::DATA) {
    for (int j = 0; j < m_args[1].length(); j++) {
      if (m_args[1][j] == ' ') {
        return false;
      }
    }
  }
  if (m_message_type == MessageType::FAILED || m_message_type == MessageType::ERROR) {
    if (m_args[1][0] != '"' || m_args[1][m_args[1].size()-1] != '"') {
      return false;
    }
    for (int i = 1; i < m_args[1].size() - 1; i++) {
      if (m_args[1][i] == '"') {
        return false;
      }
    }
  }
  return true;
}

bool Message::is_num_match() const {
  std::map<MessageType, int> map = {
    {MessageType::LOGIN, 2},
    {MessageType::CREATE, 2},
    {MessageType::PUSH, 2},
    {MessageType::POP, 1},
    {MessageType::TOP, 1},
    {MessageType::SET, 3},
    {MessageType::GET, 3},
    {MessageType::ADD, 1},
    {MessageType::MUL, 1},
    {MessageType::SUB, 1},
    {MessageType::DIV, 1},
    {MessageType::BEGIN, 1},
    {MessageType::COMMIT, 1},
    {MessageType::BYE, 1},
    {MessageType::OK, 1},
    {MessageType::FAILED, 2},
    {MessageType::ERROR, 2},
    {MessageType::DATA, 2}
  };
  if (map[m_message_type] != m_args.size()) {
    return false;
  }
  return true;
}
