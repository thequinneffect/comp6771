#include <algorithm>
#include <map>
#include <memory>
#include <set>
#include <iostream>

template <typename T>
struct ptr_less {
  bool operator()(T* lhs, T* rhs) {
    return (*lhs).value_ < (*rhs).value;
  }
};

struct Graph {
    
    struct Node {
      struct NodeComp {
        using is_transparent = void;
        bool operator()(const std::unique_ptr<Graph::Node>& a, const std::unique_ptr<Graph::Node>& b) const {
          return (*a).data_ < (*b).data_;
        }
        bool operator()(const std::unique_ptr<Graph::Node>& a, const int& value) const {
          return (*a).data_ < value;
        }
        bool operator()(const int& value, const std::unique_ptr<Graph::Node>& b) const {
          return value < (*b).data_;
        }
      };
        Node(int data) : data_{data} {}
        int data_;
    };

    std::set<std::unique_ptr<Node>, Graph::Node::NodeComp> nodes_;
};



int main(void) {

    Graph g {};
    g.nodes_.insert(std::make_unique<Graph::Node>(6));
    auto it = g.nodes_.find(6);
    std::cout << (**it).data_ << "\n";
    

}