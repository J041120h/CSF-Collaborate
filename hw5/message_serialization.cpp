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
  std::string word;
  // TODO: implement
  if (encoded_msg_.find("LOGIN") == 0) {
    processSecondWord(encoded_msg_, msg, 1);
    msg.set_message_type(MessageType::LOGIN);
  } else if (encoded_msg_.find("CREATE") == 0) {

  } else if (encoded_msg_.find("PUSH") == 0) {

  } else if (encoded_msg_.find("POP") == 0) {

  } else if (encoded_msg_.find("TOP") == 0) {

  } else if (encoded_msg_.find("SET") == 0) {

  } else if (encoded_msg_.find("GET") == 0) {

  } else if (encoded_msg_.find("ADD") == 0) {

  } else if (encoded_msg_.find("MUL") == 0) {

  } else if (encoded_msg_.find("SUB") == 0) {

  } else if (encoded_msg_.find("DIV") == 0) {

  } else if (encoded_msg_.find("BEGIN") == 0) {

  } else if (encoded_msg_.find("COMMIT") == 0) {

  } else if (encoded_msg_.find("BYE") == 0) {

  } else if (encoded_msg_.find("OK") == 0) {

  }
}

void processSecondWord(const std::string& encoded_msg_, Message &msg, int expectedArgument) {
    std::istringstream iss(encoded_msg_);
    std::vector<std::string> words;
    std::string word;

    // Split the input string by white spaces
    iss >> word; // get rid of the first argument
    while (iss >> word) {
        words.push_back(word);
    }

    // Check if there are more than two words
    if (words.size() > expectedArgument) {
        throw InvalidMessage("Incorrect Message with more than expected arguments");
    }
    // Handle the case where there is only one word or exactly two words
    if (words.size() == expectedArgument) {
        for (int i = 0; i < expectedArgument; i++){
          msg.push_arg(words[i]);
        }
    } else {
        // Handle the case for one word or no words, depending on requirements
        throw InvalidMessage("Error: Less than two words found.");
    }
}
