#include <iostream>
#include "message.h"
#include "csapp.h"
#include "vector"
#include "message_serialization.h"
#include "exceptions.h"

int main(int argc, char **argv)
{
  if (argc != 7) {
    std::cerr << "Usage: ./set_value <hostname> <port> <username> <table> <key> <value>\n";
    return 1;
  }

  std::string hostname = argv[1];
  std::string port = argv[2];
  std::string username = argv[3];
  std::string table = argv[4];
  std::string key = argv[5];
  std::string value = argv[6];

  // TODO: implement
  Message logMessage = Message(MessageType::LOGIN, {username});
  Message pushMessage = Message(MessageType::PUSH, {value});
  Message setMessage = Message(MessageType::SET, {table, key});
  Message byeMessage = Message(MessageType::BYE);
  std::vector<Message> messageList = {logMessage, pushMessage, setMessage, byeMessage};
  int fd = open_clientfd(argv[1], argv[2]);
  if (fd < 0) {
    std::cerr << "Error: cannot connect to server" << std::endl;
    return -1;
  }
  for (std::vector<Message>::const_iterator it = messageList.begin(); it != messageList.end(); it++) {
    if (!it->is_valid()) {
      std::cerr << "Error: attempt to send invalid message" << std::endl;
    }
    std::string inputMessage = "";
    try {
      MessageSerialization::encode(*it, inputMessage);
    } catch (InvalidMessage &ex) {
      std::cerr << "Error: cannot encode message" << std::endl;
    }
    rio_writen(fd, inputMessage.data(), inputMessage.length());
    rio_t rio;
    char buf[2048];
    rio_readinitb(&rio, fd);
    ssize_t n = rio_readlineb(&rio, buf, sizeof(buf));
    Message responseMessage;
    try {
      const std::string message(buf);
      MessageSerialization::decode(message, responseMessage);
    } catch (InvalidMessage &ex) {
      std::cerr << "Error: cannot decode message" << std::endl;
      exit(-1);
    }
    if (!responseMessage.is_valid()) {
      std::cerr << "Error: invalid reply message" << std::endl;
      exit(-1);
    }
    if (responseMessage.get_message_type() == MessageType::FAILED) {
      std::cerr << "Error: " << responseMessage.get_arg(0) << std::endl;
      close(fd);
      exit(-1);
    } else if (responseMessage.get_message_type() == MessageType::ERROR) {
      std::cerr << "Error:" << responseMessage.get_arg(0) << std::endl;
      close(fd);
      exit(-1);
    } else if (responseMessage.get_message_type() == MessageType::DATA) {
      std::cout << responseMessage.get_arg(0) << std::endl;
    }
  }
  close(fd);
  return 0;
}
