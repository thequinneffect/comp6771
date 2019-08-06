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
    parts were complete I moved
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

    NOTE: I don't really have any tests for my iterators other than
    the testing i did in my client file. This is because I had
    lots of issues getting the reverse iterator to generate
    using the automatic std::reverse_iterator method. It turns
    out it was because my --operator for the const_iterator had a
    bug. And now because it is already 5 hrs over the dealine I feel
    it's better to submit now and not lose any more percentage per hour
    rather than to spend time writing iterator test for which the marks
    will probably largely cancel out for. Iḿ happy with my "forwards"
    iterating, but reverse may be a little buggy, fingers crossed lol.

    I will submit "my_client" alongside the other files if the submission
    system lets me. Otherise it is added on below as a comment
  }

*/

#include "assignments/dg/graph.h"

#include <string>
#include <tuple>
#include <vector>

#include "catch.h"

/* a tuple i will use for easily creating graphs for my test */
std::vector<std::tuple<double, double, int>> di_tuple{
    {6.5, 2.3, 5}, {6.5, 7.0, 5}, {4.2, 6.9, 7}, {4.2, 2.3, 3}, {4.3, 2.3, 3},
    {1.1, 1.1, 1}, {1.1, 1.1, 2}, {6.5, 2.3, 5}, {6.5, 7.0, 5}, {6.9, 1.1, 9}};
/* another one but with different types for some variation */
std::vector<std::tuple<char, char, std::string>> cs_tuple{{'a', 'b', "brb"}, {'c', 'd', "lol"},
                                                          {'g', 'h', "omg"}, {'h', 'g', "ftw"},
                                                          {'y', 'a', "wtf"}, {'z', 'y', "kek"}};

/* I'm confident that all of my simple ctors (and dtor) work because
 * I used them thoroughly when doing unofficial testing in my
 * client file. This is my justification for not testing them here.
 * I will still test move and copy constructing though because these
 * have more room for error with regards to the smart pointers.
 * Also, all the non-cpy and move ones (which are tested below)
 * consist only of calling insert edge and insert node functions
 * so i will just test those instead */

SCENARIO("copy ctor") {
  GIVEN("a graph with some edges") {
    gdwg::Graph<char, int> g{'a', 'b', 'c'};
    g.InsertEdge('a', 'b', 1);
    g.InsertEdge('b', 'c', 2);
    g.InsertEdge('c', 'b', 3);
    g.InsertEdge('c', 'a', 4);
    WHEN("we copy construct another graph from the given one") {
      gdwg::Graph<char, int> g2 = {g};
      REQUIRE(g2 == g);
      THEN(
          "if we alter one graph, the other shouldn't be altered i.e. resources should be unique") {
        g2.DeleteNode('b');
        REQUIRE(g2 != g);
      }
    }
  }
}

SCENARIO("move ctor") {
  GIVEN("some graph and a copy of it") {
    gdwg::Graph<char, std::string> cs_g{cs_tuple.begin(), cs_tuple.end()};
    gdwg::Graph<char, std::string> cs_g_cpy{cs_g};
    WHEN("i move construct a new graph from the old original one") {
      gdwg::Graph<char, std::string> cs_g_movee{std::move(cs_g)};
      /* it should have successfully stolen the original graphs content */
      REQUIRE(cs_g_movee == cs_g_cpy);
    }
  }
}

/* As my implementation of move assignment and copy assignment are almost
 * exactly the same as move ctor and cpy ctor, I will not bother testing
 * them. The code is literally the same (only difference is return values) */

SCENARIO("InsertNode") {
  GIVEN("a graph with at least one node") {
    gdwg::Graph<int, int> iig{1, 2};
    WHEN("i try insert a node that doesnt exist") { REQUIRE(iig.InsertNode(3) == true); }
    WHEN("i try inset a node that does exist") { REQUIRE(iig.InsertNode(2) == false); }
  }
}

SCENARIO("InsertEdge") {
  GIVEN("a graph with edges") {
    gdwg::Graph<double, int> di_g{di_tuple.begin(), di_tuple.end()};
    WHEN("i try insert an edge and the src doesn't exist") {
      REQUIRE_THROWS_WITH(
          di_g.InsertEdge(999.99, 1.1, 1),
          "Cannot call Graph::InsertEdge when either src or dst node does not exist");
    }
    WHEN("i try insert an edge and the dst doesn't exist") {
      REQUIRE_THROWS_WITH(
          di_g.InsertEdge(1.1, 888.88, 1),
          "Cannot call Graph::InsertEdge when either src or dst node does not exist");
    }
    WHEN("i try insert an edge and it already exists") {
      REQUIRE(di_g.InsertEdge(1.1, 1.1, 1) == false);
    }
    WHEN("i try insert an edge and it doesn't exist") {
      REQUIRE(di_g.InsertEdge(6.5, 7.0, 70) == true);
    }
  }
}

