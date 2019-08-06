#include <algorithm>
#include <initializer_list>
#include <iostream>
#include <memory>
#include <set>
#include <tuple>
#include <utility>
#include <vector>

template <typename N, typename E>
gdwg::Graph<N, E>::Graph(typename std::vector<N>::const_iterator b,
                         typename std::vector<N>::const_iterator e) {
  for (; b != e; ++b) {
    InsertNode(*b);
  }
}

template <typename N, typename E>
gdwg::Graph<N, E>::Graph(typename std::vector<std::tuple<N, N, E>>::const_iterator b,
                         typename std::vector<std::tuple<N, N, E>>::const_iterator e) {
  for (; b != e; ++b) {
    /* unpack the current tuple */
    auto [src, dst, weight] = *b;
    /* add the nodes if they don't already exist */
    InsertNode(src);
    InsertNode(dst);
    /* insert the edge if it doesn't already exist */
    InsertEdge(src, dst, weight);
  }
}

template <typename N, typename E>
gdwg::Graph<N, E>::Graph(std::initializer_list<N> il) {
  for (auto it : il) {
    InsertNode(it);
  }
}

template <typename N, typename E>
gdwg::Graph<N, E>::Graph(const gdwg::Graph<N, E>& orig) {
  /* deep copy because we have unique ptrs inside graphs which cannot be copied
   * constructed or copy assigned */

  /* first copy the nodes. We can and should only copy by just
   * passing in the N value because this will set the node
   * value but leave the incoming/outgoing sets for that Node
   * empty (We cannot copy those because the Edge struct
   * pointers (addresses) refer to Nodes in orig, not this (new graph)) */
  for (auto on_it = orig.nodes_.begin(); on_it != orig.nodes_.end(); ++on_it) {
    nodes_.insert(std::make_unique<Node>((*on_it)->value_));
  }
  /* now copy the edges. It isn't sufficient to just call this
   * and skip above because then isolated nodes would not be in the graph */
  for (auto oe_it = orig.edges_.begin(); oe_it != orig.edges_.end(); ++oe_it) {
    InsertEdge((*oe_it)->src_->value_, (*oe_it)->dst_->value_, (*oe_it)->value_);
  }
}

template <typename N, typename E>
gdwg::Graph<N, E>::Graph(gdwg::Graph<N, E>&& recyclee) {
  nodes_ = std::move(recyclee.nodes_);
  edges_ = std::move(recyclee.edges_);
}

template <typename N, typename E>
gdwg::Graph<N, E>& gdwg::Graph<N, E>::operator=(const gdwg::Graph<N, E>& other) {
  /* this is essentially the same as the copy ctor, and
   * I am doing the same thing for the same reasons so
   * see it's comment :) */
  /* deep copy nodes */
  for (auto on_it = other.nodes_.begin(); on_it != other.nodes_.end(); ++on_it) {
    nodes_.insert(std::make_unique<Node>((*on_it)->value_));
  }
  /* deep copy edges */
  for (auto oe_it = other.edges_.begin(); oe_it != other.edges_.end(); ++oe_it) {
    InsertEdge((*oe_it)->src_->value_, (*oe_it)->dst_->value_, (*oe_it)->value_);
  }
  return *this;
}

template <typename N, typename E>
gdwg::Graph<N, E>& gdwg::Graph<N, E>::operator=(gdwg::Graph<N, E>&& recyclee) {
  nodes_ = std::move(recyclee.nodes_);
  edges_ = std::move(recyclee.edges_);
  return *this;
}

template <typename N, typename E>
bool gdwg::Graph<N, E>::InsertNode(const N& val) {
  /* if the graph doesn't already contain this node then insert */
  if (IsNode(val))
    return false;
  return this->nodes_.insert(std::make_unique<Node>(val)).second;
}

