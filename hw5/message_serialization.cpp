#include <utility>
#include <sstream>
#include <cassert>
#include <map>
#include <vector>
#include "exceptions.h"
#include "message.h"
#include "message_serialization.h"
#include <vector>
#include <string>
#include <iostream>

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
  for (int i = 0; i < (int)msg.get_num_args(); i++) {
    if (msg.get_message_type() == MessageType::FAILED || msg.get_message_type() == MessageType::ERROR) {
      encoded_msg += "\"";
    }
    encoded_msg += msg.get_arg(i);
    if (msg.get_message_type() == MessageType::FAILED || msg.get_message_type() == MessageType::ERROR) {
      encoded_msg += "\"";
    }
    if (i == (int)msg.get_num_args() - 1) {
      encoded_msg += "\n";
    } else {
      encoded_msg += " ";
    }
  }
  if (encoded_msg.length() > Message::MAX_ENCODED_LEN) {
    throw InvalidMessage("Encoded string too long\n");
  }
}

bool MessageSerialization::checkWhiteSpace(const std::string &encoded_msg_, int index) {
  for (int i = 0; i < index; i++) {
    if (encoded_msg_[i] != ' ') {
      return false;
    }
  }
  return true;
}

void MessageSerialization::decode( const std::string &encoded_msg_, Message &msg )
{
  if (encoded_msg_.length() > Message::MAX_ENCODED_LEN || encoded_msg_[encoded_msg_.length() - 1] != '\n') {
    throw InvalidMessage("Invalid encoded message\n");
  }
  Message empty;
  msg = empty;
  std::string word;
  size_t index = 0;
  // TODO: implement
  if ((index = encoded_msg_.find("LOGIN")) != std::string::npos) {
    if (!checkWhiteSpace(encoded_msg_, index)) {
      throw InvalidMessage("Encoded string fail to start with appropriate word\n");
    }
    MessageSerialization::processNormal(encoded_msg_, msg, 1);
    msg.set_message_type(MessageType::LOGIN);
  } else if ((index = encoded_msg_.find("CREATE")) != std::string::npos) {
    if (!checkWhiteSpace(encoded_msg_, index)) {
      throw InvalidMessage("Encoded string fail to start with appropriate word\n");
    }
    MessageSerialization::processNormal(encoded_msg_, msg, 1);
    msg.set_message_type(MessageType::CREATE);
  } else if ((index = encoded_msg_.find("PUSH")) != std::string::npos) {
    if (!checkWhiteSpace(encoded_msg_, index)) {
      throw InvalidMessage("Encoded string fail to start with appropriate word\n");
    }
    MessageSerialization::processNormal(encoded_msg_, msg, 1);
    msg.set_message_type(MessageType::PUSH);
  } else if ((index = encoded_msg_.find("POP")) != std::string::npos) {
    if (!checkWhiteSpace(encoded_msg_, index)) {
      throw InvalidMessage("Encoded string fail to start with appropriate word\n");
    }
    MessageSerialization::processNormal(encoded_msg_, msg, 0);
    msg.set_message_type(MessageType::POP);
  } else if ((index = encoded_msg_.find("TOP")) != std::string::npos) {
    if (!checkWhiteSpace(encoded_msg_, index)) {
      throw InvalidMessage("Encoded string fail to start with appropriate word\n");
    }
    MessageSerialization::processNormal(encoded_msg_, msg, 0);
    msg.set_message_type(MessageType::TOP);
  } else if ((index = encoded_msg_.find("SET")) != std::string::npos) {
    if (!checkWhiteSpace(encoded_msg_, index)) {
      throw InvalidMessage("Encoded string fail to start with appropriate word\n");
    }
    MessageSerialization::processNormal(encoded_msg_, msg, 2);
    msg.set_message_type(MessageType::SET);
  } else if ((index = encoded_msg_.find("GET")) != std::string::npos) {
    if (!checkWhiteSpace(encoded_msg_, index)) {
      throw InvalidMessage("Encoded string fail to start with appropriate word\n");
    }
    MessageSerialization::processNormal(encoded_msg_, msg, 2);
    msg.set_message_type(MessageType::GET);
  } else if ((index = encoded_msg_.find("ADD")) != std::string::npos) {
    if (!checkWhiteSpace(encoded_msg_, index)) {
      throw InvalidMessage("Encoded string fail to start with appropriate word\n");
    }
    MessageSerialization::processNormal(encoded_msg_, msg, 0);
    msg.set_message_type(MessageType::ADD);
  } else if ((index = encoded_msg_.find("MUL")) != std::string::npos) {
    if (!checkWhiteSpace(encoded_msg_, index)) {
      throw InvalidMessage("Encoded string fail to start with appropriate word\n");
    }
    MessageSerialization::processNormal(encoded_msg_, msg, 0);
    msg.set_message_type(MessageType::MUL);
  } else if ((index = encoded_msg_.find("SUB")) != std::string::npos) {
    if (!checkWhiteSpace(encoded_msg_, index)) {
      throw InvalidMessage("Encoded string fail to start with appropriate word\n");
    }
    MessageSerialization::processNormal(encoded_msg_, msg, 0);
    msg.set_message_type(MessageType::SUB);
  } else if ((index = encoded_msg_.find("DIV")) != std::string::npos) {
    if (!checkWhiteSpace(encoded_msg_, index)) {
      throw InvalidMessage("Encoded string fail to start with appropriate word\n");
    }
    MessageSerialization::processNormal(encoded_msg_, msg, 0);
    msg.set_message_type(MessageType::DIV);
  } else if ((index = encoded_msg_.find("BEGIN")) != std::string::npos) {
    if (!checkWhiteSpace(encoded_msg_, index)) {
      throw InvalidMessage("Encoded string fail to start with appropriate word\n");
    }
    MessageSerialization::processNormal(encoded_msg_, msg, 0);
    msg.set_message_type(MessageType::BEGIN);
  } else if ((index = encoded_msg_.find("COMMIT")) != std::string::npos) {
    if (!checkWhiteSpace(encoded_msg_, index)) {
      throw InvalidMessage("Encoded string fail to start with appropriate word\n");
    }
    MessageSerialization::processNormal(encoded_msg_, msg, 0);
    msg.set_message_type(MessageType::COMMIT);
  } else if ((index = encoded_msg_.find("BYE")) != std::string::npos) {
    if (!checkWhiteSpace(encoded_msg_, index)) {
      throw InvalidMessage("Encoded string fail to start with appropriate word\n");
    }
    MessageSerialization::processNormal(encoded_msg_, msg, 0);
    msg.set_message_type(MessageType::BYE);
  } else if ((index = encoded_msg_.find("OK")) != std::string::npos) {
    if (!checkWhiteSpace(encoded_msg_, index)) {
      throw InvalidMessage("Encoded string fail to start with appropriate word\n");
    }
    MessageSerialization::processNormal(encoded_msg_, msg, 0);
    msg.set_message_type(MessageType::OK);
  } else if ((index = encoded_msg_.find("FAILED")) != std::string::npos) {
    if (!checkWhiteSpace(encoded_msg_, index)) {
      throw InvalidMessage("Encoded string fail to start with appropriate word\n");
    }
    MessageSerialization::processServerMessage(encoded_msg_, msg);
    msg.set_message_type(MessageType::FAILED);
  } else if ((index = encoded_msg_.find("ERROR")) != std::string::npos) {
    if (!checkWhiteSpace(encoded_msg_, index)) {
      throw InvalidMessage("Encoded string fail to start with appropriate word\n");
    }
    MessageSerialization::processServerMessage(encoded_msg_, msg);
    msg.set_message_type(MessageType::ERROR);
  } else if ((index = encoded_msg_.find("DATA")) != std::string::npos) {
    if (!checkWhiteSpace(encoded_msg_, index)) {
      throw InvalidMessage("Encoded string fail to start with appropriate word\n");
    }
    MessageSerialization::processNormal(encoded_msg_, msg, 1);
    msg.set_message_type(MessageType::DATA);
  }
}

