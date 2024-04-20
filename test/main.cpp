#include <exception>
#include <iostream>
#include <memory>
#include <unchat5.hpp>
std::unique_ptr<base_chat> chat(new ChatEmpty());
int main(int argc, char **argv) {
  if (argc < 2) {
    return 0;
  }
  try {
    if (strcmp(argv[1], "server") == 0) {
      chat.reset(new ChatServer(std::atoi(argv[2])));
      std::cout << "connected!";
      chat->SendMessage("hello dear user");
    }
    if (strcmp(argv[1], "client") == 0) {
      chat.reset(new ChatClient(argv[2], std::atoi(argv[3])));
      std::cout << "connected!";
      try {
        std::string a = chat->RecieveMessage();
        std::cout << a;
      } catch (std::exception &e) {
        std::cout << "expection" << e.what() << std::endl;
      }
      DebugBreak();
    }
  } catch (std::exception &e) {
    std::cerr << e.what() << std::endl;
  }
  return 0;
}
