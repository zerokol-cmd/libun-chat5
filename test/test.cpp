#include <exception>
#include <fstream>
#include <iostream>
#include <json/reader.h>
#include <json/value.h>
#include <memory>
#include <regex>
#include <set>
#include <string>
#include <variant>
#include <vector>
using bytes = std::vector<uint8_t>;
#define INDEX_DEPENDECIES_NOT_FOUND -1
#define ENCRYPTION_LAYER_INDEX -69
class encryptor_singletone
{
public:
  static bytes encode(const std::string &encryptor_name,
                      std::vector<bytes> input)
  {

    return {};
  }
};
struct encoder_options
{
  bytes nonce;
  bytes aad;
};
class node
{
  node DependentByIndex(int index) { return {}; }

public:
  std::string name;
  std::vector<node> dependencies;
  int index;
  encoder_options cryptor_options;
  // input number to node witch returns
  std::map<int, int> input_redirector;
  bytes last_return;
  size_t input_size;
  bytes encrypt()
  {
    std::vector<bytes> input;
    input.resize(input_size);
    for (auto &a : input_redirector)
    {
      input[a.first] = dependencies[a.second].encrypt();
    }
    last_return = encryptor_singletone::encode(this->name, input);

    return last_return;
  }
};
Json::Value findOutputNode(Json::Value nodes)
{
  Json::Value output;
  for (auto node : nodes)
  {
    try
    {
      if (node["output"].asBool() == true)
      {
        output = node;
      }
    }
    catch (std::exception &e)
    {
    }
  }
  return output;
}
std::pair<int, std::string> parseExpression(const std::string &expression)
{
  std::regex regexp(R"(\$index(\d+)\.(.+))");
  std::smatch matches;
  std::regex_search(expression, matches, regexp);
  if (matches[0].matched == true)
  {
    return {std::atoi(matches.str(1).c_str()), matches.str(2)};
  }
  return {INDEX_DEPENDECIES_NOT_FOUND, {}};
}
Json::Value getNodeByIndex(int index, Json::Value root)
{
  for (auto json_node : root)
  {
    if (json_node["index"].asInt() == index)
    {
      return json_node;
    }
  }
  return {}; // not found
}

std::pair<std::vector<Json::Value>, std::map<int, int>>
getDependentNodes(Json::Value input, const Json::Value &nodes)
{
  std::set<int> required_node_indexes;
  std::map<int, int> return_node;
  for (auto iterator = input.begin(); iterator != input.end(); iterator++)
  {
    int expression_result{INDEX_DEPENDECIES_NOT_FOUND};
    int input_index = 0;
    switch (iterator->type())
    {
    case Json::ValueType::stringValue:
      expression_result = parseExpression(iterator->asString()).first;
      if (expression_result > 0)
      {
        required_node_indexes.insert(expression_result);
        if (iterator.key() == "input")
        {
          return_node.insert({0, expression_result});
        }
      }
      break;
    case Json::ValueType::arrayValue:
      for (const auto &x : *iterator)
      {
        expression_result = parseExpression(x.asString()).first;
        if (expression_result > 0)
        {
          required_node_indexes.insert(expression_result);
        }
        if (iterator.key() == "input")

        {
          return_node.insert({input_index, expression_result});
        }

        input_index++;
      }
      input_index = 0;
      break;
    default: // чтобы не выебывался
      break;
    }
  }
  std::vector<Json::Value> required_nodes;
  for (int a : required_node_indexes)
  {
    required_nodes.push_back(getNodeByIndex(a, nodes));
  }
  return {required_nodes, return_node};
}
enum ConnectionType
{
  Client,
  Server
};
enum class type
{
  int_,
  bytes,
  string,
};
inline bytes readFile(const std::string &filePath)
{
  std::ifstream file(filePath, std::ios::in | std::ios::binary);
  if (!file.is_open())
  {
    throw std::runtime_error("file '" + filePath + "' not found.");
  }
  bytes data((std::istreambuf_iterator<char>(file)),
             std::istreambuf_iterator<char>());
  return data;
}
std::pair<type, std::variant<bytes, std::string, int>>
evaluateExpression(const std::string &expression)
{
  std::regex int_regex(R"(^int\((\d+)\)$)");
  std::regex file_regex(R"(^file\((.+)\)$)");
  std::smatch match;

  std::regex_search(expression, match, int_regex);
  if (match[0].matched)
  {
    return {type::int_, {std::atoi(match.str(1).c_str())}};
  }
  std::regex_search(expression, match, int_regex);
  if (match[0].matched)
  {
    return {type::bytes, readFile(match.str(1))};
  }
  return {type::string, expression};
}
node evaluateNode(const Json::Value &json_node, node &existing_node)
{
  for (auto iterator = json_node.begin(); iterator != json_node.end(); iterator++)
  {
    if (iterator.key() == "input")
    {
      switch (iterator->type())
      {
      case Json::ValueType::arrayValue:

      break;
      default:
        break;
      }
    }
  }
  return existing_node;
}
node generateNode(const Json::Value &json_node, const Json::Value &nodes)
{
  node Node;
  std::vector<node> dependencies{};
  auto dependent_pair = getDependentNodes(json_node, nodes);
  for (const auto &a : dependent_pair.first)
  {
    try
    {
      dependencies.push_back(generateNode(a, nodes));
    }
    catch (std::exception &e)
    {
      std::cerr << "error:" << e.what() << std::endl;
    }
  }
  Node.name = json_node["method"].asString();
  Node.dependencies = dependencies;
  Node.input_redirector = dependent_pair.second;
  Node.index = json_node["index"].asInt();
  return Node;
}
node generateGraph(Json::Value json_node, ConnectionType type)
{
  node root_node;
  Json::Value output_node = findOutputNode(json_node["keyexchange"]);
  std::cout << output_node["method"].asString() << std::endl;
  node NOde = generateNode(output_node, json_node["keyexchange"]);
  std::cout << NOde.name << std::endl;
  std::cout << '\t';
  for (auto &dep : NOde.dependencies)
  {
    std::cout << "dependent nodes:" << dep.name << std::endl;
    std::cout << '\t';
  }
  return root_node;
};

int main(int argc, char **argv)
{
  if (argc < 2)
  {
    return -1;
  }
  std::ifstream file{std::string(argv[1])};
  Json::Value root;

  Json::CharReaderBuilder builder;
  parseFromStream(builder, file, &root, nullptr);
  node result = generateGraph(root, Client);
  result.encrypt();
  auto pair = parseExpression("$index1.file");
  std::cout << pair.first << std::endl;
  std::cout << pair.second << std::endl;
}
