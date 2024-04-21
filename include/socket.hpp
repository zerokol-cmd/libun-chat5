#pragma once
#include <cstdint>
#include <shared.hpp>
#include <string>
#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <WinSock2.h>
#include <Windows.h>
#include <ws2tcpip.h>
#endif
#pragma comment(lib, "Ws2_32.lib")
namespace un {
class socket {
  SOCKET ClientSocket = INVALID_SOCKET;
  bool inited = false;
  WSADATA wsaData;

public:
  __export socket();
  __export ~socket();
  __export socket(const std::string &ip, uint16_t port);
  __export socket(uint16_t port);
  __export bytes receive();
  __export void send(bytes data);
};
} // namespace un
