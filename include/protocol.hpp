#pragma once
#include "shared.hpp"
#include <json/value.h>
#include <socket.hpp>
#include <string>
namespace un {
class Key {
  bytes data_;

public:
  __export char *data();
  __export size_t size();
};
struct JsonToProtocolConfig {
  struct Config {
    struct HandshakeConfig {
      std::string keyexchange_method;
      std::string encryption_method;
      std::string nickname;

      std::string public_key_path;
      std::string private_key_path;
    };
  };
  __export Config ParseJson(const std::string &Json);
};
class Handshake {
public:
  Key HandshakeSocket(socket &socket);
  Handshake(const JsonToProtocolConfig::Config::HandshakeConfig &Config){

  }
};

class Protocol {
  Handshake handshake;
  Key shared_key;
  bool connected = false;
  void HandleInput(const Json::Value &root);

public:
  // return true if operation success
  __export bool Handshake(socket &Socket);
  __export bytes EncryptMessage(bytes Message);
  __export bytes DecryptMessage(bytes Message);
  __export Protocol(const std::string &ConfigJson);
  __export Protocol(); // empty constructor for empty chat
};

} // namespace un
