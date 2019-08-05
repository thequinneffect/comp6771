#ifndef ASSIGNMENTS_DG_GRAPH_H_
#define ASSIGNMENTS_DG_GRAPH_H_

#include <memory>
#include <set>
#include <tuple>
#include <type_traits>
#include <vector>

namespace gdwg {

template <typename N, typename E>
class Graph {

  private:
    /* forward declarations so iterator knows about these types */
    struct Edge;
    struct Node;

  public:
    class const_iterator {
      public:
        using iterator_category = std::bidirectional_iterator_tag;
        using value_type = std::tuple<N,N,E>;
        using reference = std::tuple<const N&, const N&, const E&>;

        // may have to double deref these things because they are iterators to smart ptrs to things
        // 1 deref gives you smart ptr, 2 deref gives you thing
        reference operator*() const;
        const_iterator& operator++();
        const_iterator operator++(int) {
          auto copy{*this};
          ++(*this);
          return copy;
        }

        friend bool operator==(const const_iterator& lhs, const const_iterator& rhs) {
          return lhs.node_it_ == rhs.node_it_ && (lhs.node_it_ == lhs.node_end_it_ || lhs.edge_it_ == rhs.edge_it_);
        }
        friend bool operator!=(const const_iterator& lhs, const const_iterator& rhs) { return !(lhs == rhs); }

      private:

        typename std::set<std::unique_ptr<Node>>::iterator node_it_;
        typename std::set<std::weak_ptr<Edge>>::iterator edge_it_;
        const typename std::set<std::unique_ptr<Node>>::iterator node_end_it_;

        friend class Graph;
        const_iterator(
          const decltype(node_it_)& node_it, 
          const decltype(edge_it_)& edge_it,
          const decltype(node_end_it_)& node_end_it)
          : node_it_{node_it}
          , edge_it_{edge_it}
          , node_end_it_{node_end_it} {
            //std::cout << "const_iterator ctor called!\n";
          }
    };
    
    
    Graph()=default;
    Graph(
      typename std::vector<N>::const_iterator b,
      typename std::vector<N>::const_iterator e
    );

    Graph(
      typename std::vector<std::tuple<N, N, E>>::const_iterator b,
      typename std::vector<std::tuple<N, N, E>>::const_iterator e
    );

    /* methods */
    bool InsertNode(const N& val);
    bool InsertEdge(const N& src, const N& dst, const E& w);
    bool IsNode(const N& val) const;

    const_iterator find(const N&, const N&, const E&);

    /* iterator methods */
    /* auto generate our reverse iterator from normal iterator */
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;
    const_iterator cbegin();
    const_iterator cend();
    const_iterator begin() { return cbegin(); };
    const_iterator end() { return cend(); };
    const_reverse_iterator crbegin();
    const_reverse_iterator crend();
    const_reverse_iterator rbegin();
    const_reverse_iterator rend();

  private:

    /* R is a resource (N or E), smart_ptr is any of the smart
    pointers to one of the wrapper structs (Node or Edge) */
    template <typename R, typename smart_ptr>
    struct WrapperComp {
        using is_transparent = R;
        bool operator()(const smart_ptr& a, const smart_ptr& b) const {
          if constexpr (std::is_same<smart_ptr, std::weak_ptr<Edge>>::value) {
            auto a_sp = a.lock();
            auto b_sp = b.lock();
            return (*a_sp).value_ < (*b_sp).value_;
          } else {
            return (*a).value_ < (*b).value_;
          }
        }
        bool operator()(const smart_ptr& a, const R& value) const {
          if constexpr (std::is_same<smart_ptr, std::weak_ptr<Edge>>::value) {
            auto a_sp = a.lock();
            return (*a_sp).value_ < value;
          } else {
            return (*a).value_ < value;
          }
        }
        bool operator()(const R& value, const smart_ptr& b) const {
          if constexpr (std::is_same<smart_ptr, std::weak_ptr<Edge>>::value) {
            auto b_sp = b.lock();
            return value < (*b_sp).value_;
          } else {
            return value < (*b).value_;
          }
        }
      };

  /*  */
    struct Node {

      Node(N val) : value_{val} {};
      /* the set of 0..* incoming edges */
      std::set<std::weak_ptr<Edge>, WrapperComp<E, std::weak_ptr<Edge>>> incoming_;
      /* the set of 0..* outgoing edges */
      std::set<std::weak_ptr<Edge>, WrapperComp<E, std::weak_ptr<Edge>>> outgoing_;
      N value_;
    };

    /* */
    struct Edge { 

      Edge(Node* src, Node* dst, E value)
        : src_{src}
        , dst_{dst}
        , value_{value} {}
      Node* src_; // exactly 1
      Node* dst_; // exactly 1
      E value_;
    };

    /* owns all nodes */
    std::set<std::unique_ptr<Node>, WrapperComp<N, std::unique_ptr<Node>>> nodes_;
    /* owns all edges */
    std::set<std::shared_ptr<Edge>, WrapperComp<E, std::shared_ptr<Edge>>> edges_;
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