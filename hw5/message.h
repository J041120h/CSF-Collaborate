#ifndef MESSAGE_H
#define MESSAGE_H

#include <vector>
#include <string>

enum class MessageType {
  // Used only for uninitialized Message objects
  NONE,

  // Requests
  LOGIN,
  CREATE,
  PUSH,
  POP,
  TOP,
  SET,
  GET,
  ADD,
  SUB,
  MUL,
  DIV,
  BEGIN,
  COMMIT,
  BYE,

  // Responses
  OK,
  FAILED,
  ERROR,
  DATA,
};

class Message {
private:
  MessageType m_message_type;
  std::vector<std::string> m_args;

  static bool is_letter(char c) { return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'z');}
  static bool is_number(char c) { return c >= '0' and c <= '9';} //helper functions for deciding type of char
  bool is_num_match() const;

public:
  // Maximum encoded message length (including terminator newline character)
  static const unsigned MAX_ENCODED_LEN = 1024;

  Message();
  Message( MessageType message_type, std::initializer_list<std::string> args = std::initializer_list<std::string>() );
  Message( const Message &other );
  ~Message();

  Message &operator=( const Message &rhs );

  MessageType get_message_type() const;
  void set_message_type( MessageType message_type );

  std::string get_username() const;
  std::string get_table() const;
  std::string get_key() const;
  std::string get_value() const;
  std::string get_quoted_text() const;

  void push_arg( const std::string &arg );

  bool is_valid() const;

  unsigned get_num_args() const { return m_args.size(); }
  std::string get_arg( unsigned i ) const { return m_args.at( i ); }

  
};

#endif // MESSAGE_H
