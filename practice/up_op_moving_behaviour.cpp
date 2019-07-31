#include <memory>
#include <vector>
#include <string>

    
struct Node {

    Node(std::string n) : value_ {n} {}

    std::vector<Edge*> incoming_; // 0..*
    std::vector<Edge*> outgoing_; // 0..*
    std::string value_; // exactly 1
};
    
struct Edge {

    Edge(int w) : value_{w} {}

    Node* src_;
    Node* dst_;
    int value_;
};

class Graph {

  public:
    Graph() = default;
    Graph(std::string n) {
        nodes_.push_back(std::make_unique<Node>(n));
    }

    struct Node MakeNode(std::string n) {
        nodes_.push_back(std::make_unique<Node>(n));
    }

    struct Edge MakeEdge(Node n1, Node n2, int e) {

    }

    void MakeEdge(std::string n1, std::string n2, int e) {
        Node n1_ = MakeNode(n1);
        Node n2_ = MakeNode(n2);
        MakeEdge(n1_, n2_, e);
    }

  private:



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