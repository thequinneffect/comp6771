#include <iostream>
#include <string>
#include <vector>

#include "assignments/dg/graph.h"

// Note: At the moment, there is no client.sampleout. Please do your own testing

int main() {
  std::cout << "creating a Graph<double, int>\n";
  gdwg::Graph<double, int> g_double_int;

  std::cout << "creating a Graph<string, double> and adding vector of strings to it\n";
  std::vector<std::string> v{"Hello", "how", "are", "you"};
  gdwg::Graph<std::string, double> b{v.begin(),v.end()};

  std::cout << "inserting a new different node (should pass): ";
  std::cout << b.InsertNode("different") << "\n";
  std::cout << "inserting a new but already existing node (should fail): ";
  std::cout << b.InsertNode("Hello") << "\n";

  std::cout << "inserting a new different node (should pass): ";
  std::cout << g_double_int.InsertNode(6.5) << "\n";
  std::cout << "inserting a new different node (should pass): ";
  std::cout << g_double_int.InsertNode(2.3) << "\n";
  std::cout << "inserting a new but already existing node (should fail): ";
  std::cout << g_double_int.InsertNode(6.5) << "\n";

  std::cout << "about to insert some edges into the Graph<string, double>\n";
  std::cout << "inserting edge (Hello, how, 1) should SUCCEED\n";
  b.InsertEdge("Hello", "how", 1.0);
  std::cout << "inserting edge (how, are, 2) should SUCCEED\n";
  b.InsertEdge("how", "are", 2.0);
  std::cout << "inserting edge (are, you, 3) should SUCCEED\n";
  b.InsertEdge("are", "you", 3.0);
  std::cout << "inserting edge (Hello, how, 1) should FAIL\n";
  b.InsertEdge("Hello", "how", 1.0);

  std::cout << "about to iterate over Graph<string, double>\n";
  for (auto it = b.cbegin(); it != b.cend(); ++it) {
    auto [from, to, weight] = *it;
    std::cout << "[" << from << ", " << to << ", " << weight << "]\n";
  }

  std::cout << "about to insert some edges into the Graph<double, int>\n";
  std::cout << "inserting edge 6.5, 2.3, 6 should succeed: ";
  std::cout << g_double_int.InsertEdge(2.3, 6.5, 6) << "\n";
  std::cout << "inserting edge 6.5, 2.3, 5 should succeed: ";
  std::cout << g_double_int.InsertEdge(2.3, 6.5, 5) << "\n";
  std::cout << "inserting edge 6.5, 2.3, 6 should fail: ";
  std::cout << g_double_int.InsertEdge(2.3, 6.5, 6) << "\n";

  std::cout << "about to iterate over Graph<double, int>\n";
  for (auto it = g_double_int.cbegin(); it != g_double_int.cend(); ++it) {
    auto [from, to, weight] = *it;
    std::cout << "[" << from << ", " << to << ", " << weight << "]\n";
  }

  std::cout << "testing inserting into graph with vector of tuples\n";
  std::vector<std::tuple<double, double, int>> di_tuple {
    {6.5, 2.3, 5},
    {6.5, 7.0, 5},
    {4.2, 6.9, 7}, // 4
    {4.2, 2.3, 3}, // 3
    {4.3, 2.3, 3}, // 5
    {1.1, 1.1, 1}, // 1
    {1.1, 1.1, 2}, // 2
    {6.5, 2.3, 5},
    {6.5, 7.0, 5}
  }; 

  gdwg::Graph<double, int> gi_tuple_graph {di_tuple.begin(), di_tuple.end()};
  for (auto it : gi_tuple_graph) {
    auto [from, to, weight] = it;
    std::cout << "[" << from << ", " << to << ", " << weight << "]\n";
  }

  std::cout << "testing creating a graph with an initialiser list\n";
  gdwg::Graph<char, std::string> cs_il_graph { 'x', 'b', 'a', 'c', 'z', 'y', 'd'};

  cs_il_graph.InsertEdge('x', 'b', "aaa");
  cs_il_graph.InsertEdge('b', 'a', "bob");
  cs_il_graph.InsertEdge('a', 'c', "bbb");
  cs_il_graph.InsertEdge('a', 'c', "aaa");
  cs_il_graph.InsertEdge('z', 'y', "aaa");
  cs_il_graph.InsertEdge('y', 'x', "aaa");
  cs_il_graph.InsertEdge('b', 'x', "aaa");
  cs_il_graph.InsertEdge('b', 'x', "aaa");
  cs_il_graph.InsertEdge('b', 'x', "Aaa");
  cs_il_graph.InsertEdge('a', 'b', "ccc");
  cs_il_graph.InsertEdge('a', 'd', "z");
  cs_il_graph.InsertEdge('d', 'a', "z");
  cs_il_graph.InsertEdge('d', 'b', "g");
  cs_il_graph.InsertEdge('d', 'c', "f");

  for (auto it : cs_il_graph) {
    auto [from, to, weight] = it;
    std::cout << "[" << from << ", " << to << ", " << weight << "]\n";
  }

  std::cout << "copy constructing a graph with nodes and edges\n";
  gdwg::Graph<char, std::string> cs_il_graph_cpy {cs_il_graph};
  for (auto it : cs_il_graph_cpy) {
    auto [from, to, weight] = it;
    std::cout << "[" << from << ", " << to << ", " << weight << "]\n";
  }  

  std::cout << "move constructing a graph with nodes and edges\n";
  gdwg::Graph<char, std::string> cs_il_graph_mv {std::move(cs_il_graph)};
  for (auto it : cs_il_graph_mv) {
    auto [from, to, weight] = it;
    std::cout << "[" << from << ", " << to << ", " << weight << "]\n";
  }  

  std::cout << "move constructing a graph with nodes and edges v2 w/ auto\n";
  auto auto_moved_g {std::move(b)};
  for (auto it : auto_moved_g) {
    auto [from, to, weight] = it;
    std::cout << "[" << from << ", " << to << ", " << weight << "]\n";
  }

  std::cout << "testing dtor (letting graph go out of scope)\n";
  {
    std::cout << "testing creating a graph with an initialiser list\n";
    gdwg::Graph<char, std::string> cs_il_graph { 'x', 'b', 'a', 'c', 'z', 'y', 'd'};

    cs_il_graph.InsertEdge('x', 'b', "aaa");
    cs_il_graph.InsertEdge('b', 'a', "bob");
    cs_il_graph.InsertEdge('a', 'c', "bbb");
    cs_il_graph.InsertEdge('a', 'c', "aaa");
    cs_il_graph.InsertEdge('z', 'y', "aaa");
    cs_il_graph.InsertEdge('y', 'x', "aaa");
    cs_il_graph.InsertEdge('b', 'x', "aaa");
    cs_il_graph.InsertEdge('b', 'x', "aaa");
    cs_il_graph.InsertEdge('b', 'x', "Aaa");
    cs_il_graph.InsertEdge('a', 'b', "ccc");
    cs_il_graph.InsertEdge('a', 'd', "z");
    cs_il_graph.InsertEdge('d', 'a', "z");
    cs_il_graph.InsertEdge('d', 'b', "g");
    cs_il_graph.InsertEdge('d', 'c', "f");
  }

  std::cout << "testing dtor (does copy going out of scope break orig)\n";
  {
    gdwg::Graph<char, std::string> cs_il_graph_cpy2 {cs_il_graph_cpy};
  }
  std::cout << "printing original to see if it's valid\n";
  for (auto it : cs_il_graph_cpy) {
    auto [from, to, weight] = it;
    std::cout << "[" << from << ", " << to << ", " << weight << "]\n";
  }    

  std::cout << "testing dtor (does orig going out of scope break copy)\n";
  {
    gdwg::Graph<char, std::string> cs_il_graph_cpy3 {cs_il_graph_cpy};
    {
      gdwg::Graph<char, std::string> cs_il_graph_movee = std::move(cs_il_graph_cpy3);
    }
  }

  std::cout << "test creating a copy via assignment, and that changing the copy doesn't change original\n";
  gdwg::Graph<double, int> copy_assign_me {di_tuple.begin(), di_tuple.end()};
  gdwg::Graph<double, int> copy_assigned {copy_assign_me};
  copy_assigned.InsertEdge(1.1, 4.2, 9);
  for (auto it : copy_assign_me) {
    auto [from, to, weight] = it;
    std::cout << "[" << from << ", " << to << ", " << weight << "]\n";
  }
  for (auto it : copy_assigned) {
    auto [from, to, weight] = it;
    std::cout << "[" << from << ", " << to << ", " << weight << "]\n";
  }

  std::cout << "test move assignment, and that changing the moved from one doesn't change new one\n";
  gdwg::Graph<double, int> move_assigned {std::move(copy_assigned)};
  copy_assigned.~Graph();
  for (auto it : move_assigned) {
    auto [from, to, weight] = it;
    std::cout << "[" << from << ", " << to << ", " << weight << "]\n";
  }

  std::cout << "testing calling DeleteNode\n";
  move_assigned.InsertEdge(4.2, 1.1, 101);
  move_assigned.InsertEdge(4.2, 4.3, 101);
  move_assigned.InsertEdge(4.2, 6.5, 101);
  move_assigned.InsertEdge(4.2, 7.0, 101);
  move_assigned.InsertEdge(1.1, 4.2, 101);
  move_assigned.InsertEdge(4.3, 4.2, 101);
  move_assigned.InsertEdge(6.5, 4.2, 101);
  move_assigned.InsertEdge(7.0, 4.2, 101);
  std::cout << "before delete print;\n";
  for (auto it : move_assigned) {
    auto [from, to, weight] = it;
    std::cout << "[" << from << ", " << to << ", " << weight << "]\n";
  }
  move_assigned.DeleteNode(4.2);
  std::cout << "after delete print;\n";
  for (auto it : move_assigned) {
    auto [from, to, weight] = it;
    std::cout << "[" << from << ", " << to << ", " << weight << "]\n";
  }

  // std::cout << "inserting edge 7.0, 2.3, 6 should fail: ";
  // std::cout << g_double_int.InsertEdge(7.0, 2.3, 6) << "\n";
  
//   g.InsertNode("hello");
//   g.InsertNode("how");
//   g.InsertNode("are");
//   g.InsertNode("you?");

//   g.InsertEdge("hello", "how", 5);
//   g.InsertEdge("hello", "are", 8);
//   g.InsertEdge("hello", "are", 2);

//   g.InsertEdge("how", "you?", 1);
//   g.InsertEdge("how", "hello", 4);

//   g.InsertEdge("are", "you?", 3);

//   std::cout << g << '\n';

//   gdwg::Graph<std::string, int> g2{g};

//   std::cout << g2 << "\n";

//   // This is a structured binding.
//   // https://en.cppreference.com/w/cpp/language/structured_binding
//   // It allows you to unpack your tuple.
//   for (const auto& [from, to, weight] : g) {
//     std::cout << from << " -> " << to << " (weight " << weight << ")\n";
//   }
}
