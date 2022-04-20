#include <iostream>
#include <string>
#include <sstream>
#include <stdexcept>
#include "csapp.h"
#include "message.h"
#include "connection.h"
#include "client_util.h"

std::string getRoomName(std::string input);

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

  // TODO: connect to server

  Connection *conn = new Connection();
  conn->connect(server_hostname, server_port);

  // TODO: send slogin message

  Message login_msg = Message();
  
  conn->send(Message(TAG_RLOGIN, username));
  conn->receive(login_msg);
  
  if (login_msg.tag == TAG_ERR) {
    std::cerr << login_msg.data;
    delete conn;
    throw -1;
  }  

  // TODO: loop reading commands from user, sending messages to
  //       server as appropriate
  std::string input;
  do {
    getline(std::cin, input); // check if this is memory safe
    input = trim(input);

    std::cout << "Input read is: " << input << std::endl;

    if (input.find("/join") != std::string::npos) {
      Message join_msg = Message();
      conn->send(Message(TAG_JOIN, getRoomName(input)));
      conn->receive(join_msg);
  
      if(join_msg.tag == TAG_ERR) {
        std::cerr << join_msg.data;
        throw -2;
      }
    } else if (input == "/leave") {
      Message leave_msg = Message();
      conn->send(Message(TAG_LEAVE, ""));
      conn->receive(leave_msg);

      if (leave_msg.tag == TAG_ERR) {
        std::cerr << leave_msg.data;
      }

    } else if (input != "/quit") {
      Message confim_msg = Message();
      conn->send(Message(TAG_SENDALL, input));
      conn->receive(confim_msg);

      if (confim_msg.tag == TAG_ERR) {
        std::cerr << confim_msg.data;
      }
    }

  } while (input != "/quit");

  Message quit_msg = Message();
  conn->send(Message(TAG_QUIT, "bye"));
  conn->receive(quit_msg);

  if (quit_msg.tag == TAG_ERR) {
    std::cerr << quit_msg.data;
    delete conn;
    throw 4;
  }

  conn->close();
  delete conn;

  return 0;
}

std::string getRoomName(std::string input) {
  return input.substr(6); // cuts off first 5 indicies "/join "
}
