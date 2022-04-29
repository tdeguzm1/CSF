/*
 * Connection object between server and clients for a chat server
 * CSF Assignment 5.1
 * Theo DeGuzman
 * tdeguzm1@jhu.edu
 */

#include <sstream>
#include <cctype>
#include <cassert>
#include "csapp.h"
#include "message.h"
#include "connection.h"
#include <iostream>
#include <string>

/**
 *  Creates a connection- default constructor
 */
Connection::Connection()
  : m_fd(-1)
  , m_last_result(SUCCESS) {
}

/**
 *  Creates a connection
 *  
 *  Param:
*     fd - a given file descriptor
 */
Connection::Connection(int fd)
  : m_fd(fd)
  , m_last_result(SUCCESS) {
  rio_readinitb(&m_fdbuf, m_fd);
}


/* 
 * Open a client with the server. Outdated version of the function
 *
 * Params:
 *  hostname - name of server
 *  port (int) - port
 */
void Connection::connect(const std::string &hostname, int port) {
  
  // call open_clientfd to connect to the server
  const char* p = std::to_string(port).c_str();
  m_fd = open_clientfd(hostname.c_str(), p);
  if (m_fd < 0) { throw -3; }

  // call rio_readinitb to initialize the rio_t object
  rio_readinitb(&m_fdbuf, m_fd);

}


/* 
 * Open a client with the server. Uses c strings for convenience
 *
 * Params:
 *  hostname - name of server
 *  port - port
 */
void Connection::connect(const char* hostname, const char* port) {
  // call open_clientfd to connect to the server
  
  if (!is_open()){
    m_fd = open_clientfd(hostname, port);
    if (!is_open()) { throw -3; }
  }

  // call rio_readinitb to initialize the rio_t object
  rio_readinitb(&m_fdbuf, m_fd);

}


/* destructor for connection  */
Connection::~Connection() {
  close();
}

/* return true if the connection is open */
bool Connection::is_open() const {
  return m_fd >= 0;

}

/* close the connection if it is open */
void Connection::close() {
  if (is_open()) {
    Close(m_fd);
    m_fd = -1;
  }
}

/* send a message
 * make sure that m_last_result is set appropriately
 *
 * param:
 *  msg - a reference to the message to be sent
 *
 * return: 
 *  true if successful, false if not
 */
bool Connection::send(const Message &msg) {
  std::string string_message;
  string_message = msg.tag + ":" + msg.data + "\n";
  
  // message does not have valid tag
  if (!msg.hasValidTag()) {
    m_last_result = INVALID_MSG;
    return false;
  }
  
  std::cout << "buffer sent: " << string_message.c_str() << std::endl;
  // send message to server
  size_t bytes_w = rio_writen(m_fd, string_message.c_str(), string_message.length());
  
  // write was interrupted
  if (bytes_w < string_message.length()) {
    m_last_result = EOF_OR_ERROR;
    return false;
  }
  return true;
}

/* recieve a message, storing its tag and data in msg
 * make sure that m_last_result is set appropriately
 *
 * params:
 *   msg - reference message to store recieved value into
 *
 * return:
 *   true if successful, false if not
 */
bool Connection::receive(Message &msg) {
  
  char buf[msg.MAX_LEN + 1];
  ssize_t n = rio_readlineb(&m_fdbuf, buf, sizeof(buf));

  if (n < 1) {
    msg.tag = TAG_ERR;
    m_last_result = EOF_OR_ERROR;
    return false;
  }
  
  std::cout << "rbuffer len: " << n << std::endl;
  std::cout << "buffer recieved: " << buf << std::endl;
  
  return convertToMsg(msg, buf);

}


/* Take a character buffer and turn it into a message
 *
 * Params:
 *   msg - message reference to be written into
 *   buf - c-style string buffer of chars
 *
 * Return:
 *   true if sucessful conversion, false if not
 */
bool Connection::convertToMsg(Message &msg, char* buf) {
  char cTag[msg.MAX_LEN + 1];
  char cData[msg.MAX_LEN + 1];

  char* ptr = strchr(buf, ':');
  

  // invalid if no colon in the buffer
  if (ptr == NULL) {
    msg.tag = TAG_ERR;
    m_last_result = INVALID_MSG;
    return false;
  }

  // put message and tag into char arrays
  ptrdiff_t index = ptr - buf;
  memcpy(cTag, buf, (size_t) index);
  cTag[index] = '\0';
  strcpy(cData, ptr + 1);

  msg.tag = cTag;
  msg.data = cData;
  
  if (!msg.hasValidTag()) {
    m_last_result = INVALID_MSG;
    return false;
  } else if (msg.tag == TAG_ERR) {
    m_last_result = EOF_OR_ERROR;
  } else {
    m_last_result = SUCCESS;
  }
  return true;

} // 21 lines