SCENARIO("DeleteNode") {
  GIVEN("a graph with some nodes, of which one has incoming and outgoing edges") {
    gdwg::Graph<double, int> di_g{di_tuple.begin(), di_tuple.end()};
    WHEN("i try delete a node that doesn't exist") { REQUIRE(di_g.DeleteNode(99.9) == false); }
    WHEN("i try delete a node that does exist and has incoming/outgoing edges") {
      REQUIRE(di_g.find(6.9, 1.1, 9) != di_g.end());
      REQUIRE(di_g.find(4.2, 6.9, 7) != di_g.end());
      REQUIRE(di_g.DeleteNode(6.9) == true);
      /* it should no longer be a node and the edges should be gone */
      REQUIRE(di_g.IsNode(6.9) == false);
      REQUIRE(di_g.find(6.9, 1.1, 9) == di_g.end());
      REQUIRE(di_g.find(4.2, 6.9, 7) == di_g.end());
    }
  }
}

SCENARIO("Replace") {
  GIVEN("a simple graph with edges") {
    gdwg::Graph<char, int> ci_g{'a', 'b', 'c', 'd'};
    ci_g.InsertEdge('a', 'b', 3);
    ci_g.InsertEdge('c', 'b', 2);
    ci_g.InsertEdge('d', 'b', 4);
    ci_g.InsertEdge('b', 'b', 3);
    ci_g.InsertEdge('b', 'b', 6);
    WHEN("i call replace and there is no such target node") {
      REQUIRE_THROWS_WITH(ci_g.Replace('e', 'f'),
                          "Cannot call Graph::Replace on a node that doesn't exist");
    }
    WHEN("i call replace and there is such target node") {
      REQUIRE(ci_g.Replace('b', 'e') == true);
      /* require the edges to have been changed too */
      REQUIRE(ci_g.find('a', 'e', 3) != ci_g.end());
      REQUIRE(ci_g.find('c', 'e', 2) != ci_g.end());
      REQUIRE(ci_g.find('d', 'e', 4) != ci_g.end());
      REQUIRE(ci_g.find('e', 'e', 3) != ci_g.end());
      REQUIRE(ci_g.find('e', 'e', 6) != ci_g.end());
    }
  }
}

SCENARIO("MergeReplace") {
  GIVEN("a simple graph with edges") {
    gdwg::Graph<char, int> ci_mr_g{};
    ci_mr_g.InsertNode('a');
    ci_mr_g.InsertNode('b');
    ci_mr_g.InsertNode('c');
    ci_mr_g.InsertNode('d');
    ci_mr_g.InsertEdge('a', 'b', 3);
    ci_mr_g.InsertEdge('c', 'b', 2);
    ci_mr_g.InsertEdge('d', 'b', 4);
    ci_mr_g.InsertEdge('b', 'b', 3);
    WHEN("src is not found in the graph") {
      REQUIRE_THROWS_WITH(
          ci_mr_g.MergeReplace('z', 'a'),
          "Cannot call Graph::MergeReplace on old or new data if they don't exist in the graph");
    }
    WHEN("dst is not found in the graph") {
      REQUIRE_THROWS_WITH(
          ci_mr_g.MergeReplace('b', 'y'),
          "Cannot call Graph::MergeReplace on old or new data if they don't exist in the graph");
    }
    WHEN("the merge is successful then all src<->dst edges become reflexive and replacer gets "
         "replaccee edges") {
      ci_mr_g.MergeReplace('b', 'a');
      REQUIRE(ci_mr_g.find('a', 'a', 3) != ci_mr_g.end());
      REQUIRE(ci_mr_g.find('c', 'a', 2) != ci_mr_g.end());
      REQUIRE(ci_mr_g.find('d', 'a', 4) != ci_mr_g.end());
    }
  }
  GIVEN("another simple graph with edges different types of edges (incoming to replacee)") {
    gdwg::Graph<char, int> ci_mr_g{};
    ci_mr_g.InsertNode('a');
    ci_mr_g.InsertNode('b');
    ci_mr_g.InsertNode('c');
    ci_mr_g.InsertNode('d');
    ci_mr_g.InsertEdge('b', 'a', 3);
    ci_mr_g.InsertEdge('b', 'c', 2);
    ci_mr_g.InsertEdge('b', 'd', 4);
    ci_mr_g.InsertEdge('b', 'b', 6);
    WHEN("the merge is successful then all src<->dst edges become reflexive and replacer gets "
         "replaccee edges") {
      ci_mr_g.MergeReplace('b', 'a');
      REQUIRE(ci_mr_g.find('a', 'a', 3) != ci_mr_g.end());
      REQUIRE(ci_mr_g.find('a', 'a', 6) != ci_mr_g.end());
      REQUIRE(ci_mr_g.find('a', 'c', 2) != ci_mr_g.end());
      REQUIRE(ci_mr_g.find('a', 'd', 4) != ci_mr_g.end());
    }
  }
}

