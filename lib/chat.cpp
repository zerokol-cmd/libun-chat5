#include "shared.hpp"
#include <cstdint>
#include <unchat5.hpp>

_cdecl ChatClient::ChatClient(const std::string &ip, uint16_t port) {
  socket_ = un::socket(ip, port);
}
_cdecl ChatServer::ChatServer(uint16_t port) { socket_ = un::socket(port); }
