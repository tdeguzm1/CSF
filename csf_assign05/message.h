#ifndef MESSAGE_H
#define MESSAGE_H

#include <vector>
#include <string>

// standard message tags (note that you don't need to worry about
// "senduser" or "empty" messages)
#define TAG_ERR       "err"       // protocol error
#define TAG_OK        "ok"        // success response
#define TAG_SLOGIN    "slogin"    // register as specific user for sending
#define TAG_RLOGIN    "rlogin"    // register as specific user for receiving
#define TAG_JOIN      "join"      // join a chat room
#define TAG_LEAVE     "leave"     // leave a chat room
#define TAG_SENDALL   "sendall"   // send message to all users in chat room
#define TAG_SENDUSER  "senduser"  // send message to specific user in chat room
#define TAG_QUIT      "quit"      // quit
#define TAG_DELIVERY  "delivery"  // message delivered by server to receiving client
#define TAG_EMPTY     "empty"     // sent by server to receiving client to indicate no msgs available

struct Message {
  // An encoded message may have at most this many characters,
  // including the trailing newline ('\n'). Note that this does
  // *not* include a NUL terminator (if one is needed to
  // temporarily store the encoded message.)
  static const unsigned MAX_LEN = 255;

  std::string tag;
  std::string data;

  Message() { }

  Message(const std::string &tag, const std::string &data)
    : tag(tag), data(data) { }

  // returns the payload (data) as a vector of strings,
  // split using ':' as the separator
  std::vector<std::string> split_payload() const {
    std::vector<std::string> result;
    // TODO: split the message data into fields separated by ':', add them
    //       to result vector
    result.push_back(tag);
    result.push_back(": ");
    result.push_back(data);
    return result;
  }

  bool hasValidTag() const {
    if (tag == TAG_ERR || tag == TAG_OK || tag == TAG_SLOGIN || tag == TAG_RLOGIN 
        || tag == TAG_JOIN || tag == TAG_LEAVE || tag == TAG_SENDALL || tag == TAG_SENDUSER 
        || tag == TAG_QUIT || tag == TAG_DELIVERY || tag == TAG_EMPTY) {
      return true;
    }
    return false;
  }
};

#endif // MESSAGE_H
