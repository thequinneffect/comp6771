#include <memory>
#include <vector>
#include <string>

class Graph {

  public:
    Graph() = default;
    Graph(std::string n) {
        nodes_.push_back(std::make_unique<Node>(n));
    }

  private:

    struct Edge;
    struct Node {

        Node(std::string n) : value_ {n} {}

        std::vector<Edge*> incoming_; // 0..*
        std::vector<Edge*> outgoing_; // 0..*
        std::string value_; // exactly 1
    };
    
    struct Edge {
        Node* src_;
        Node* dst_;
        int value_;
    };

    /* owns all nodes */
    std::vector<std::unique_ptr<Node>> nodes_;
    /* owns all edges */
    std::vector<std::unique_ptr<Edge>> edges_;

};

int main(void) {

    Graph e_g {};
    std::string n1 = "node1";
    Graph g1 {n1};
    

    return 0;

}