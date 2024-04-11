#ifndef MESSAGE_SERIALIZATION_H
#define MESSAGE_SERIALIZATION_H

#include "message.h"

namespace MessageSerialization {
  void encode(const Message &msg, std::string &encoded_msg);
  void decode(const std::string &encoded_msg, Message &msg);
  void processSecondWord(const std::string& encoded_msg_, Message &msg, int expectedArgument);
};

#endif // MESSAGE_SERIALIZATION_H
