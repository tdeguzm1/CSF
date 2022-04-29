/*
 * Reciever as a client for a chat server
 * CSF Assignment 5.1
 * Theo DeGuzman
 * tdeguzm1@jhu.edu
 */

#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>
#include "csapp.h"
#include "message.h"
#include "connection.h"
#include "client_util.h"

void printMessage(std::string data);
void receiverLoop(Connection* conn);


// main function to drive the reciever
int main(int argc, char **argv) {
  if (argc != 5) {
    std::cerr << "Usage: ./receiver [server_address] [port] [username] [room]\n";
    return 1;
  }

  std::string server_hostname = argv[1];
  char* hostname_cString = argv[1];
  char* port_cString = argv[2];
  std::string username = argv[3];
  std::string room_name = argv[4];

  // connect to server
  Connection *conn = new Connection();
  conn->connect(hostname_cString, port_cString);

  // request to log in
  Message login_msg = Message();
  conn->send(Message(TAG_RLOGIN, username));
  conn->receive(login_msg);
  
  if (login_msg.tag == TAG_ERR) {
    std::cerr << login_msg.data;
    delete conn;
    return 1;
  }  

  std::cout << "logged on" << std::endl;
  
  // request to join room
  Message join_msg = Message();
  conn->send(Message(TAG_JOIN, room_name));
  conn->receive(join_msg);
  std::cout << "Join message tag: " << join_msg.tag << std::endl;
  std::cout << "Join message data: " << join_msg.data << std::endl;
  

  // NEED TO PUT THIS BACK IN
  // if (join_msg.tag == TAG_ERR) {
  //   std::cerr << join_msg.data;
  //   delete conn;
  //   return 2;
  // }

  std::cout << "joined room (static)" << std::endl;

  // loop waiting to recieve message
  while (true) {
    receiverLoop(conn);
  }

  return 0;
} // 27 lines

/* print delivered message
 * assume that the message is formatted correctly coming from server
 *
 * params:
 *   data - contains unformatted msg.data, which includes the username
 */
void printMessage(std::string data) {
  size_t sender = data.find(":");
  data = data.substr(sender + 1);
  size_t message = data.find(":");
  std::string username = trim(data.substr(0, message));
  std::string msg = trim(data.substr(message + 1));
  std::cout << username << ": " << msg << std::endl;
}

/* print delivered message
 *
 * params:
 *   conn - connection object that talks to the server
 */
void receiverLoop(Connection* conn) {
  Message msg = Message();
  if (conn->receive(msg)) {
    if (msg.tag == TAG_DELIVERY) {
      printMessage(msg.data);
    }
    else if (msg.tag == TAG_ERR) {
      std::cout << msg.data;
    }
  }
}