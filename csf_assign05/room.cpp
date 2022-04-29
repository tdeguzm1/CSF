#include "guard.h"
#include "message.h"
#include "message_queue.h"
#include "user.h"
#include "room.h"

Room::Room(const std::string &room_name)
  : room_name(room_name) {
  // TODO: initialize the mutex
  pthread_mutex_init(&lock, nullptr);
}

Room::~Room() {
  // TODO: destroy the mutex
  pthread_mutex_destroy(&lock);
}

void Room::add_member(User *user) {
  // TODO: add User to the room
  pthread_mutex_lock(&lock);
  members.insert(user); // TODO: check for a user with the same username
  pthread_mutex_unlock(&lock);

}

void Room::remove_member(User *user) {
  // TODO: remove User from the room
  pthread_mutex_lock(&lock);
  members.erase(user); // TODO: check for error state
  pthread_mutex_unlock(&lock);
}


void Room::broadcast_message(const std::string &sender_username, const std::string &message_text) { // define message_text as the entire TPC message minus the tag
  // TODO: send a message to every (receiver) User in the room
  pthread_mutex_lock(&lock);
  
  for (std::set<User *>::iterator it = members.begin(); it != members.end(); it++) {
    if ((*it)->username != sender_username) {
      (*it)->mqueue.enqueue(new Message(TAG_DELIVERY, message_text)); // go through and delete this
    }
  }
  pthread_mutex_unlock(&lock);
}
