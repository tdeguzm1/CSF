// #include <pthread.h>
// #include <iostream>
// #include <sstream>
// #include <memory>
// #include <set>
// #include <vector>
// #include <cctype>
// #include <cassert>
// #include "message.h"
// #include "connection.h"
// #include "user.h"
// #include "room.h"
// #include "guard.h"
// #include "server.h"

// ////////////////////////////////////////////////////////////////////////
// // Server implementation data types
// ////////////////////////////////////////////////////////////////////////

// // TODO: add any additional data types that might be helpful
// //       for implementing the Server member functions

// ////////////////////////////////////////////////////////////////////////
// // Client thread functions
// ////////////////////////////////////////////////////////////////////////

// namespace {

// void *worker(void *arg) {
//   pthread_detach(pthread_self());

//   // TODO: use a static cast to convert arg from a void* to
//   //       whatever pointer type describes the object(s) needed
//   //       to communicate with a client (sender or receiver)

//   // TODO: read login message (should be tagged either with
//   //       TAG_SLOGIN or TAG_RLOGIN), send response

//   // TODO: depending on whether the client logged in as a sender or
//   //       receiver, communicate with the client (implementing
//   //       separate helper functions for each of these possibilities
//   //       is a good idea)

//   return nullptr;
// }

// }

// ////////////////////////////////////////////////////////////////////////
// // Server member function implementation
// ////////////////////////////////////////////////////////////////////////

// Server::Server(int port)
//   : m_port(port)
//   , m_ssock(-1) {
//   // TODO: initialize mutex
// }

// Server::~Server() {
//   // TODO: destroy mutex
// }

// bool Server::listen() {
//   // TODO: use open_listenfd to create the server socket, return true
//   //       if successful, false if not
//   return false;
// }

// void Server::handle_client_requests() {
//   // TODO: infinite loop calling accept or Accept, starting a new
//   //       pthread for each connected client
// }

// Room *Server::find_or_create_room(const std::string &room_name) {
//   // TODO: return a pointer to the unique Room object representing
//   //       the named chat room, creating a new one if necessary
//   return new Room(room_name);
// }



///////////// START OF SERVER SKELETON.CPP /////////////////////

#include <pthread.h>
#include <iostream>
#include <sstream>
#include <memory>
#include <set>
#include <vector>
#include <cctype>
#include <cassert>
#include "message.h"
#include "connection.h"
#include "user.h"
#include "room.h"
#include "guard.h"
#include "server.h"

////////////////////////////////////////////////////////////////////////
// Server implementation data types
////////////////////////////////////////////////////////////////////////

struct ConnInfo {
  Connection *conn;
  Server *server;

  ConnInfo(Connection *conn, Server *server) : conn(conn), server(server) { }
  ~ConnInfo() {
    // destroy connection when ConnInfo object is destroyed
    delete conn;
  }
};

void receiverServerLoop(ConnInfo* info, User* user);
void senderServerLoop(ConnInfo* info, User* user);

////////////////////////////////////////////////////////////////////////
// Client thread functions
////////////////////////////////////////////////////////////////////////

namespace {

void *worker(void *arg) {
  pthread_detach(pthread_self());

  ConnInfo *info_ = static_cast<ConnInfo *>(arg);

  // use a std::unique_ptr to automatically destroy the ConnInfo object
  // when the worker function finishes; this will automatically ensure
  // that the Connection object is destroyed
  std::unique_ptr<ConnInfo> info(info_);

  Message msg;

  if (!info->conn->receive(msg)) {
    if (info->conn->get_last_result() == Connection::INVALID_MSG) {
      info->conn->send(Message(TAG_ERR, "invalid message"));
    }
    return nullptr;
  }

  if (msg.tag != TAG_SLOGIN && msg.tag != TAG_RLOGIN) {
    info->conn->send(Message(TAG_ERR, "first message should be slogin or rlogin"));
    return nullptr;
  }

  std::string username = msg.data;
  if (!info->conn->send(Message(TAG_OK, "welcome " + username))) {
    return nullptr;
  }
  User* userObj = new User(username);
  
  if (msg.tag == TAG_RLOGIN) {
    receiverServerLoop(info_, userObj);
  } else {
    senderServerLoop(info_, userObj);
  }

  return nullptr;
}

}

