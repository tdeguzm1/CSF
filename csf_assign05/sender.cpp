/*
 * Sender as a client for a chat server
 * CSF Assignment 5.1
 * Theo DeGuzman
 * tdeguzm1@jhu.edu
 */

#include <iostream>
#include <string>
#include <sstream>
#include <stdexcept>
#include "csapp.h"
#include "message.h"
#include "connection.h"
#include "client_util.h"

std::string getRoomName(std::string input);
bool nameValid(std::string name);
void handleInput(std::string input, Connection* conn);
void joinRoom(std::string input, Connection* conn);
void leaveRoom(std::string input, Connection* conn);
void sendMessage(std::string input, Connection* conn);


// main function to drive the sender
int main(int argc, char **argv) {
  if (argc != 4) {
    std::cerr << "Usage: ./sender [server_address] [port] [username]\n";
    return 1;
  }

  std::string server_hostname(argv[1]);
  int server_port = std::stoi(argv[2]);
  std::string username(argv[3]); 
  
  // connect to server
  Connection *conn = new Connection();
  conn->connect(server_hostname, server_port);

  
  // login to server
  Message login_msg = Message();
  conn->send(Message(TAG_SLOGIN, username));
  conn->receive(login_msg);
  
  if (login_msg.tag == TAG_ERR) {
    std::cerr << login_msg.data;
    delete conn;
    return -1;
  }  

  std::string input;
  do {
    getline(std::cin, input);
    input = trim(input);

    handleInput(input, conn);
  } while (input != "/quit");
  

  // send quit signal
  Message quit_msg = Message();
  conn->send(Message(TAG_QUIT, "bye"));
  conn->receive(quit_msg);   

  // For error when quitting
  if (quit_msg.tag != TAG_OK) {
    std::cerr << quit_msg.data;
    delete conn;
    return 4;
  }
  
  delete conn;
  return 0;
} // 30 lines


/* gets a room name from input by cutting off a constant number of characters
 *
 * Params:
 *   input - the string to cut
 *
 * Return:
 *   the trimmed string
 */
std::string getRoomName(std::string input) {
  return input.substr(6); // cuts off first 5 indicies "/join "
}


/* does input handling for main loop
 * 
 * Params:
 *  input - user input
 *  conn - connection object that interacts with server
 */
void handleInput(std::string input, Connection* conn) {
  if (input.find("/join") != std::string::npos) {
    joinRoom(input, conn);
  } else if (input == "/leave") {
    leaveRoom(input, conn);
  } else if (input != "/quit") {
    sendMessage(input, conn);
  }
}


/* carries out tasks to join a room
 * 
 * Params:
 *  input - user input
 *  conn - connection object that interacts with server
 */
void joinRoom(std::string input, Connection* conn) {
  Message join_msg = Message();
  std::string roomname = getRoomName(input);
  conn->send(Message(TAG_JOIN, roomname));
  conn->receive(join_msg);
  
  if(join_msg.tag != TAG_OK) {
    std::cerr << join_msg.data;
  }
}

/* carries out tasks to leave a room
 * 
 * Params:
 *  input - user input
 *  conn - connection object that interacts with server
 */
void leaveRoom(std::string input, Connection* conn) {
  Message leave_msg = Message();
  conn->send(Message(TAG_LEAVE, ""));
  conn->receive(leave_msg);
  
  if (leave_msg.tag != TAG_OK) {
    std::cerr << leave_msg.data;
  }
}

/* carries out tasks to send a message
 * 
 * Params:
 *  input - user input
 *  conn - connection object that interacts with server
 */
void sendMessage(std::string input, Connection* conn) {
  Message confirm_msg = Message();
  conn->send(Message(TAG_SENDALL, input));
  conn->receive(confirm_msg);
      
  if (confirm_msg.tag != TAG_OK) {
    std::cerr << confirm_msg.data;
  }
}
