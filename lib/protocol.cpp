#include <cassert>
#include <cstdint>
#include <cstdlib>
#include <json/reader.h>
#include <memory>
#include <protocol.hpp>
#include <unordered_map>
#include <vector>
// class cool_node {
// public:
//   std::vector<cool_node> dependencies;
//   struct dependency_map {
//     int node_index;
//     std::string propertry_name;
//   };
//   std::vector<dependency_map> input;
//   uint64_t size;
//   std::string return_value;
//   // specific node + node.output
//
//   void proccess(std::map<std::string, dependency_map> input_redirector) {
//     TaskGroup group;
//     for (node dep : dependencies) {
//       group.add_task([=, this]() { dep.proccess({}); });
//     }
//     group.wait();
//     for (auto a : input_redirector) {
//       input[std::atoi(a.first.c_str())] = a.second;
//     }
//     kmf::HKDF(input, size);
//   }
// };
// class Graph{
// public:
//   std::unordered_map<int, std::unordered_map<int>> a;
//
// };
// Graph un::Protocol::BuildNodeGraph(const Json::Value &node) {
//
//
//   return {};
// }
class base_node {
public:
  virtual void process() = 0;
};

class Graph {
  std::unordered_map<int, std::unordered_map<int, std::vector<base_node>>>
      graph;

public:
  void add_edge(int dependent, base_node dependency) {
    graph[dependent].push_back(dependency);
  }
  std::vector<graph_node> get_dependencies(const graph_node &node) {
    return graph[node];
  }
};
void un::Protocol::HandleInput(const Json::Value &value) {}
un::Protocol::Protocol(const std::string &JsonConfig)
    : handshake({"", "", "", "", ""}) {
  Json::Value root;
  Json::Reader reader;
  bool parsingSuccessful =
      reader.parse(JsonConfig.c_str(), root); // parse process
  if (!parsingSuccessful) {
    throw std::runtime_error("failed to parse:\n " +
                             reader.getFormattedErrorMessages());
  }
  HandleInput(root);
}