template <typename N, typename E>
bool gdwg::Graph<N, E>::InsertEdge(const N& src, const N& dst, const E& w) {
  /* see if both nodes exists in the graph, if not throw */
  auto src_it = this->nodes_.find(src);
  auto dst_it = this->nodes_.find(dst);
  if (src_it == this->nodes_.end() || dst_it == this->nodes_.end()) {
    throw std::runtime_error(
        "Cannot call Graph::InsertEdge when either src or dst node does not exist");
  }
  /* if the edge already exists then exit */
  if (find(src, dst, w) != cend())
    return false;

  /* otherwise add the edge to the graphs edges_ set */
  auto new_edge_it = edges_.insert(std::make_shared<Edge>(src_it->get(), dst_it->get(), w)).first;
  /* and now update the book-keeping on the Nodes edges sets (outgoing for src and incoming for dst)
   */
  bool out_succ = (*src_it)->outgoing_.insert(std::weak_ptr<Edge>(*new_edge_it)).second;
  bool in_succ = (*dst_it)->incoming_.insert(std::weak_ptr<Edge>(*new_edge_it)).second;
  return (out_succ && in_succ);
}

template <typename N, typename E>
bool gdwg::Graph<N, E>::DeleteNode(const N& deletee) {
  /* no point looping all edges below if the node doesn't even exist */
  if (!IsNode(deletee))
    return false;

  /* node exists, so firstly need to delete the nodes edges */
  for (auto edges_it = edges_.begin(); edges_it != edges_.end();) {
    /* if the deletee node is part of this edge then delete it */
    if ((*edges_it)->src_->value_ == deletee || (*edges_it)->dst_->value_ == deletee) {
      edges_it = edges_.erase(edges_it);
    } else {  // otherwise just continue
      ++edges_it;
    }
  }

  /* now delete the node */
  auto deletee_it = this->nodes_.find(deletee);
  if (deletee_it == this->nodes_.end())
    return false; /* this shouldn't occur but do it for safety */
  nodes_.erase(deletee_it);
  return true;
}

template <typename N, typename E>
bool gdwg::Graph<N, E>::Replace(const N& old_data, const N& new_data) {
  /* if a node with old_data doesn't even exist then throw */
  auto old_node = nodes_.find(old_data);
  if (old_node == nodes_.end()) {
    throw std::runtime_error("Cannot call Graph::Replace on a node that doesn't exist");
  }
  /* if there is already a node with new data, return false */
  if (IsNode(new_data))
    return false;
  /* otherwise create a new node with the new data */
  InsertNode(new_data);
  /* now get the same outgoing edges as the old node */
  for (auto edge_wp : (*old_node)->outgoing_) {
    /* only get the edge if it isn't expired */
    if (!edge_wp.expired()) {
      auto edge_sp = edge_wp.lock();
      /* reflexive edges must be reflexive on new node and
       * not be an edge between new node and old node */
      if (edge_sp->dst_->value_ == old_data) {
        InsertEdge(new_data, new_data, edge_sp->value_);
      } else {
        InsertEdge(new_data, edge_sp->dst_->value_, edge_sp->value_);
      }
    }
  }
  /* and now the same for the incoming edges */
  for (auto edge_wp : (*old_node)->incoming_) {
    /* only get the edge if it isn't expired */
    if (!edge_wp.expired()) {
      auto edge_sp = edge_wp.lock();
      /* if we encounter a reflexive edge here, because we
       * already handled it above, we just skip it */
      if (edge_sp->src_->value_ == old_data)
        continue;
      InsertEdge(edge_sp->src_->value_, new_data, edge_sp->value_);
    }
  }
  /* now delete the old node */
  DeleteNode(old_data);
  return true;
}

