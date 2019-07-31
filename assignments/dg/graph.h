#ifndef ASSIGNMENTS_DG_GRAPH_H_
#define ASSIGNMENTS_DG_GRAPH_H_

#include <memory>
#include <vector>

namespace gdwg {

template <typename N, typename E> // could be for example "string, int", "int, double", "string, string" etc.
class Graph {
  public:
    class const_iterator {};
    

  private:

    /* represents a node graphically. As nodes are unique, I store
     * the resource on this Node object (which is owned by a 
     * unique_ptr). This is so that when the node is deleted, so is
     * the resource. I chose to solely store observer_ptrs to Edge
     * objects rather than other Nodes because an Edge has to store
     * the src->dst relationship anyway. */
    class Node {
        std::vector<std::observer_ptr<Edge>> incoming_; // 0..*
        std::vector<std::observer_ptr<Edge>> outgoing_; // 0..*
        N value_; // exactly 1
    };

    /* represents an edge graphically. As edges are also unique, I
     * store the resource for the edge on this Edge object which is
     * owned by a unique_ptr (for same reasons as above). Each edge
     * also has exactly one src Node and dst Node, and because these
     * resources are already owned by unique_ptr's, I just use 
     * observer_ptrs as references (they can re-point when the 
     * underlying heap memory moves, refs cannot) */
    class Edge {
        std::observer_ptr<Node> src_; // exactly 1
        std::observer_ptr<Node> dst_; // exactly 1
        E weight_; // exactly 1
    };

    /* owns all nodes */
    std::vector<std::unique_ptr<Node>> nodes_;
    /* owns all edges */
    std::vector<std::unique_ptr<Edge>> edges_;
    
};

}  // namespace gdwg

#include "assignments/dg/graph.tpp"

#endif  // ASSIGNMENTS_DG_GRAPH_H_
