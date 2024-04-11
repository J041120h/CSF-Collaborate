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
  for (int i = 0; i < msg.get_num_args(); i++) {
    encoded_msg += msg.get_arg(i);
    if (i == msg.get_num_args() - 1) {
      encoded_msg += "\0";
    } else {
      encoded_msg += " ";
    }
  }
}

void MessageSerialization::decode( const std::string &encoded_msg_, Message &msg )
{
  // TODO: implement
}