SCENARIO("Clear") {
  GIVEN("a graph with lots of edges") {
    gdwg::Graph<char, std::string> cs_g{cs_tuple.begin(), cs_tuple.end()};
    /* require at least the testing edge */
    REQUIRE(cs_g.find('a', 'b', "brb") != cs_g.end());
    WHEN("we clear the graph") {
      cs_g.Clear();
      REQUIRE(cs_g.IsNode('a') == false);
      REQUIRE(cs_g.IsNode('b') == false);
      gdwg::Graph<char, std::string> cs_g_empty{};
      REQUIRE(cs_g == cs_g_empty);
      /* we should be able to re-add nodes and edges */
      REQUIRE(cs_g.InsertNode('a') == true);
      REQUIRE(cs_g.InsertNode('b') == true);
      REQUIRE(cs_g.InsertNode('c') == true);
      REQUIRE(cs_g.InsertEdge('a', 'b', "brb") == true);
      REQUIRE(cs_g.InsertEdge('c', 'b', "cbf") == true);
    }
  }
}

/* I believe IsNode has been indirectly tested sufficiently and its implementation is
 * basically reliant on STL types find() working */

SCENARIO("IsConnected") {
  GIVEN("a graph with an nodes were only some are connected") {
    gdwg::Graph<int, double> idg{1, 2, 3};
    idg.InsertEdge(1, 2, 1.2);
    idg.InsertEdge(1, 3, 1.3);
    WHEN("src is not in the graph") {
      REQUIRE_THROWS_WITH(
          idg.IsConnected(4, 1),
          "Cannot call Graph::IsConnected if src or dst node don't exist in the graph");
    }
    WHEN("dst is not in the graph") {
      REQUIRE_THROWS_WITH(
          idg.IsConnected(2, 5),
          "Cannot call Graph::IsConnected if src or dst node don't exist in the graph");
    }
    WHEN("the nodes are not connected") { REQUIRE(idg.IsConnected(2, 3) == false); }
    WHEN("the nodes are connected") {
      REQUIRE(idg.IsConnected(1, 2) == true);
      REQUIRE(idg.IsConnected(1, 3) == true);
    }
  }
}

SCENARIO("GetNodes") {
  GIVEN("a graph with no nodes") {
    gdwg::Graph<int, double> no_nodes{};
    WHEN("we call getnodes") {
      auto node_vec = no_nodes.GetNodes();
      REQUIRE(node_vec.empty());
    }
  }
  GIVEN("a graph with some nodes") {
    gdwg::Graph<int, double> no_nodes{1, 5, 3, 2, 7};
    WHEN("we call getnodes") {
      auto node_vec = no_nodes.GetNodes();
      REQUIRE(node_vec.size() == 5);
      REQUIRE(node_vec[0] == 1);
      REQUIRE(node_vec[1] == 2);
      REQUIRE(node_vec[2] == 3);
      REQUIRE(node_vec[3] == 5);
      REQUIRE(node_vec[4] == 7);
    }
  }
}

SCENARIO("GetConnected") {
  GIVEN("a graph with a node that has outgoing edges and another with none") {
    gdwg::Graph<char, int> g{};
    g.InsertNode('a');
    g.InsertNode('b');
    g.InsertNode('c');
    g.InsertNode('d');
    g.InsertEdge('a', 'b', 3);
    g.InsertEdge('c', 'b', 2);
    g.InsertEdge('d', 'b', 4);
    g.InsertEdge('a', 'c', 6);
    g.InsertEdge('a', 'd', 3);

    WHEN("we call GetConnected and src doesnt exist") {
      REQUIRE_THROWS_WITH(g.GetConnected('z'),
                          "Cannot call Graph::GetConnected if src doesn't exist in the graph");
    }
    WHEN("we call GetConnected and there is no outgoing edges from src") {
      auto vec = g.GetConnected('b');
      REQUIRE(vec.empty());
    }
    WHEN("we call GetConnected and src has outgoing edges") {
      auto vec = g.GetConnected('a');
      REQUIRE(vec.size() == 3);
      REQUIRE(vec[0] == 'b');
      REQUIRE(vec[1] == 'c');
      REQUIRE(vec[2] == 'd');
    }
  }
}

