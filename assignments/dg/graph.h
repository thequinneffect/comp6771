#ifndef ASSIGNMENTS_DG_GRAPH_H_
#define ASSIGNMENTS_DG_GRAPH_H_

#include <experimental/memory>
#include <map>
#include <memory>
#include <unordered_set>
#include <unordered_map>

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
    struct Node {
        /* i've chosen sets rather than vectors so that removal is
         * nicer in that we don't have wasted slots */
        std::unordered_set<std::weak_ptr<Edge>> incoming_; // 0..*
        std::unordered_set<std::weak_ptr<Edge>> outgoing_; // 0..*
        N value_; // exactly 1
    };

    /* represents an edge graphically. As edges are also unique, I
     * store the resource for the edge on this Edge object which is
     * owned by a unique_ptr (for same reasons as above). Each edge
     * also has exactly one src Node and dst Node, and because these
     * resources are already owned by unique_ptr's, I just use 
     * observer_ptrs */
    struct Edge {
        std::experimental::observer_ptr<Node> src_; // exactly 1
        std::experimental::observer_ptr<Node> dst_; // exactly 1
        E weight_; // exactly 1
    };

    /* owns all nodes */
    /* By storing the nodes in a map which is indexed by the node
     * value N, we can look-up whether or not a node exists in
     * the graph in amortised constant time. 
     * - ordered so that GetNodes is easy */
    std::map<N, std::unique_ptr<Node>> nodes_;
    //std::vector<std::unique_ptr<Node>> nodes_;
    /* owns all edges */
    /* - Map of N to Edge so that InsertEdge is easy
     * By storing the edges in a map which is indexed by the source
     * node, we only need to check whether or not the source node
     * exists or not (amortised constant look-up), and then check 
     * it's edges if it does (linear in number of outgoing edges) */
    std::unordered_map<N, std::shared_ptr<Edge>> edges_;
    //std::vector<std::unique_ptr<Edge>> edges_;
    
};

}  // namespace gdwg

#include "assignments/dg/graph.tpp"

#endif  // ASSIGNMENTS_DG_GRAPH_H_
