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

int main(int argc, char **argv) {
  if (argc != 4) {
    std::cerr << "Usage: ./sender [server_address] [port] [username]\n";
    return 1;
  }

  std::string server_hostname;
  int server_port;
  std::string username;

  server_hostname = argv[1];
  server_port = std::stoi(argv[2]);
  username = argv[3];

  // Connection conn;

  if (!nameValid(username)) {
    std::cerr << "Invalid username" << std::endl;
    return -1;
  }

  // TODO: connect to server

  Connection *conn = new Connection();
  conn->connect(server_hostname, server_port);

  // TODO: send slogin message

  Message login_msg = Message();

  
  conn->send(Message(TAG_SLOGIN, username));
  conn->receive(login_msg);
  std::cout << login_msg.tag << ":" << login_msg.data << std::endl;
  
  if (login_msg.tag == TAG_ERR) {
    std::cerr << login_msg.data;
    delete conn;
    return -1;
  }  

  // TODO: loop reading commands from user, sending messages to
  //       server as appropriate
  std::string input;
  do {
    getline(std::cin, input); // check if this is memory safe
    input = trim(input);

    // std::cout << "Input read is: " << input << std::endl;

    if (input.find("/join") != std::string::npos) {
      Message join_msg = Message();
      std::string roomname = getRoomName(input);

      // if (!nameValid(roomname)) {
      //   std::cerr << "Invalid roomname" << std::endl;
      //   continue;
      // }

      conn->send(Message(TAG_JOIN, roomname));
      conn->receive(join_msg);
      // std::cout << join_msg.tag << ":" << join_msg.data << std::endl;
  
      if(join_msg.tag != TAG_OK) {
        std::cerr << join_msg.data;
      }

    } else if (input == "/leave") {
      Message leave_msg = Message();
      conn->send(Message(TAG_LEAVE, ""));
      conn->receive(leave_msg);
      // std::cout << leave_msg.tag << ":" << leave_msg.data << std::endl;

      if (leave_msg.tag != TAG_OK) {
        std::cerr << leave_msg.data;
      }

    } else if (input != "/quit") {
      Message confirm_msg = Message();
      conn->send(Message(TAG_SENDALL, input));
      conn->receive(confirm_msg);
      // std::cout << confirm_msg.tag << ":" << confirm_msg.data << std::endl;

      if (confirm_msg.tag != TAG_OK) {
        std::cerr << confirm_msg.data;
      }

    }

  } while (input != "/quit");
  

  // add back
  Message quit_msg = Message();
  conn->send(Message(TAG_QUIT, "bye"));
  conn->receive(quit_msg); 
  
  std::cout << quit_msg.tag << ": " << quit_msg.data << std::endl;
  

  // add back
  if (quit_msg.tag != TAG_OK) {
    //std::cout << "error was thrown";
    std::cerr << quit_msg.data;
    delete conn;
    return 4;
  }
  
  // conn->close();
  delete conn;

  return 0;
}

std::string getRoomName(std::string input) {
  return input.substr(6); // cuts off first 5 indicies "/join "
}