void receiverServerLoop(ConnInfo* info, User* user) {
  Message msg;
  info->conn->receive(msg);
  if(msg.tag != TAG_JOIN) {
    info->conn->send(Message(TAG_ERR, "failed to join room"));
    std::cout << "room join failure occurred" << std::endl;
    return;
  } else {
    Room* myRoom = info->server->find_or_create_room(msg.data);
    myRoom->add_member(user);
    std::cout << "room join success occurred" << std::endl;
    Message joinMsg = Message(TAG_OK, "joined room okay");
    std::cout << "Sent join confirm: " << joinMsg.tag << std::endl;
    std::cout << "Sent join confirm: " << joinMsg.data << std::endl;
    info->conn->send(joinMsg);
    std::cout << "sent join success msg" << std::endl;
  }
  
  Message* sentMsg;
  while (true) {
    sentMsg = user->mqueue.dequeue();
    if (sentMsg != nullptr) {
      info->conn->send(*sentMsg);
    }
  }
}

void senderServerLoop(ConnInfo* info, User* user) {
  bool in_room = false;
  Message msg;
  Room* myRoom;
  while (true) {
    info->conn->receive(msg);
    if (msg.tag == TAG_JOIN) {
      if (in_room) {
        myRoom->remove_member(user);
      }
      myRoom = info->server->find_or_create_room(msg.data);
      myRoom->add_member(user);
      in_room = true;
      info->conn->send(Message(TAG_OK, "joined room successfully"));
    } else if (msg.tag == TAG_LEAVE) {
      myRoom->remove_member(user);
      in_room = false;
      info->conn->send(Message(TAG_OK, "left room successfully"));
    } else if (msg.tag == TAG_SENDALL) {
      if (!in_room) {
        info->conn->send(Message(TAG_ERR, "must be in room to send a message"));
      }
      myRoom->broadcast_message(user->username, msg.data);
      info->conn->send(Message(TAG_OK, "sendall message recieved"));
    }
  }
}

////////////////////////////////////////////////////////////////////////
// Server member function implementation
////////////////////////////////////////////////////////////////////////

Server::Server(int port)
  : m_port(port)
  , m_ssock(-1) {
  pthread_mutex_init(&m_lock, nullptr);
}

Server::~Server() {
  pthread_mutex_destroy(&m_lock);
}

bool Server::listen() {
  std::string port = std::to_string(m_port);
  m_ssock = open_listenfd(port.c_str());
  return m_ssock >= 0;
}

void Server::handle_client_requests() {
  assert(m_ssock >= 0);

  while (true) {
    int clientfd = accept(m_ssock, nullptr, nullptr);
    if (clientfd < 0) {
      std::cerr << "Error accepting connection\n";
      return;
    }

    ConnInfo *info = new ConnInfo(new Connection(clientfd), this);

    pthread_t thr_id;
    if (pthread_create(&thr_id, nullptr, worker, static_cast<void *>(info)) != 0) {
      std::cerr << "Could not create thread\n";
      return;
    }
  }
}

Room *Server::find_or_create_room(const std::string &room_name) {
  // this function can be called from multiple threads, so
  // make sure the mutex is held while accessing the shared
  // data (the map of room names to room objects)
  Guard g(m_lock);

  Room *room;

  auto i = m_rooms.find(room_name);
  if (i == m_rooms.end()) {
    // room does not exist yet, so create it and add it to the map
    room = new Room(room_name);
    m_rooms[room_name] = room;
  } else {
    room = i->second;
  }

  return room;
}