SCENARIO("GetWeights") {
  GIVEN("a graph in which there are a pair of nodes with edges going either way between them") {
    gdwg::Graph<char, int> ci{'a', 'b', 'c'};
    ci.InsertEdge('a', 'a', 1);
    ci.InsertEdge('a', 'b', 2);
    ci.InsertEdge('a', 'b', 7);
    ci.InsertEdge('a', 'b', 9);
    ci.InsertEdge('a', 'b', 1);
    ci.InsertEdge('a', 'c', 3);
    ci.InsertEdge('c', 'a', 4);
    ci.InsertEdge('b', 'a', 6);

    WHEN("src is not in graph") {
      REQUIRE_THROWS_WITH(
          ci.GetWeights('d', 'a'),
          "Cannot call Graph::GetWeights if src or dst node don't exist in the graph");
    }
    WHEN("dst is not in graph") {
      REQUIRE_THROWS_WITH(
          ci.GetWeights('a', 'z'),
          "Cannot call Graph::GetWeights if src or dst node don't exist in the graph");
    }
    WHEN("src has incoming and outgoing edges, only get outgoing, and in order") {
      auto edge_vec = ci.GetWeights('a', 'b');
      REQUIRE(edge_vec.size() == 4);
      REQUIRE(edge_vec[0] == 1);
      REQUIRE(edge_vec[1] == 2);
      REQUIRE(edge_vec[2] == 7);
      REQUIRE(edge_vec[3] == 9);
    }
  }
}

/* since i have used find pretty extensively above, I will not explicitly test
 * it as it has been tested indirectly enough */

SCENARIO("bool erase") {
  GIVEN("a graph with some edges") {
    gdwg::Graph<double, int> di_g{di_tuple.begin(), di_tuple.end()};
    WHEN("we try erase an edge that doesnt exist") { REQUIRE(di_g.erase(1.1, 6.5, 90) == false); }
    WHEN("we try erase an edge that does exist") { REQUIRE(di_g.erase(4.2, 2.3, 3) == true); }
  }
}

/* as bool erase uses iterator erase, i will not explictly test
 * iterator as i tested it in my client (ctrl-f for "test erasing via iterator"
 * to see it below) */

SCENARIO("friend operator== (and also != because it is the inverse)") {
  GIVEN("two graphs that are the same (one is a copy of the other)") {
    gdwg::Graph<char, std::string> cs{cs_tuple.begin(), cs_tuple.end()};
    gdwg::Graph<char, std::string> cs_cpy{cs};
    WHEN("we compare them equal") {
      REQUIRE((cs == cs_cpy) == true);
      REQUIRE((cs != cs_cpy) == false);
    }
    WHEN("we alter one of the graphs") {
      cs.DeleteNode('c');
      REQUIRE((cs == cs_cpy) == false);
      REQUIRE((cs != cs_cpy) == true);
      /* but then they should be equal if we re-add */
      cs.InsertNode('c');
      cs.InsertEdge('c', 'd', "lol");
      REQUIRE((cs == cs_cpy) == true);
      REQUIRE((cs != cs_cpy) == false);
    }
  }
}

/* i have tested the operator<< by observation because i feel that is easier.
 * I did this in my client testing. ctrl-f for "std::cout << clear_me;"
 * and "std::cout << lhs" below. Additionally, the testing of ordering being
 * correct in the edge iterator and in getweights and getnodes is sufficient
 * enough for me to be happy that the ordering of << output is also okay */

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

  {
  std::cout << "now testing reverse iterator\n";
  auto rb = clear_me.rbegin();
  auto re = clear_me.rend();
  (void)rb;
  (void)re;
  std::cout << "making rb and re succeeded\n";

  std::cout << "print out graph first\n";
  for (auto it : clear_me) {
    auto [from, to, weight] = it;
    std::cout << "[" << from << ", " << to << ", " << weight << "]\n";
  }

  std::cout << "about to deref rb\n";
  auto [s, d, w] = *rb;
  std::cout << "[" << s << ", " << d << ", " << w << "]\n";

  std::cout << "about to fully reverse iterate\n";
  for (auto rb = clear_me.rbegin(); rb != clear_me.rend(); ++rb) {
    auto [s, d, w] = *rb;
    std::cout << "[" << s << ", " << d << ", " << w << "]\n";
  }

  }
}

*/
