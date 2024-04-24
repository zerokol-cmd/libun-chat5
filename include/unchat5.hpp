#pragma once
#include <cstdint>
#include <shared.hpp>
#include <socket.hpp>
#include <protocol.hpp>
#undef SendMessage
class base_chat {
protected:

  un::socket socket_;

public:
  base_chat() {}
  virtual void SendMessage(const std::string &message) = 0;
  virtual std::string RecieveMessage() = 0;
};
class ChatEmpty : public base_chat {
public:
  virtual void SendMessage(const std::string &message) {}
  virtual std::string RecieveMessage() { return ""; }
};
class ChatServer : public base_chat {
public:
  __export ChatServer(uint16_t port);
  void SendMessage(const std::string &message) override {
    socket_.send(std::vector<uint8_t>(message.begin(), message.end()));
  }
  std::string RecieveMessage() override {
    bytes data = socket_.receive();
    return std::string(data.begin(), data.end());
  }
};
class ChatClient : public base_chat {
public:
  __export ChatClient(const std::string &ip, uint16_t port);
  void SendMessage(const std::string &message) override {
    socket_.send(std::vector<uint8_t>(message.begin(), message.end()));
  }
  std::string RecieveMessage() override {
    bytes data = socket_.receive();
    return std::string(data.begin(), data.end());
  }
};
