#include <utility>
#include <sstream>
#include <cassert>
#include <map>
#include "exceptions.h"
#include "message_serialization.h"
#include <vector>
#include <string>

void MessageSerialization::encode( const Message &msg, std::string &encoded_msg )
{
  encoded_msg = "";
  std::map<MessageType, std::string> map = {
    {MessageType::LOGIN, "LOGIN"},
    {MessageType::CREATE, "CREATE"},
    {MessageType::PUSH, "PUSH"},
    {MessageType::POP, "POP"},
    {MessageType::TOP, "TOP"},
    {MessageType::GET, "GET"},
    {MessageType::SET, "SET"},
    {MessageType::ADD, "ADD"},
    {MessageType::SUB, "SUB"},
    {MessageType::MUL, "MUL"},
    {MessageType::DIV, "DIV"},
    {MessageType::BEGIN, "BEGIN"},
    {MessageType::COMMIT, "COMMIT"},
    {MessageType::BYE, "BYE"},
    {MessageType::OK, "OK"},
    {MessageType::FAILED, "FAILED"},
    {MessageType::ERROR, "ERROR"},
    {MessageType::DATA, "DATA"}
  };
  encoded_msg += map[msg.get_message_type()];
  if (msg.get_num_args() == 0) {
    encoded_msg += "\n";
  } else {
    encoded_msg += " ";
  }
  for (int i = 0; i < msg.get_num_args(); i++) {
    encoded_msg += msg.get_arg(i);
    if (i == msg.get_num_args() - 1) {
      encoded_msg += "\n";
    } else {
      encoded_msg += " ";
    }
  }
  if (encoded_msg.length() > Message::MAX_ENCODED_LEN) {
    throw new InvalidMessage("Encoded string too long\n");
  }
}

void MessageSerialization::decode( const std::string &encoded_msg_, Message &msg )
{
  // TODO: implement
}