template <typename N, typename E>
void gdwg::Graph<N, E>::MergeReplace(const N& replacee, const N& replacer) {
  /* try find both the replacer and replacee nodes, if either doesn't exist then throw */
  auto replacee_it = nodes_.find(replacee);
  auto replacer_it = nodes_.find(replacer);
  if (replacee_it == nodes_.cend() || replacer_it == nodes_.cend()) {
    throw std::runtime_error(
        "Cannot call Graph::MergeReplace on old or new data if they don't exist in the graph");
  }
  /* loop through the outgoing edges of replacer and add them to
   * replacee (removing dupes as necessary) */
  for (auto edge_wp : (*replacer_it)->outgoing_) {
    /* only get the edge if it isn't expired */
    if (!edge_wp.expired()) {
      auto edge_sp = edge_wp.lock();
      InsertEdge(replacee, edge_sp->dst_->value_, edge_sp->value_);
    }
  }
  /* now do the same but for incoming edges of replacer */
  for (auto edge_wp : (*replacer_it)->incoming_) {
    /* only get the edge if it isn't expired */
    if (!edge_wp.expired()) {
      auto edge_sp = edge_wp.lock();
      InsertEdge(edge_sp->src_->value_, replacee, edge_sp->value_);
    }
  }
  /* now delete replacer from the graph */
  DeleteNode(replacer);
  /* and now, Replace replacee's value with replacer */
  Replace(replacee, replacer);
}

template <typename N, typename E>
void gdwg::Graph<N, E>::Clear() {
  nodes_.clear();
  edges_.clear();
}

template <typename N, typename E>
bool gdwg::Graph<N, E>::IsNode(const N& val) const {
  auto node_it = this->nodes_.find(val);
  return node_it != this->nodes_.end();
}

template <typename N, typename E>
bool gdwg::Graph<N, E>::IsConnected(const N& src, const N& dst) const {
  /* see if both nodes exists in the graph, if not throw */
  auto src_it = this->nodes_.find(src);
  auto dst_it = this->nodes_.find(dst);
  if (src_it == this->nodes_.end() || dst_it == this->nodes_.end()) {
    throw std::runtime_error(
        "Cannot call Graph::IsConnected if src or dst node don't exist in the graph");
  }
  /* loop through all the outgoing edges of src */
  for (auto oe_it = (*src_it)->outgoing_.begin(); oe_it != (*src_it)->outgoing_.end(); ++oe_it) {
    /* may as well erase and expired edges whilst we are at it */
    if (oe_it->expired()) {
      oe_it = (*src_it)->outgoing_.erase(oe_it);
      continue;
    } else {
      auto edge_sp = oe_it->lock();
      /* if we find an edge that connects the two nodes then return true */
      if (edge_sp->dst_->value_ == dst) {
        return true;
      }
    }
  }
  return false; /* didn't find any connecting edges */
}

template <typename N, typename E>
std::vector<N> gdwg::Graph<N, E>::GetNodes() const {
  std::vector<N> node_vec{};
  for (auto& node : nodes_) {
    node_vec.push_back(node->value_);
  }
  return node_vec;
}

template <typename N, typename E>
std::vector<N> gdwg::Graph<N, E>::GetConnected(const N& src) const {
  /* see if the src node even exists, throw if not */
  auto src_it = this->nodes_.find(src);
  if (src_it == this->nodes_.end()) {
    throw std::out_of_range("Cannot call Graph::GetConnected if src doesn't exist in the graph");
  }
  /* I use a set so that N's are only added once and are ordered implicitly */
  std::set<N> node_set{};
  /* loop through all the outgoing edges of src */
  for (auto oe_it = (*src_it)->outgoing_.begin(); oe_it != (*src_it)->outgoing_.end(); ++oe_it) {
    /* may as well erase whilst i am looping */
    if (oe_it->expired()) {
      oe_it = (*src_it)->outgoing_.erase(oe_it);
      continue;
    } else {
      auto edge_sp = oe_it->lock();
      /* add the node value of this connected node */
      node_set.insert(edge_sp->dst_->value_);
    }
  }
  /* return a vector of the set values */
  return std::vector<N>{node_set.begin(), node_set.end()};
}

