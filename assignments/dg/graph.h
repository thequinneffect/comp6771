#ifndef ASSIGNMENTS_DG_GRAPH_H_
#define ASSIGNMENTS_DG_GRAPH_H_

#include <initializer_list>
#include <memory>
#include <set>
#include <tuple>
#include <type_traits>
#include <utility>
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
    using value_type = std::tuple<N, N, E>;
    using reference = std::tuple<const N&, const N&, const E&>;
    using pointer = void;
    using difference_type = int;

    reference operator*() const;
    const_iterator& operator++();
    const_iterator operator++(int) {
      auto copy{*this};
      ++(*this);
      return copy;
    }
    const_iterator& operator--();
    const_iterator operator--(int) {
      auto copy{*this};
      --(*this);
      return copy;
    }

    /* the iterators are only equal if their node iterator (outside) is equal,
     * and the node iterators are either both the end() or they aren't and
     * in that case they are then only equal if they are up to the same edge */
    friend bool operator==(const const_iterator& lhs, const const_iterator& rhs) {
      return lhs.node_it_ == rhs.node_it_ &&
             (lhs.node_it_ == lhs.node_end_it_ || lhs.edge_it_ == rhs.edge_it_);
    }
    friend bool operator!=(const const_iterator& lhs, const const_iterator& rhs) {
      return !(lhs == rhs);
    }

   private:
    /* we need an outer, inner, way of tracking end of outer, and
     * way of tracking reverse end of outer */
    typename std::set<std::unique_ptr<Node>>::iterator node_it_;
    typename std::set<std::weak_ptr<Edge>>::iterator edge_it_;
    const typename std::set<std::unique_ptr<Node>>::iterator node_end_it_;
    const typename std::set<std::unique_ptr<Node>>::iterator node_rend_it_;

    friend class Graph;
    const_iterator(const decltype(node_it_)& node_it,
                   const decltype(edge_it_)& edge_it,
                   const decltype(node_end_it_)& node_end_it,
                   const decltype(node_rend_it_)& node_rend_it)
      : node_it_{node_it}, edge_it_{edge_it}, node_end_it_{node_end_it}, node_rend_it_{
                                                                             node_rend_it} {}
  };

  /* ctors and dtor */
  Graph() = default;
  Graph(typename std::vector<N>::const_iterator b, typename std::vector<N>::const_iterator e);
  Graph(typename std::vector<std::tuple<N, N, E>>::const_iterator b,
        typename std::vector<std::tuple<N, N, E>>::const_iterator e);
  Graph<N, E>(std::initializer_list<N> il);
  Graph<N, E>(const gdwg::Graph<N, E>& orig);
  Graph<N, E>(gdwg::Graph<N, E>&& recyclee);
  ~Graph<N, E>() = default;

  /* operators */
  Graph<N, E>& operator=(const gdwg::Graph<N, E>& other);
  Graph<N, E>& operator=(gdwg::Graph<N, E>&& recyclee);

  /* methods */
  bool InsertNode(const N& val);
  bool InsertEdge(const N& src, const N& dst, const E& w);
  bool DeleteNode(const N& deletee);
  bool Replace(const N& old_data, const N& new_data);
  void MergeReplace(const N& replacee, const N& replacer);
  void Clear();
  bool IsNode(const N& val) const;
  bool IsConnected(const N& src, const N& dst) const;
  std::vector<N> GetNodes() const;
  std::vector<N> GetConnected(const N& src) const;
  std::vector<E> GetWeights(const N& src, const N& dst) const;
  const_iterator find(const N&, const N&, const E&) const;
  bool erase(const N& src, const N& dst, const E& w);
  const_iterator erase(const_iterator it);

  /* iterator methods */
  /* auto generate our reverse iterator from normal iterator */
  using const_reverse_iterator = std::reverse_iterator<const_iterator>;
  const_iterator cbegin() const;
  const_iterator cend() const;
  const_iterator begin() const { return cbegin(); }
  const_iterator end() const { return cend(); }
  const_reverse_iterator crbegin() const { return const_reverse_iterator{cend()}; }
  const_reverse_iterator crend() const { return const_reverse_iterator{cbegin()}; }
  const_reverse_iterator rbegin() const { return crbegin(); }
  const_reverse_iterator rend() const { return crend(); }

  /* friend methods */
  friend bool operator==(const gdwg::Graph<N, E>& lhs, const gdwg::Graph<N, E>& rhs) {
    /* first see if they have the same nodes */
    if (lhs.GetNodes() != rhs.GetNodes())
      return false;
    /* now, for each edge in lhs, find it in rhs */
    for (auto lhs_it = lhs.cbegin(); lhs_it != lhs.cend(); ++lhs_it) {
      auto [src, dst, w] = *lhs_it;
      if (rhs.find(src, dst, w) == rhs.cend())
        return false;
    }
    /* and the same for rhs to lhs */
    for (auto rhs_it = rhs.cbegin(); rhs_it != rhs.cend(); ++rhs_it) {
      auto [src, dst, w] = *rhs_it;
      if (lhs.find(src, dst, w) == lhs.cend())
        return false;
    }
    return true;
  }
  friend bool operator!=(const gdwg::Graph<N, E>& lhs, const gdwg::Graph<N, E>& rhs) {
    return !(lhs == rhs);
  }
  friend std::ostream& operator<<(std::ostream& os, const gdwg::Graph<N, E>& g) {
    /* loop through the nodes_ set */
    for (auto& node_up : g.nodes_) {
      os << node_up->value_ << " (\n";
      /* now for each outgoing edge that node has, print it out */
      for (auto edge_wp : node_up->outgoing_) {
        /* but only if the edge isn't expired */
        if (!edge_wp.expired()) {
          auto edge_sp = edge_wp.lock();
          os << "  " << edge_sp->dst_->value_ << " | " << edge_sp->value_ << "\n";
        }
      }
      os << ")\n";
    }
    return os;
  }

 private:
  /* R is a resource (N or E), smart_ptr is any of the smart
  pointers to one of the wrapper structs (Node or Edge) */
  template <typename R, typename smart_ptr>
  struct WrapperComp {
    using is_transparent = R;
    bool operator()(const smart_ptr& a, const smart_ptr& b) const {
      /* if we want to compare edges then we need to compare src, dst, w, in that order
       * std::tie lets us easily do this */
      if constexpr (std::is_same<smart_ptr, std::shared_ptr<Edge>>::value) {
        return std::tie(a->src_->value_, a->dst_->value_, a->value_) <
               std::tie(b->src_->value_, b->dst_->value_, b->value_);
      } else if constexpr (std::is_same<smart_ptr, std::weak_ptr<Edge>>::value) {
        /* if it's a weak pointer to an edge then we must promote it to a shared
         * pointer first before comparing */
        if (a.expired() || b.expired()) /* expired shouldn't give any ordering */
          return false;
        auto a_sp = a.lock();
        auto b_sp = b.lock();
        return std::tie(a_sp->src_->value_, a_sp->dst_->value_, a_sp->value_) <
               std::tie(b_sp->src_->value_, b_sp->dst_->value_, b_sp->value_);
      } else {
        /* Nodes are only compared by their value (and we are guaranteed to be
         * using unique pointers here for a and b as that's what nodes use) */
        return a->value_ < b->value_;
      }
    }
    /* allows for using non-keys/pseudo keys in our sets */
    bool operator()(const smart_ptr& a, const R& value) const { return a->value_ < value; }
    bool operator()(const R& value, const smart_ptr& b) const { return value < b->value_; }
  };

  /* a wrapper for N's. This is to keep the N, and its associated incoming and outgoing
   * edges all in the one place */
  struct Node {
    explicit Node(N val) : value_{val} {};
    /* the set of 0..* incoming edges */
    std::set<std::weak_ptr<Edge>, WrapperComp<E, std::weak_ptr<Edge>>> incoming_;
    /* the set of 0..* outgoing edges */
    std::set<std::weak_ptr<Edge>, WrapperComp<E, std::weak_ptr<Edge>>> outgoing_;
    N value_;
  };

  /* a wrapper for E's. This is so we can have each E associated with its
   * src and dst node */
  struct Edge {
    Edge(Node* src, Node* dst, E value) : src_{src}, dst_{dst}, value_{value} {}
    Node* src_;  // exactly 1
    Node* dst_;  // exactly 1
    E value_;
  };

  /* set of all Nodes, which are owned by unique pointers. This instantiation of
   * WrapperComp compares Nodes (orders) solely based on the Node value N. */
  std::set<std::unique_ptr<Node>, WrapperComp<N, std::unique_ptr<Node>>> nodes_;
  /* set of all Edges, owned by shared pointers. Note that there is only ever
   * 1 shared pointer per Edge in this set. This instantiation of WrapperComp
   * compares Edges (orders) based on src, then dst, then edge weight */
  std::set<std::shared_ptr<Edge>, WrapperComp<E, std::shared_ptr<Edge>>> edges_;
};

}  // namespace gdwg

#include "assignments/dg/graph.tpp"

#endif  // ASSIGNMENTS_DG_GRAPH_H_
