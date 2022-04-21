#include <sstream>
#include <cctype>
#include <cassert>
#include "csapp.h"
#include "message.h"
#include "connection.h"
#include <iostream>


Connection::Connection()
  : m_fd(-1)
  , m_last_result(SUCCESS) {
}

Connection::Connection(int fd)
  : m_fd(fd)
  , m_last_result(SUCCESS) {
  // TODO: call rio_readinitb to initialize the rio_t object

  rio_readinitb(&m_fdbuf, m_fd);

}

void Connection::connect(const std::string &hostname, int port) {
  // TODO: call open_clientfd to connect to the server
  const char* p = std::to_string(port).c_str();

  m_fd = open_clientfd(hostname.c_str(), p);
  if (m_fd < 0) { throw -3; }

  // TODO: call rio_readinitb to initialize the rio_t object
  
  rio_readinitb(&m_fdbuf, m_fd);

}

void Connection::connect(const char* hostname, const char* port) {
  // TODO: call open_clientfd to connect to the server

  m_fd = open_clientfd(hostname, port);
  if (!is_open()) { throw -3; }

  // TODO: call rio_readinitb to initialize the rio_t object
  
  rio_readinitb(&m_fdbuf, m_fd);

}

Connection::~Connection() {
  // TODO: close the socket if it is open

  close();

}

bool Connection::is_open() const {
  // TODO: return true if the connection is open
  return m_fd >= 0;

}

void Connection::close() {
  // TODO: close the connection if it is open
  if (is_open()) {
    Close(m_fd);
    m_fd = -1;
  }

}

bool Connection::send(const Message &msg) {
  // TODO: send a message
  // return true if successful, false if not
  // make sure that m_last_result is set appropriately
  // std::stringstream m;
  std::string string_message;

  //std::cout << "msg got says: " << msg.tag << ":" << msg.data << std::endl;

  //m << msg.tag << ":" << msg.data << std::endl;
  //m >> string_message;
  
  string_message = msg.tag + ":" + msg.data + "\n";

  // std::cout << "msg post ss: " << msg.tag << ":" << msg.data << std::endl;

  // string_message.append("\n");

  //std::cout << "msg sent says: " << string_message;
  
  rio_writen(m_fd, string_message.c_str(), string_message.length()); // send message to server
  
  // TODO: handle error things
  return true;
}

bool Connection::receive(Message &msg) {
  // TODO: recieve a message, storing its tag and data in msg
  // return true if successful, false if not
  // make sure that m_last_result is set appropriately
  
  char buf[msg.MAX_LEN + 1];
  ssize_t n = rio_readlineb(&m_fdbuf, buf, sizeof(buf));
  
  // std::cout << "buffer recieved: " << buf << std::endl;
  
  char cTag[msg.MAX_LEN + 1];
  char cData[msg.MAX_LEN + 1];

  char* ptr = strchr(buf, ':');
  ptrdiff_t index = ptr - buf;


  memcpy(cTag, buf, (size_t) index);
  cTag[index] = '\0';
  strcpy(cData, ptr + 1);

  //std::stringstream s;
  //std::string ms;

  //s << buf;
  //s >> ms;

  //std::cout << "string recieved: " << ms << std::endl;

  //size_t tag_end = ms.find(":");
  //size_t message_end = ms.find("\n");
  // handle if cannot find colon

  //msg.tag = ms.substr(0, tag_end);
  //msg.data = ms.substr(tag_end + 1, message_end);
  
  std::string cppTag(cTag);
  std::string cppData(cData);

  msg.tag = cppTag;
  msg.data = cppData;

  if (n > 0) {
    return true;
  }
  return false;

  // TODO: handle error things

}
