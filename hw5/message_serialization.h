#ifndef MESSAGE_SERIALIZATION_H
#define MESSAGE_SERIALIZATION_H

#include "message.h"

namespace MessageSerialization {
  void encode(const Message &msg, std::string &encoded_msg);
  void decode(const std::string &encoded_msg, Message &msg);
  void processNormal(const std::string& encoded_msg_, Message &msg, int expectedArgument);
  void processServerMessage(const std::string& encoded_msg_, Message &msg);
};

#endif // MESSAGE_SERIALIZATION_H