template <typename N, typename E>
std::vector<E> gdwg::Graph<N, E>::GetWeights(const N& src, const N& dst) const {
  /* see if both nodes exists in the graph, if not throw */
  auto src_it = this->nodes_.find(src);
  if (src_it == this->nodes_.end() || !IsNode(dst)) {
    throw std::out_of_range(
        "Cannot call Graph::GetWeights if src or dst node don't exist in the graph");
  }
  /* using a set so E's are only added once */
  std::set<E> edge_set{};
  /* loop through all the outgoing edges of src */
  for (auto oe_it = (*src_it)->outgoing_.begin(); oe_it != (*src_it)->outgoing_.end(); ++oe_it) {
    if (oe_it->expired()) {
      oe_it = (*src_it)->outgoing_.erase(oe_it);
      continue;
    } else {
      auto edge_sp = oe_it->lock();
      if (edge_sp->dst_->value_ == dst) {
        /* add the edge value of this edge to the connected node */
        edge_set.insert(edge_sp->value_);
      }
    }
  }
  return std::vector<E>{edge_set.begin(), edge_set.end()};
}

template <typename N, typename E>
typename gdwg::Graph<N, E>::const_iterator
gdwg::Graph<N, E>::find(const N& src, const N& dst, const E& w) const {
  /* use the edge iterater to iterate over all edges in the graph */
  for (auto it = cbegin(); it != cend(); ++it) {
    /* if we find a matching edge, return iterator to it */
    if (src == std::get<0>(*it) && dst == std::get<1>(*it) && w == std::get<2>(*it)) {
      return it;
    }
  }
  return cend(); /* didn't find edge so return end */
}

template <typename N, typename E>
bool gdwg::Graph<N, E>::erase(const N& src, const N& dst, const E& w) {
  /* try find the edge */
  auto edge_it = find(src, dst, w);
  if (edge_it == cend())
    return false;
  /* if we found it then erase it */
  erase(edge_it);
  return true;
}

template <typename N, typename E>
typename gdwg::Graph<N, E>::const_iterator gdwg::Graph<N, E>::erase(const_iterator it) {
  /* trying to erase end so just return end */
  if (it == end())
    return end();
  /* save a copy of the current iterator as we need to delete it,
   * and increment it because we need to return it */
  auto deletee_it = it++;
  /* now search the set of shared_ptr to Edges until we find this
   * edge, then delete it */
  for (auto edge_it = edges_.begin(); edge_it != edges_.end(); ++edge_it) {
    /* get the current edge values (src, dst, w) */
    auto src = (*edge_it)->src_->value_;
    auto dst = (*edge_it)->dst_->value_;
    auto w = (*edge_it)->value_;
    /* if it's a match, erase the owning shared pointer */
    if (src == std::get<0>(*deletee_it) && dst == std::get<1>(*deletee_it) &&
        w == std::get<2>(*deletee_it)) {
      edges_.erase(edge_it);
      break;
    }
  }
  return it;
}

template <typename N, typename E>
typename gdwg::Graph<N, E>::const_iterator gdwg::Graph<N, E>::cbegin() const {
  /* loop through the graph nodes (because we can only start at an un-isolated node) */
  for (auto nodes_it = nodes_.begin(); nodes_it != nodes_.end(); ++nodes_it) {
    auto edges_it = (*nodes_it)->outgoing_.begin();
    auto edges_end = (*nodes_it)->outgoing_.end();

    /* loop through the current nodes outgoing edges */
    while (edges_it != edges_end) {
      /* remove any expired wp's as we iterate through */
      if (edges_it->expired()) {
        edges_it = ((*nodes_it)->outgoing_).erase(edges_it);
        continue;
      } else { /* not expired, so this is the first valid edge */
        /* create a new iterator at this edge and return it */
        return {nodes_it, edges_it, nodes_.end(), nodes_.begin()};
      }
      ++edges_it;
    }
  }
  return cend(); /* no edges, return end */
}

template <typename N, typename E>
typename gdwg::Graph<N, E>::const_iterator gdwg::Graph<N, E>::cend() const {
  /* node it (outer) is end, thus edge it (inner) is just an empty iterator
   * and the sentinals are always the end and begin no matter what */
  return {nodes_.end(), {}, nodes_.end(), nodes_.begin()};
}

