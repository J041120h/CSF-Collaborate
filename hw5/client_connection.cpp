#include <iostream>
#include <cassert>
#include <string.h>
#include <sstream>
#include <algorithm>
#include "csapp.h"
#include "message.h"
#include "message_serialization.h"
#include "server.h"
#include "exceptions.h"
#include "client_connection.h"
#include "value_stack.h"

ClientConnection::ClientConnection( Server *server, int client_fd )
  : m_server( server )
  , m_client_fd( client_fd )
{
  rio_readinitb( &m_fdbuf, m_client_fd );
}

ClientConnection::~ClientConnection()
{
  close(m_client_fd);
}

void ClientConnection::chat_with_client()
{
  bool ongoing = true;
  bool transaction = false;
  ValueStack operand_stack;
  std::vector<std::string> modified_tables;
  std::vector<Table*> new_tables;
  while (ongoing) {
    Message responseMessage;
    std::string encoded_message = "";
    try {
      Message message;
      char buf[2048];
      rio_readlineb(&m_fdbuf, buf, sizeof(buf));
      MessageSerialization::decode(buf, message); //potentially throw invalidMessage when input message is invalid
      if (message.get_message_type() == MessageType::BEGIN) {
        if (transaction) {
          throw FailedTransaction("Cannot call begin again in transaction mode\n");
        }
        transaction = true;
        responseMessage = Message(MessageType::OK, {});
      } else if (message.get_message_type() == MessageType::LOGIN) {
        responseMessage = Message(MessageType::OK, {});
      } else if (message.get_message_type() == MessageType::CREATE) {
        std::string table = message.get_table();
        if (m_server->getTable(table) != nullptr) {
          throw InvalidMessage("Try to create a existing table");
        }
        if (transaction) {
          for (std::vector<Table*>::iterator it = new_tables.begin(); it != new_tables.end(); it++) {
            if ((*it)->get_name() == table) {
              throw InvalidMessage("Try to create a existing table");
            }
          }
        }
        Table tableObject(message.get_table());
        if (transaction) {
          new_tables.push_back(&tableObject);
          if(!tableObject.trylock()) {
            throw FailedTransaction("Transaction Failed\n");
          }
        } else {
          m_server->addTable(tableObject);
        }
        responseMessage = Message(MessageType::OK, {});
      } else if (message.get_message_type() == MessageType::PUSH) {
        operand_stack.push(message.get_value());
        responseMessage = Message(MessageType::OK, {});
      } else if (message.get_message_type() == MessageType::POP) {
        operand_stack.pop();
        responseMessage = Message(MessageType::OK, {});
      } else if (message.get_message_type() == MessageType::TOP) {
        std::string value = operand_stack.get_top();
        responseMessage = Message(MessageType::DATA, {value});
      } else if (message.get_message_type() == MessageType::SET) {
        std::string table = message.get_table();
        std::string key = message.get_key();
        std::string value = operand_stack.get_top();
        operand_stack.pop();
        if (transaction) {
          bool temp = false; //check if setting value in newly created table in transaction
          for (std::vector<Table*>::iterator it = new_tables.begin(); it != new_tables.end(); it++) {
            if ((*it)->get_name() == table) {
              (*it)->set(key, value);
              temp = true;
            }
          }
          if (!temp) {
            Table* tableptr = m_server->getTable(table);
            if (tableptr == nullptr && !temp) {
              throw InvalidMessage("Try to modify an unexisting table\n");
            }
            if (std::find(modified_tables.begin(), modified_tables.end(), table) == modified_tables.end()) {
              if (!tableptr->trylock()) {
                throw FailedTransaction("Table cannot get locked\n");
              }
              modified_tables.push_back(table);
            }
            tableptr->set(key, value); 
          }      
        } else {
          Table* tableptr = m_server->getTable(table);
          if (tableptr == nullptr) {
            throw InvalidMessage("Try to modify an unexisting table\n");
          }
          tableptr->lock();
          tableptr->set(key, value);
          tableptr->unlock();
        }     
      } else if (message.get_message_type() == MessageType::GET) {
        std::string table = message.get_table();
        std::string key = message.get_key();
        if (transaction) {
          bool temp = false; //check if setting value in newly created table in transaction
          for (std::vector<Table*>::iterator it = new_tables.begin(); it != new_tables.end(); it++) {
            if ((*it)->get_name() == table) {
              if (!(*it)->has_key(key)) {
                throw InvalidMessage("Input key unexist in table\n");
              }
              operand_stack.push((*it)->get(key));
              temp = true;
            }
          }
          if (!temp) {
            Table* tableptr = m_server->getTable(table);
            if (tableptr == nullptr && !temp) {
              throw InvalidMessage("Try to access an unexisting table\n");
            }
            if (std::find(modified_tables.begin(), modified_tables.end(), table) == modified_tables.end()) {
              if (!tableptr->trylock()) {
                throw FailedTransaction("Table cannot get locked\n");
              }
              modified_tables.push_back(table);
            }
            if (!tableptr->has_key(key)) {
              throw InvalidMessage("Input key unexist in table\n");
            }
            operand_stack.push(tableptr->get(key)); 
          }      
        } else {
          Table* tableptr = m_server->getTable(table);
          if (tableptr == nullptr) {
            throw InvalidMessage("Try to access an unexisting table\n");
          }
          tableptr->lock();
          if (!tableptr->has_key(key)) {
            tableptr->unlock();
            throw InvalidMessage("Input key unexist in table\n");
          }
          operand_stack.push(tableptr->get(key));
          tableptr->unlock();
        }
      } else if (message.get_message_type() == MessageType::ADD) {
        std::string value1 = operand_stack.get_top();
        operand_stack.pop();
        std::string value2 = operand_stack.get_top();
        operand_stack.pop();
        std::stringstream temp;
        temp << value1 << value2;
        int first = 0;
        int second = 0;
        temp >> first >> second;
        int sum = first + second;
        operand_stack.push(std::to_string(sum));
        responseMessage = Message(MessageType::OK, {});
      } else if (message.get_message_type() == MessageType::MUL) {
        std::string value1 = operand_stack.get_top();
        operand_stack.pop();
        std::string value2 = operand_stack.get_top();
        operand_stack.pop();
        std::stringstream temp;
        temp << value1 << value2;
        int first = 0;
        int second = 0;
        temp >> first >> second;
        int mul = first * second;
        operand_stack.push(std::to_string(mul));
        responseMessage = Message(MessageType::OK, {});
      } else if (message.get_message_type() == MessageType::SUB) {
        std::string value1 = operand_stack.get_top();
        operand_stack.pop();
        std::string value2 = operand_stack.get_top();
        operand_stack.pop();
        std::stringstream temp;
        temp << value1 << value2;
        int first = 0;
        int second = 0;
        temp >> first >> second;
        int dif = second - first;
        operand_stack.push(std::to_string(dif));
        responseMessage = Message(MessageType::OK, {});
      } else if (message.get_message_type() == MessageType::DIV) {
        std::string value1 = operand_stack.get_top();
        operand_stack.pop();
        std::string value2 = operand_stack.get_top();
        operand_stack.pop();
        std::stringstream temp;
        temp << value1 << value2;
        int first = 0;
        int second = 0;
        temp >> first >> second;
        int div = second / first;
        operand_stack.push(std::to_string(div));
        responseMessage = Message(MessageType::OK, {});
      } else if (message.get_message_type() == MessageType::COMMIT) {
        if (!transaction) {
          throw InvalidMessage("Cannot commit when no transaction begin\n");
        }
        for (std::vector<std::string>::iterator it = modified_tables.begin(); it != modified_tables.end(); it++) {
          Table* table = m_server->getTable(*it);
          table->commit_changes();
          table->unlock();
        }
        for (std::vector<Table*>::iterator it = new_tables.begin(); it != new_tables.end(); it++) {
          (*it)->commit_changes();
          (*it)->unlock();
          m_server->addTable(**it);
        }
      } else if (message.get_message_type() == MessageType::BYE) {
        responseMessage = Message(MessageType::OK, {});
        ongoing = false;
      } else {
        throw InvalidMessage("Response message is mistakenly sent\n"); //input message take form of response message
      }
    } catch (InvalidMessage &ex) {
      responseMessage = Message(MessageType::ERROR, {ex.what()});
      if (transaction) {
        for (std::vector<std::string>::iterator it = modified_tables.begin(); it != modified_tables.end(); it++) {
          Table * tableptr = m_server->getTable(*it);
          tableptr->rollback_changes();
          tableptr->unlock();
        }
      }
      ongoing = false;
    } catch (OperationException &ex) {
      responseMessage = Message(MessageType::FAILED, {ex.what()});
      if (transaction) {
        for (std::vector<std::string>::iterator it = modified_tables.begin(); it != modified_tables.end(); it++) {
          Table * tableptr = m_server->getTable(*it);
          tableptr->rollback_changes();
          tableptr->unlock();
        }
        transaction = false;
      }
    } catch (FailedTransaction &ex) {
      responseMessage = Message(MessageType::FAILED, {ex.what()});
      if (transaction) {
        for (std::vector<std::string>::iterator it = modified_tables.begin(); it != modified_tables.end(); it++) {
          Table * tableptr = m_server->getTable(*it);
          tableptr->rollback_changes();
          tableptr->unlock();
        }
        transaction = false;
      }
    }
    MessageSerialization::encode(responseMessage, encoded_message);
    rio_writen(m_client_fd, encoded_message.data(), encoded_message.length());
  }
}

// TODO: additional member functions
