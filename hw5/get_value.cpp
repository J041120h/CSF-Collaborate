#include "exceptions.h"
#include "message.h"
#include "csapp.h"
#include "message_serialization.h"
#include <iostream>
#include <stdexcept>
#include <sys/types.h>
#include <unistd.h>

int main(int argc, char **argv)
{
  if ( argc != 6 ) {
    std::cerr << "Usage: ./get_value <hostname> <port> <username> <table> <key>\n";
    return 1;
  }

  std::string hostname = argv[1];
  std::string port = argv[2];
  std::string username = argv[3];
  std::string table = argv[4];
  std::string key = argv[5];
  std::vector<Message> messages;
  // TODO: implement
  messages.push_back(Message(MessageType::LOGIN, {username}));
  messages.push_back(Message(MessageType::GET, {table, key}));
  messages.push_back(Message(MessageType::TOP, {}));
  messages.push_back(Message(MessageType::BYE, {}));

  int fd = open_clientfd(hostname.data(), port.data());
  if (fd < 0) {
      std::cerr << "Error: couldn't connect to the server\n";
      close(fd);
      exit(-2);
  }

  for (std::vector<Message>::iterator it = messages.begin(); it != messages.end(); ++it) {
    std::string encoded_message = "";
    try {
      MessageSerialization::encode(*it, encoded_message);
    } catch (std::runtime_error &e) {
      std::cerr << "Error: fail to encode the message\n";
      close(fd);
      exit(-3);
    }
    rio_writen(fd, encoded_message.data(), encoded_message.length());

    //read response
    rio_t rio;
    rio_readinitb(&rio, fd);
    char buf[2048];
    ssize_t size = rio_readlineb(&rio, buf, sizeof(buf));
    if (size > 1024) {
      std::cerr << "Error: The response message is too long\n";
      close(fd);
      exit(-4);
    }

    Message response;
    MessageSerialization::decode(buf,response);
    if (response.get_message_type() == MessageType::OK) {
      continue;
    } else if (response.get_message_type() == MessageType::ERROR) {
      close(fd);
      std::cout << response.get_arg(0);
      exit(-1);
    } else if (response.get_message_type() == MessageType::FAILED) {
      std::cout << response.get_arg(0);
      close(fd);
      exit(-5);
    } else if (response.get_message_type() == MessageType::DATA) {
      std::cout << response.get_value() << "\n";
      continue;
    } else {
      std::cerr << "Error: fail to response";
      close(fd);
      exit(-6);
    }
  }
}
