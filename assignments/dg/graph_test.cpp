/*

  == Explanation and rational of testing ==

  Explain and justify how you approached testing, the degree
  to which you're certain you have covered all possibilities,
  and why you think your tests are that thorough.

  how i approached testing
  {
    explanation: during the development of the implementation,
    because loads of things were really
    tricky to get right (i.e. a lot of the stuff to do with
    containers of smart pointers and manipulating those), I
    did lots of implementation-side testing (i.e. inside the
    tpp file)) with asserts, printing, debugger etc. because
    that was the only way to really see what was going on and
    that the state of things were as expected. Then to see
    if methods roughly worked before officially testing them,
    I made my own client file that just tested most of the
    basics. Once I was confident in my implementation and all
    parts were complete (except reverse iterator, i did not
    do that because i couldnt get the std::reverse_iterator
    template to generate it without tons of errors), I moved
    over to this file to do some catch2 tests.

    justification: Because I did a lot of unofficial testing
    during development, I am pretty happy with how my design
    is running (SEE PROOF OF UNOFFICIAL TESTING DOWN THE
    BOTTOM - my client file i used for testing). For this
    reason, I didn't go too heavy with the testing in this
    file.
  }

  degree and thoroughness {
    I think i have covered pretty much all of the basics, most
    of the obvious edge cases but I feel like (due to the
    complexity of the assignment and my implementation of it),
    there are probably a few cases that would break my design.
    I feel like this is to be expected when using smart pointers
    in this fashion for my first time using them, so I am okay
    with these edge cases existing as it would simply take too
    long to think of them and alter my implementation to fix them.
  }

*/

#include "assignments/dg/graph.h"
#include "catch.h"

