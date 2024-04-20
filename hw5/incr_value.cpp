#include <iostream>
#include "message.h"
#include "csapp.h"
#include "message_serialization.h"
#include "exceptions.h"

int main(int argc, char **argv) {
  if ( argc != 6 && (argc != 7 || std::string(argv[1]) != "-t") ) {
    std::cerr << "Usage: ./incr_value [-t] <hostname> <port> <username> <table> <key>\n";
    std::cerr << "Options:\n";
    std::cerr << "  -t      execute the increment as a transaction\n";
    return 1;
  }

  int count = 1;

  bool use_transaction = false;
  if ( argc == 7 ) {
    use_transaction = true;
    count = 2;
  }

  std::string hostname = argv[count++];
  std::string port = argv[count++];
  std::string username = argv[count++];
  std::string table = argv[count++];
  std::string key = argv[count++];

  // TODO: implement
  Message beginMessage = Message(MessageType::BEGIN);
  Message logMessage = Message(MessageType::LOGIN, {username});
  Message getMessage = Message(MessageType::GET, {table, key});
  Message pushMessage = Message(MessageType::PUSH, {"1"});
  Message setMessage = Message(MessageType::SET, {table, key});
  Message addMessage = Message(MessageType::ADD);
  Message commitMessage = Message(MessageType::COMMIT);
  Message byeMessage = Message(MessageType::BYE);
  std::vector<Message> messageList;
  if (use_transaction) {
    messageList = {logMessage, beginMessage, getMessage, pushMessage, addMessage, setMessage, commitMessage, byeMessage};
  } else {
    messageList = {logMessage, getMessage, pushMessage, addMessage, setMessage, byeMessage};
  }
  
  int fd = open_clientfd(hostname.data(), port.data());
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
    Message responseMessage;
    ssize_t n = rio_readlineb(&rio, buf, sizeof(buf));
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
      exit(-1);
    } else if (responseMessage.get_message_type() == MessageType::ERROR) {
      std::cerr << "Error:" << responseMessage.get_arg(0) << std::endl;
      exit(-1);
    }
  }
  return 0;
}
