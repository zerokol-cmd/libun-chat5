#include <cstdint>
#include <iostream>
#include <json/value.h>
#include <stdexcept>
#include <string>
#include <vector>
using bytes = std::vector<uint8_t>;
using input_directorion = std::pair<int, std::string>;
enum class ProcessType {
  Encode,
  Decode,
};
class cryptor_base {
public:
  virtual bytes process(ProcessType type, std::vector<bytes> input) = 0;
};
class node {
public:
  std::vector<node> dependencies;
  std::vector<input_directorion> input;
  size_t inputs;
  bytes return_value;
  std::string cryptor_name;
  std::unique_ptr<cryptor_base> cryptor;
  void process(ProcessType type) {
    for (auto &node_ : dependencies) {
      node_.process(type);
    }
    std::vector<bytes> redirected_input;
    for (auto &redirection : input) {
       redirected_input.push_back(dependencies[redirection.first].return_value);
    }
    cryptor.get()->process(type, redirected_input);
  }
};
class scrypt : public cryptor_base {
public:
  bytes return_value;
  virtual bytes process(ProcessType type, std::vector<bytes> input) override {
    printf("scrypt called");
    return {};
  }
};
class HKDF : public cryptor_base {
public:
  virtual bytes process(ProcessType type, std::vector<bytes> input) override {
    printf("crypting two keys process started\n");
    return {};
  }
};
class Chacha30 : public cryptor_base {
public:
  virtual bytes process(ProcessType type, std::vector<bytes> input) override {
    printf("chacha process started\n");
    return {};
  }
};

std::vector<input_directorion>
getInputRedirectionForProperty(Json::Value value,
                               const std::string &propertyName) {
  return {};
}
struct algorithm_options {
  size_t input_count;
  std::string full_name;
};
algorithm_options getAlgorithmOptions(const std::string &algorithmName) {
  if (algorithmName == "XChaCha20") {
    return {1, "XChaCha20-Poly1305-IETF"};
  }
  throw std::runtime_error("algorithm not found");
  return {};
}
std::vector<Json::Value> getDependencies(Json::Value node) { return {}; }
cryptor_base *createCryptorByName(const std::string &algorithm) {
  return dynamic_cast<cryptor_base *>(new Chacha30());
}
node generateNode(Json::Value json_node) {
  node Node;
  for (auto node : getDependencies(json_node)) {
    Node.dependencies.push_back(generateNode(node));
  }
  algorithm_options options =
      getAlgorithmOptions(json_node["method"].asString());
  Node.inputs = options.input_count;
  Node.input.resize(Node.inputs);
  Node.input = getInputRedirectionForProperty(json_node, "input");

  Node.cryptor_name = options.full_name;
  Node.cryptor.reset(createCryptorByName(options.full_name));
  return Node;
}

int main() { return 0; }
