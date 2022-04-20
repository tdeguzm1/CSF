#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>
#include "csapp.h"
#include "message.h"
#include "connection.h"
#include "client_util.h"

void printMessage(std::string data);

int main(int argc, char **argv) {
  if (argc != 5) {
    std::cerr << "Usage: ./receiver [server_address] [port] [username] [room]\n";
    return 1;
  }

  std::string server_hostname = argv[1];
  char* hostname_cString = argv[1];
  int server_port = std::stoi(argv[2]);
  char* port_cString = argv[2];
  std::string username = argv[3];
  std::string room_name = argv[4];

  // Connection conn;

  // TODO: connect to server
  
  Connection *conn = new Connection();
  //conn->connect(server_hostname, server_port);
  conn->connect(hostname_cString, port_cString);

  // TODO: send rlogin and join messages (expect a response from
  //       the server for each one)
  Message login_msg = Message();
  Message join_msg = Message();
  
  conn->send(Message(TAG_RLOGIN, username));
  //std::cout << "rlogin attempt" << std::endl;

  conn->receive(login_msg);
  //::cout << "rlogin message recieved2" << std::endl;

  //std::cout << login_msg.tag << std::endl;
  
  if (login_msg.tag == TAG_ERR) {
    //std::cout << "rlogin error detected" << std::endl;
    std::cerr << login_msg.data;
    delete conn;
    throw 1;
  }  
  //std::cout << "rlogin complete" << std::endl;
  
  conn->send(Message(TAG_JOIN, room_name));
  conn->receive(join_msg);
  
  if (join_msg.tag == TAG_ERR) {
    std::cerr << join_msg.data;
    delete conn;
    throw 2;
  }


  // TODO: loop waiting for messages from server
  //       (which should be tagged with TAG_DELIVERY)

  while (true) {
    Message msg = Message();
    // conn.receive(msg);
    // if (msg.tag == TAG_DELIVERY) {
    if (conn->receive(msg)) {
      if (msg.tag == TAG_DELIVERY) {
        //std::cout << "message recieved" << std::endl;
        printMessage(msg.data);
      }
      else if (msg.tag == TAG_ERR) {
        std::cout << msg.data;
      }
    }
  }

  return 0;
}

void printMessage(std::string data) {
   //std::cout << "print Message reached: ";
   size_t sender = data.find(":");
   data = data.substr(sender + 1);

   size_t message = data.find(":");
   std::string username = trim(data.substr(0, message));
   std::string msg = trim(data.substr(message + 1));

   std::cout << username << ": " << msg << std::endl;
}