void MessageSerialization::processNormal(const std::string& encoded_msg_, Message &msg, int expectedArgument) {
    std::istringstream iss(encoded_msg_);
    std::vector<std::string> words;
    std::string word;

    // Split the input string by white spaces
    iss >> word; // get rid of the first argument
    while (iss >> word) {
        words.push_back(word);
    }

    // Check if there are more than two words
    if ((int)words.size() > expectedArgument) {
        throw InvalidMessage("Incorrect Message with more than expected arguments");
    }
    // Handle the case where there is only one word or exactly two words
    if ((int)words.size() == expectedArgument) {
        for (int i = 0; i < expectedArgument; i++){
          msg.push_arg(words[i]);
        }
    } else {
        // Handle the case for one word or no words, depending on requirements
        throw InvalidMessage("Error: Less than two words found.");
    }
}

void MessageSerialization::processServerMessage(const std::string& encoded_msg_, Message &msg) {
    std::istringstream iss(encoded_msg_);
    std::string token;
    std::vector<std::string> words;

    // Read and discard the first word
    if (!(iss >> token)) {
        throw InvalidMessage("Error: Message is empty");
    }

    if (!(iss >> token)) {
        throw InvalidMessage("Error: Message does not contain enough words.");
    }

    // Read the rest of the words considering quotes
    if (token.front() == '"' && token.back() != '"') { // Start of a quoted string
      std::string quotedWord = token;
      while (iss >> token) {
          quotedWord += " " + token;
          if (token.back() == '"') {
              break;
          }
      }
      words.push_back(quotedWord.substr(1, quotedWord.length() - 2));
      } else {
          words.push_back(token.substr(1, token.length() - 2));
      }

    // Check number of words against expected arguments
    if (iss >> token) {
        throw InvalidMessage("Error: More than expected arguments provided.");
    }

    // If number of words is as expected, add them to the message
    for (const auto& arg : words) {
        msg.push_arg(arg);
    }
}
