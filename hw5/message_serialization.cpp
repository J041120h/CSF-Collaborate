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
    processNormal(encoded_msg_, msg, 1);
    msg.set_message_type(MessageType::LOGIN);
  } else if (encoded_msg_.find("CREATE") == 0) {
    processNormal(encoded_msg_, msg, 1);
    msg.set_message_type(MessageType::CREATE);
  } else if (encoded_msg_.find("PUSH") == 0) {
    processNormal(encoded_msg_, msg, 1);
    msg.set_message_type(MessageType::PUSH);
  } else if (encoded_msg_.find("POP") == 0) {
    processNormal(encoded_msg_, msg, 0);
    msg.set_message_type(MessageType::POP);
  } else if (encoded_msg_.find("TOP") == 0) {
    processNormal(encoded_msg_, msg, 0);
    msg.set_message_type(MessageType::TOP);
  } else if (encoded_msg_.find("SET") == 0) {
    processNormal(encoded_msg_, msg, 2);
    msg.set_message_type(MessageType::SET);
  } else if (encoded_msg_.find("GET") == 0) {
    processNormal(encoded_msg_, msg, 2);
    msg.set_message_type(MessageType::GET);
  } else if (encoded_msg_.find("ADD") == 0) {
    processNormal(encoded_msg_, msg, 0);
    msg.set_message_type(MessageType::ADD);
  } else if (encoded_msg_.find("MUL") == 0) {
    processNormal(encoded_msg_, msg, 0);
    msg.set_message_type(MessageType::MUL);
  } else if (encoded_msg_.find("SUB") == 0) {
    processNormal(encoded_msg_, msg, 0);
    msg.set_message_type(MessageType::SUB);
  } else if (encoded_msg_.find("DIV") == 0) {
    processNormal(encoded_msg_, msg, 0);
    msg.set_message_type(MessageType::DIV);
  } else if (encoded_msg_.find("BEGIN") == 0) {
    processNormal(encoded_msg_, msg, 0);
    msg.set_message_type(MessageType::BEGIN);
  } else if (encoded_msg_.find("COMMIT") == 0) {
    processNormal(encoded_msg_, msg, 0);
    msg.set_message_type(MessageType::COMMIT);
  } else if (encoded_msg_.find("BYE") == 0) {
    processNormal(encoded_msg_, msg, 0);
    msg.set_message_type(MessageType::BYE);
  } else if (encoded_msg_.find("OK") == 0) {
    processNormal(encoded_msg_, msg, 1);
    msg.set_message_type(MessageType::LOGIN);
  } else if (encoded_msg_.find("FAILED") == 0) {
    processServerMessage(encoded_msg_, msg);
    msg.set_message_type(MessageType::FAILED);
  } else if (encoded_msg_.find("ERROR") == 0) {
    processServerMessage(encoded_msg_, msg);
    msg.set_message_type(MessageType::ERROR);
  } else if (encoded_msg_.find("DATA") == 0) {
    processNormal(encoded_msg_, msg, 1);
    msg.set_message_type(MessageType::DATA);
  }
}

void processNormal(const std::string& encoded_msg_, Message &msg, int expectedArgument) {
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

void processServerMessage(const std::string& encoded_msg_, Message &msg) {
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
      words.push_back(quotedWord);
      } else {
          words.push_back(token);
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