template <typename N, typename E>
typename gdwg::Graph<N, E>::const_iterator& gdwg::Graph<N, E>::const_iterator::operator++() {
  /* increment the edge iterator */
  ++edge_it_;

  /* it is now possible that edge_it_ is actually the end() iterator
   * of the current edges container (i.e. node->outgoing set) we are up
   * to OR it is an expired edge (weak pointer it refers to is expired) */
  while (edge_it_ == (*node_it_)->outgoing_.end() || edge_it_->expired()) {
    /* if we got to the end of current outgoing edges set, go to next nodes one */
    if (edge_it_ == (*node_it_)->outgoing_.end()) {
      ++node_it_;
      /* if there was no next node, return equivalent of empty iterator */
      if (node_it_ == node_end_it_) {
        edge_it_ = typename std::set<std::weak_ptr<Edge>>::iterator{};
        return *this;
      }
      /* otherwise set the edge iterator to be the begin of the new edges set */
      edge_it_ = (*node_it_)->outgoing_.begin();
    } else { /* we got an expired edge, erase it and continue search */
      edge_it_ = (*node_it_)->outgoing_.erase(edge_it_);
    }
  }
  return *this;
}

template <typename N, typename E>
typename gdwg::Graph<N, E>::const_iterator& gdwg::Graph<N, E>::const_iterator::operator--() {
  /* if we were off the end, then restore to valid state */
  if (node_it_ == node_end_it_) {
    --node_it_;
    /* skip empty edge sets and return equivalent of end if we
     * make it to node reverse end */
    while ((*node_it_)->outgoing_.begin() == (*node_it_)->outgoing_.end()) {
      /* no more nodes (outers), so we return equivalent of end() */
      if (node_it_ == node_rend_it_) {
        edge_it_ = typename std::set<std::weak_ptr<Edge>>::iterator{};
        return *this;
      }
      --node_it_;
    }
    edge_it_ = (*node_it_)->outgoing_.end();
  }
  /* at this stage it is guaranteed that we either returned
   * the equivlent of end() OR both the node it and edge it
   * are valid (but edge it could be pointing to end()) */
  if (edge_it_ != (*node_it_)->outgoing_.begin()) {
    --edge_it_;
    if (!edge_it_->expired())
      return *this;
  }

  /* only need to keep decrementing if we have an expired edge or we're
  at the beginning of an edge set */
  while (edge_it_->expired() || edge_it_ == (*node_it_)->outgoing_.begin()) {
    if (edge_it_ == (*node_it_)->outgoing_.begin()) {
      /* try find the prev node edge set */
      do {
        /* if there is no previous nodes, return equivalent of end() */
        if (node_it_ == node_rend_it_) {
          edge_it_ = typename std::set<std::weak_ptr<Edge>>::iterator{};
          return *this;
        }
        /* otherwise go to previous node */
        --node_it_;
      } while ((*node_it_)->outgoing_.begin() == (*node_it_)->outgoing_.end());
      /* got a previous node without an empty out edge set, set edge it to its last edge */
      edge_it_ = --((*node_it_)->outgoing_.end());
      if (!edge_it_->expired())
        return *this;
    } else { /* got an expired edge (not beginning though, so keep searching) */
      edge_it_ = --((*node_it_)->outgoing_.erase(edge_it_));
    }
  }
  return *this;
}

template <typename N, typename E>
typename gdwg::Graph<N, E>::const_iterator::reference gdwg::Graph<N, E>::const_iterator::
operator*() const {
  /* get the src value from the node iterator */
  const N& src_val = (*node_it_)->value_;
  /* now promote the weak pointer the edge iterator is up to to a shared pointer so we can extract
   * the edge values */
  auto edge_sp = (*edge_it_).lock();
  /* dst is the edges destination nodes value */
  const N& dst_val = edge_sp->dst_->value_;
  /* src is the edges source nodes value */
  const E& edge_val = edge_sp->value_;
  /* return a tuple of this */
  return {src_val, dst_val, edge_val};
}
