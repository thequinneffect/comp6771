#ifndef ASSIGNMENTS_DG_GRAPH_H_
#define ASSIGNMENTS_DG_GRAPH_H_

#include <experimental/memory>
#include <map>
#include <memory>
#include <set>
#include <tuple>
#include <unordered_map>

namespace gdwg {

template <typename N, typename E>
class Graph {

  public:
    class const_iterator {};

    /*  */
    struct Edge;
    struct Node {

      struct NodeComp {
        using is_transparent = void;
        bool operator()(const std::unique_ptr<Graph::Node>& a, const std::unique_ptr<Graph::Node>& b) const {
          return (*a).value_ < (*b).value_;
        }
        bool operator()(const std::unique_ptr<Graph::Node>& a, const N& value) const {
          return (*a).value_ < value;
        }
        bool operator()(const N& value, const std::unique_ptr<Graph::Node>& b) const {
          return value < (*b).value_;
        }
      };

      Node(N val) : value_{val} {};
      /* the set of 0..* incoming edges */
      std::set<std::weak_ptr<Edge>> incoming_;
      /* the set of 0..* outgoing edges */
      std::set<std::weak_ptr<Edge>> outgoing_;
      N value_;
    };

    /* */
    struct Edge { 
      
      struct EdgeComp {
        using is_transparent = void;
        bool operator()(const std::shared_ptr<Graph::Edge>& a, const std::shared_ptr<Graph::Edge>& b) const {
          return (*a).weight_ < (*b).weight_;
        }
        bool operator()(const std::shared_ptr<Graph::Edge>& a, const E& value) const {
          return (*a).weight_ < value;
        }
        bool operator()(const E& value, const std::shared_ptr<Graph::Edge>& b) const {
          return value < (*b).weight_;
        }
      };

      Node* src_; // exactly 1
      Node* dst_; // exactly 1
      E weight_;
    };
    
    Graph()=default;
    Graph(
      typename std::vector<N>::const_iterator b,
      typename std::vector<N>::const_iterator e
    );

    /* methods */
    bool InsertNode(const N& val);

  private:

    /* owns all nodes */
    std::set<std::unique_ptr<Node>, typename Node::NodeComp> nodes_;
    /* owns all edges */
    std::set<std::shared_ptr<Edge>, typename Edge::EdgeComp> edges_;
};

}  // namespace gdwg

#include "assignments/dg/graph.tpp"

#endif  // ASSIGNMENTS_DG_GRAPH_H_

/*

  01. InsertNode : requires fast look-up of whether or not Node(val) exists or not [/]
    - std::map<N, std::unique_ptr<Node>> nodes_;
  02. InsertEdge : requires fast look-up of whether Edge(N1,N2,W) exists or not
    - map<tuple<N,N,E>, E> edges_; 
  03. DeleteNode : requires fast look-up of target node (see 1), then for each edge
  on that node it requires fast look-up of the edge (see 2)
    - note: the node itself cannot own all its edges (because it has incoming and
    outgoing edges so that would lead to dual ownership, so a third party structure
    own them)
    - loop through incoming_ and outgoing_, these provide you with a {src, dst, w} to
    look up edges_ with and delete it if it exists. Then just delete the node.
  04. Replace : see 1
  05. MergeReplace : need a fast way of finding edges between src and dst
    - map<N, set<E>> incoming/outgoing; this allows us to seach src outgoing with dst
    as key, and src incoming with dst as key
  06. Clear : just empty the owning containers nodes_ and edges_. Everything is then
  destructed and gone.
  07. IsNode : see 1
  08. IsConnected : see 2
  09. GetNodes : nodes_ is implicitly ordered as it's a map
  10. GetConnected :returns a sorted (increasing order) vector of nodes connected
  to the src node from outgoing edges
    - map<N, set<E>> outgoing_; i.e. can loop through and grab keys (N) of this map
    and these are the nodes. They are also implicitly ordered
  11. GetWeights : returns a sorted (increasing order) vector of weights from src -> dst
    - map<N, set<E>> outgoing_; i.e. access src node, instantiate vector from src nodes 
    outgoing set for dest node N
  12. find :
  13. erase :

 */