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
  return get_arg(0);
}

std::string Message::get_table() const
{
  if (m_message_type != MessageType::SET && m_message_type != MessageType::GET && m_message_type != MessageType::CREATE) {
    throw new std::invalid_argument("Only set and get message have table field\n");
  }
  if (!this->is_valid()) {
    throw new InvalidMessage("Invalid argument value\n");
  }
  return get_arg(0);
}

std::string Message::get_key() const
{
  if (m_message_type != MessageType::SET && m_message_type != MessageType::GET) {
    throw new std::invalid_argument("Only set and get message have key field\n");
  }
  if (!this->is_valid()) {
    throw new InvalidMessage("Invalid argument value\n");
  }
  return get_arg(1);
}

std::string Message::get_value() const
{
  if (m_message_type != MessageType::PUSH && m_message_type != MessageType::DATA) {
    throw new std::invalid_argument("Only push and data message have value field\n");
  }
  if (!this->is_valid()) {
    throw new InvalidMessage("Invalid argument value\n");
  }
  return get_arg(0);
}

std::string Message::get_quoted_text() const
{
  if (m_message_type != MessageType::FAILED && m_message_type != MessageType::ERROR) {
    throw new std::invalid_argument("Only failed and error message have quoted_text field\n");
  }
  if (!this->is_valid()) {
    throw new InvalidMessage("Invalid argument value\n");
  }
  return get_arg(0);
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
    for (int i = 0; i < m_args.size(); i++) {
      if(!Message::is_letter(m_args[i][0])) {
        return false;
      }
      for (int j = 1; j < m_args[i].length(); j++) {
        if(!Message::is_letter(m_args[i][j]) && !Message::is_number(m_args[i][j]) && m_args[i][j] != '_') {
          return false;
        }
      }
    }
  }
  if (m_message_type == MessageType::PUSH || m_message_type == MessageType::DATA) {
    for (int j = 0; j < m_args[0].length(); j++) {
      if (m_args[0][j] == ' ') {
        return false;
      }
    }
  }
  if (m_message_type == MessageType::FAILED || m_message_type == MessageType::ERROR) {
    for (int i = 0; i < m_args[0].size(); i++) {
      if (m_args[0][i] == '"') {
        return false;
      }
    }
  }
  return true;
}

bool Message::is_num_match() const {
  std::map<MessageType, int> map = {
    {MessageType::LOGIN, 1},
    {MessageType::CREATE, 1},
    {MessageType::PUSH, 1},
    {MessageType::POP, 0},
    {MessageType::TOP, 0},
    {MessageType::SET, 2},
    {MessageType::GET, 2},
    {MessageType::ADD, 0},
    {MessageType::MUL, 0},
    {MessageType::SUB, 0},
    {MessageType::DIV, 0},
    {MessageType::BEGIN, 0},
    {MessageType::COMMIT, 0},
    {MessageType::BYE, 0},
    {MessageType::OK, 0},
    {MessageType::FAILED, 1},
    {MessageType::ERROR, 1},
    {MessageType::DATA, 1}
  };
  if (map[m_message_type] != m_args.size()) {
    return false;
  }
  return true;
}