/* PROOF OF UNOFFICIAL TESTING

#include <iostream>
#include <string>
#include <vector>

#include <cassert>

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

  std::cout << "test creating a copy via assignment, and that changing the copy doesn't change
original\n"; gdwg::Graph<double, int> copy_assign_me {di_tuple.begin(), di_tuple.end()};
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

  std::cout << "test move assignment, and that changing the moved from one doesn't change new
one\n"; gdwg::Graph<double, int> move_assigned {std::move(copy_assigned)}; copy_assigned.~Graph();
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

  std::cout << "testing clearing and then adding again\n";
  gdwg::Graph<double, int> clear_me {di_tuple.begin(), di_tuple.end()};
  for (auto it : clear_me) {
    auto [from, to, weight] = it;
    std::cout << "[" << from << ", " << to << ", " << weight << "]\n";
  }
  clear_me.Clear();
  std::cout << "printing after clear\n";
  for (auto it : clear_me) {
    auto [from, to, weight] = it;
    std::cout << "[" << from << ", " << to << ", " << weight << "]\n";
  }
  std::cout << "printing after adding after clear\n";
  clear_me.InsertNode(1.1);
  clear_me.InsertNode(2.2);
  clear_me.InsertNode(3.3);
  clear_me.InsertNode(4.4);
  clear_me.InsertNode(5.5);
  clear_me.InsertEdge(1.1, 1.1, 1);
  clear_me.InsertEdge(1.1, 1.1, 2);
  clear_me.InsertEdge(1.1, 1.1, -1);
  clear_me.InsertEdge(2.2, 1.1, 1);
  clear_me.InsertEdge(2.2, 2.2, 2);
  clear_me.InsertEdge(3.3, 2.2, 3);
  clear_me.InsertEdge(3.3, 3.3, 300);
  clear_me.InsertEdge(1.1, 3.3, 60);
  clear_me.InsertEdge(2.2, 3.3, 343);
  clear_me.InsertEdge(4.4, 1.1, 4);
  clear_me.InsertEdge(4.4, 4.4, 16);
  clear_me.InsertEdge(2.2, 4.4, 8);

  for (auto it : clear_me) {
    auto [from, to, weight] = it;
    std::cout << "[" << from << ", " << to << ", " << weight << "]\n";
  }

  std::cout << "test isconnected\n";
  std::cout << "isconnected should be 1: " << clear_me.IsConnected(4.4, 1.1) << "\n";
  std::cout << "isconnected should be 1: " << clear_me.IsConnected(4.4, 4.4) << "\n";
  std::cout << "isconnected should be 1: " << clear_me.IsConnected(2.2, 4.4) << "\n";
  std::cout << "isconnected should be 0: " << clear_me.IsConnected(4.4, 3.3) << "\n";
  std::cout << "isconnected should be 0: " << clear_me.IsConnected(5.5, 5.5) << "\n";
  std::cout << "isconnected should be 0: " << clear_me.IsConnected(4.4, 2.2) << "\n";
  

  std::cout << "testing getnodes\n";
  auto node_vec1 = cs_il_graph_cpy.GetNodes();
  auto node_vec2 = clear_me.GetNodes();
  std::cout << "printing nodevec1\n";
  for (auto node : node_vec1) {
    std::cout << node << " ";
  }
  std::cout << "\nnow printing nodevec2\n";
  for (auto node : node_vec2) {
    std::cout << node << " ";
  }
  std::cout << "\n";

  std::cout << "testing getconnected\n";
  auto conn_vec1 = cs_il_graph_cpy.GetConnected('a');
  auto conn_vec2 = clear_me.GetConnected(4.4);
  std::cout << "printing connvec1\n";
  for (auto node : conn_vec1) {
    std::cout << node << " ";
  }
  std::cout << "\nnow printing connvec2\n";
  for (auto node : conn_vec2) {
    std::cout << node << " ";
  }
  std::cout << "\n";

  std::cout << "testing getweights\n";
  auto weight_vec1 = cs_il_graph_cpy.GetWeights('a', 'c');
  auto weight_vec2 = clear_me.GetWeights(1.1, 1.1);
  std::cout << "printing weightvec1\n";
  for (auto e : weight_vec1) {
    std::cout << e << " ";
  }
  std::cout << "\nnow printing weightvec2\n";
  for (auto e : weight_vec2) {
    std::cout << e << " ";
  }
  std::cout << "\n";

  std::cout << "testing find!\n";
  gdwg::Graph<std::string, int> gsi;
  std::string a1{"e"};
  std::string a2{"i"};
  int e = 8;
  auto it = gsi.find(a1, a2, e);
  assert(it == gsi.end());
  gsi.InsertNode(a1);
  gsi.InsertNode(a2);
  gsi.InsertEdge(a1, a2, e);
  auto it2 = gsi.find(a1, a2, e);
  assert(it2 != gsi.end());
  auto [s, d, w] = *it2;
  std::cout << "found the searched for edge: [" << s << ", " << d << ", " << w << "]\n";

  std::cout << "test erasing by manually specifying edge\n";
  std::cout << "erasing non existant edge should fail: " << clear_me.erase(1.1, 4.4, 7) << "\n";
  std::cout << "erasing existant edge should succeed: " << clear_me.erase(1.1, 3.3, 60) << "\n";
  for (auto it : clear_me) {
    auto [from, to, weight] = it;
    std::cout << "[" << from << ", " << to << ", " << weight << "]\n";
  }

  std::cout << "test erasing via iterator\n";
  auto end_it = clear_me.find(1.1, 4.4, 7);
  assert(end_it == clear_me.cend());
  auto end_it2 = clear_me.erase(end_it);
  assert(end_it2 == clear_me.end());
  auto found_it = clear_me.find(2.2, 4.4, 8);
  assert(found_it != clear_me.cend());
  clear_me.erase(found_it);
  for (auto it : clear_me) {
    auto [from, to, weight] = it;
    std::cout << "[" << from << ", " << to << ", " << weight << "]\n";
  }
  std::cout << "testing erasing last\n";
  auto last_e = clear_me.find(4.4, 4.4, 16);
  auto found_it2 = clear_me.erase(last_e);
  assert(found_it2 == clear_me.cend());
  for (auto it : clear_me) {
    auto [from, to, weight] = it;
    std::cout << "[" << from << ", " << to << ", " << weight << "]\n";
  }

  std::cout << "test mass erasing then re-printing\n";
  clear_me.erase(1.1, 1.1, -1);
  clear_me.erase(1.1, 1.1, 1);
  clear_me.erase(2.2, 1.1, 1);
  clear_me.erase(2.2, 2.2, 2);
  clear_me.erase(2.2, 3.3, 343);
  clear_me.erase(3.3, 3.3, 300);
  for (auto it : clear_me) {
    auto [from, to, weight] = it;
    std::cout << "[" << from << ", " << to << ", " << weight << "]\n";
  }
  

  std::cout << "test mass erasing, printing, readding, printing\n";
  clear_me.InsertEdge(1.1, 1.1, -1);
  clear_me.InsertEdge(1.1, 1.1, 1);
  clear_me.InsertEdge(2.2, 1.1, 1);
  clear_me.InsertEdge(2.2, 3.3, 343);
  clear_me.InsertEdge(3.3, 3.3, 300);
  for (auto it = clear_me.begin(); it != clear_me.end(); ++it) {
    if (std::get<0>(*it) == 2.2 && std::get<1>(*it) == 1.1)  {
      auto new_it = clear_me.erase(it);
      for (; new_it != clear_me.end(); ++new_it) {
        auto [from, to, weight] = *new_it;
        std::cout << "[" << from << ", " << to << ", " << weight << "]\n";
      }
      break;
    }
    auto [from, to, weight] = *it;
    std::cout << "[" << from << ", " << to << ", " << weight << "]\n";
  }

  std::cout << "\n";
  for (auto it : clear_me) {
    auto [from, to, weight] = it;
    std::cout << "[" << from << ", " << to << ", " << weight << "]\n";
  }

  std::cout << "testing -- operator for iterator\n";
  int i = 1;
  for (auto it = clear_me.begin(); it != clear_me.end(); ++it, ++i) {
    if (i == 7) {
      std::cout << "iterator going backwards one! current = ";
      auto [from, to, weight] = *it;
      std::cout << "[" << from << ", " << to << ", " << weight << "]\n";
      --it;
      auto [from2, to2, weight2] = *it;
      std::cout << "[" << from2 << ", " << to2 << ", " << weight2 << "]\n";
    } else {
      std::cout << "iterator going forwards one! current = ";
      auto [from, to, weight] = *it;
      std::cout << "[" << from << ", " << to << ", " << weight << "]\n";
    }
  }

  std::cout << "testing graph equality comparisons\n";
  gdwg::Graph<double, int> lhs {di_tuple.begin(), di_tuple.end()};
  gdwg::Graph<double, int> rhs {di_tuple.begin(), di_tuple.end()};
  std::cout << "should be equal: " << (lhs == rhs) << ", " << (lhs != rhs) << "\n";
  rhs.erase(4.2, 2.3, 3);
  std::cout << "should not be equal: " << (lhs == rhs) << ", " << (lhs != rhs) << "\n";
  rhs.InsertEdge(4.2, 2.3, 3);
  std::cout << "should be equal: " << (lhs == rhs) << ", " << (lhs != rhs) << "\n";
  

  std::cout << lhs;

  std::cout << "\nnow testing replace\nprinting before replace\n";
  clear_me.InsertNode(1.1);
  clear_me.InsertNode(2.2);
  clear_me.InsertNode(3.3);
  clear_me.InsertNode(4.4);
  clear_me.InsertNode(5.5);
  clear_me.InsertEdge(1.1, 1.1, 1);
  clear_me.InsertEdge(1.1, 1.1, 2);
  clear_me.InsertEdge(1.1, 1.1, -1);
  clear_me.InsertEdge(2.2, 1.1, 1);
  clear_me.InsertEdge(2.2, 2.2, 2);
  clear_me.InsertEdge(3.3, 2.2, 3);
  clear_me.InsertEdge(3.3, 3.3, 300);
  clear_me.InsertEdge(1.1, 3.3, 60);
  clear_me.InsertEdge(2.2, 3.3, 343);
  clear_me.InsertEdge(4.4, 1.1, 4);
  clear_me.InsertEdge(4.4, 4.4, 16);
  clear_me.InsertEdge(2.2, 4.4, 8);
  for (auto it : clear_me) {
    auto [from, to, weight] = it;
    std::cout << "[" << from << ", " << to << ", " << weight << "]\n";
  }
  std::cout << "now replacing 2.2 with 99.9\n";
  clear_me.Replace(2.2, 99.9);
  for (auto it : clear_me) {
    auto [from, to, weight] = it;
    std::cout << "[" << from << ", " << to << ", " << weight << "]\n";
  }

  std::cout << clear_me;

  std::cout << "testing merge replace!!\n";
  gdwg::Graph<char, int> ci_mr_g {};
  ci_mr_g.InsertNode('a');
  ci_mr_g.InsertNode('b');
  ci_mr_g.InsertNode('c');
  ci_mr_g.InsertNode('d');
  ci_mr_g.InsertEdge('a', 'b', 3);
  ci_mr_g.InsertEdge('c', 'b', 2);
  ci_mr_g.InsertEdge('d', 'b', 4);
  std::cout << ci_mr_g;

  std::cout << "merging now\n";
  ci_mr_g.MergeReplace('b', 'a');
  std::cout << ci_mr_g;

  std::cout << "testing merge replace variant 2!!\n";
  gdwg::Graph<char, int> ci_mr_g2 {};
  ci_mr_g2.InsertNode('a');
  ci_mr_g2.InsertNode('b');
  ci_mr_g2.InsertNode('c');
  ci_mr_g2.InsertNode('d');
  ci_mr_g2.InsertEdge('b', 'a', 3);
  ci_mr_g2.InsertEdge('b', 'b', 6);
  ci_mr_g2.InsertEdge('b', 'c', 2);
  ci_mr_g2.InsertEdge('b', 'd', 4);
  std::cout << ci_mr_g2;

  std::cout << "merging now\n";
  ci_mr_g2.MergeReplace('b', 'a');
  std::cout << ci_mr_g2;
}
*/
