#include "shared.hpp"
#include <WinSock2.h>
#include <socket.hpp>
#include <stdexcept>
#include <string>
un::socket::socket() {}
un::socket::~socket() {
  shutdown(ClientSocket, SD_SEND);
  shutdown(ClientSocket, SD_SEND);
  closesocket(ClientSocket);
  closesocket(ClientSocket);
  WSACleanup();
}
un::socket::socket(const std::string &ip, uint16_t port) {
  struct addrinfo *result = NULL, *ptr = NULL, hints;
  int iResult;
  iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
  if (iResult != 0) {
    throw std::runtime_error("failed to startup wsa");
  }

  ZeroMemory(&hints, sizeof(hints));
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_protocol = IPPROTO_TCP;

  iResult =
      ::getaddrinfo(ip.c_str(), std::to_string(port).c_str(), &hints, &result);
  if (iResult != 0) {
    throw std::runtime_error("failed to find port");
  }
  for (ptr = result; ptr != NULL; ptr = ptr->ai_next) {
    ClientSocket = ::socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
    if (ClientSocket == INVALID_SOCKET) {
      throw std::runtime_error("failed to create socket");
    }

    iResult = connect(ClientSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
    if (iResult == SOCKET_ERROR) {
      closesocket(ClientSocket);
      ClientSocket = INVALID_SOCKET;
      continue;
    }
    break;
  }

  freeaddrinfo(result);

  if (ClientSocket == INVALID_SOCKET) {
    throw std::runtime_error("failed to connect socket");
  }
  inited = true;
}
un::socket::socket(uint16_t port) {
  int iResult;

  struct addrinfo *result = NULL;
  struct addrinfo hints;

  iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
  if (iResult != 0) {
    throw std::runtime_error("failed to Initialize wsa sockets");
  }

  ZeroMemory(&hints, sizeof(hints));
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_protocol = IPPROTO_TCP;
  hints.ai_flags = AI_PASSIVE;

  iResult = ::getaddrinfo(NULL, std::to_string(port).c_str(), &hints, &result);
  if (iResult != 0) {
    throw std::runtime_error("port not found");
  }

  ClientSocket =
      ::socket(result->ai_family, result->ai_socktype, result->ai_protocol);
  if (ClientSocket == INVALID_SOCKET) {
    throw std::runtime_error("failed to init socket");
  }

  iResult = bind(ClientSocket, result->ai_addr, (int)result->ai_addrlen);
  if (iResult == SOCKET_ERROR) {
    throw std::runtime_error("failed to bind socket");
  }

  freeaddrinfo(result);

  iResult = listen(ClientSocket, SOMAXCONN);
  if (iResult == SOCKET_ERROR) {
    throw std::runtime_error("failed to listen socket");
  }

  ClientSocket = accept(ClientSocket, NULL, NULL);
  if (ClientSocket == INVALID_SOCKET) {
    throw std::runtime_error("failed to accept connection");
  }
  closesocket(ClientSocket);

  inited = true;
}
bytes un::socket::receive() {
  bytes result;
  result.resize(4096);
  if (recv(ClientSocket, (char *)result.data(), 4096, 0) == SOCKET_ERROR) {
    throw std::runtime_error(std::string( "failed to receive buffer ")+std::to_string(WSAGetLastError()));
  }
  return result;
}
void un::socket::send(bytes b) {
  if (!inited) {
    throw std::runtime_error("socket is not inited");
  }
  if (::send(ClientSocket, (const char *)b.data(), b.size(), 0) ==
      SOCKET_ERROR) {
    throw std::runtime_error(std::string("failed to send data ")+std::to_string(WSAGetLastError()));
  }
}
