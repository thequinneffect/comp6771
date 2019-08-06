//template <typename N, typename E>
//gdwg::Graph<N,E>::Graph() {}

#include <algorithm>
#include <initializer_list>
#include <iostream>
#include <memory>
#include <utility>
#include <vector>

template <typename N, typename E>
gdwg::Graph<N,E>::Graph(
      typename std::vector<N>::const_iterator b,
      typename std::vector<N>::const_iterator e
)
{
    for (; b != e; ++b) { InsertNode(*b); }
}

template <typename N, typename E>
gdwg::Graph<N,E>::Graph(
      typename std::vector<std::tuple<N, N, E>>::const_iterator b,
      typename std::vector<std::tuple<N, N, E>>::const_iterator e
)
{
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

// TODO: why isnt this a const initlist& ??? are we supposed to change it to this?
template <typename N, typename E>
gdwg::Graph<N, E>::Graph(std::initializer_list<N> il) {
    for (auto it : il) {
        InsertNode(it);
    }
}

template <typename N, typename E>
gdwg::Graph<N, E>::Graph(const gdwg::Graph<N, E>& orig) {
    /* deep copy because we have unique ptrs inside graphs */

    /* first copy the nodes. We can and should only copy by just
     * passing in the N value because this will set the node 
     * value but leave the incoming/outgoing sets for that Node
     * empty (We cannot copy those because the Edge struct 
     * pointers (addresses) refer to Nodes in orig, not this) */
    for (auto on_it = orig.nodes_.begin(); 
        on_it != orig.nodes_.end(); ++on_it) 
    {
        nodes_.insert(std::make_unique<Node>((*on_it)->value_));
    }
    /* now copy the edges. It isn't sufficient to just call this
     * because then isolated nodes would not be in the graph */
    for (auto oe_it = orig.edges_.begin();
        oe_it != orig.edges_.end(); ++oe_it)
    {
        InsertEdge((*oe_it)->src_->value_, (*oe_it)->dst_->value_, (*oe_it)->value_);
    }
}

template <typename N, typename E>
gdwg::Graph<N, E>::Graph(gdwg::Graph<N, E>&& recyclee) {

    nodes_ = std::move(recyclee.nodes_);
    edges_ = std::move(recyclee.edges_);

}

template <typename N, typename E>
gdwg::Graph<N, E>& 
gdwg::Graph<N,E>::operator=(const gdwg::Graph<N, E>& other) {
    /* this is essentially the same as the copy ctor, and
     * I am doing the same thing for the same reasons so
     * see it's comment :) */
    /* deep copy nodes */
    for (auto on_it = other.nodes_.begin(); 
        on_it != other.nodes_.end(); ++on_it) 
    {
        nodes_.insert(std::make_unique<Node>((*on_it)->value_));
    }
    /* deep copy edges */
    for (auto oe_it = other.edges_.begin();
        oe_it != other.edges_.end(); ++oe_it)
    {
        InsertEdge((*oe_it)->src_->value_, (*oe_it)->dst_->value_, (*oe_it)->value_);
    }
    return *this;
}

template <typename N, typename E>
gdwg::Graph<N, E>& 
gdwg::Graph<N,E>::operator=(gdwg::Graph<N, E>&& recyclee) {
    nodes_ = std::move(recyclee.nodes_);
    edges_ = std::move(recyclee.edges_);
    return *this;
}

template <typename N, typename E>
bool gdwg::Graph<N,E>::InsertNode(const N& val)
{
    /* if the graph doesn't already contain this node then insert */
    if (IsNode(val)) return false;
    return this->nodes_.insert(std::make_unique<Node>(val)).second;
}

template <typename N, typename E>
bool gdwg::Graph<N,E>::InsertEdge(const N& src, const N& dst, const E& w)
{
    /* see if both nodes exists in the graph, if not throw */
    auto src_it = this->nodes_.find(src);
    auto dst_it = this->nodes_.find(dst);
    if (src_it == this->nodes_.end() || dst_it == this->nodes_.end()) {
        throw std::runtime_error("Cannot call Graph::InsertEdge when either src or dst node does not exist");
    }
    /* if the edge already exists then exit */
    if (find(src, dst, w) != cend()) return false;

    //std::cout << "edge [" << src << ", " << dst << ", " << w << "] was not found in find(s,d,w) and so should be inserted!\n";
    /* otherwise add the edge to the edges_ set and update the 
     * book-keeping on the src and dst Node stuctures */
    auto new_edge_it = edges_.insert(std::make_shared<Edge>(src_it->get(), dst_it->get(), w)).first;
    // if (new_edge_it != edges_.end()) {
    //     std::cout << "inserted (or prevented by) : " << (*new_edge_it)->src_->value_ << (*new_edge_it)->dst_->value_ << (*new_edge_it)->value_ << "\n";
    // }
    //std::cout << "inserted edge was: " << src_it->get()  << " " << dst_it->get() << " " << (*new_edge_it)->value_ << "\n";
    bool out_succ = (*src_it)->outgoing_.insert(std::weak_ptr<Edge>(*new_edge_it)).second;
    bool in_succ = (*dst_it)->incoming_.insert(std::weak_ptr<Edge>(*new_edge_it)).second;
    //std::cout << "updating src node success: " << out_succ << ", updating dst node success: " << in_succ << "\n";
    return (out_succ && in_succ);
}

template <typename N, typename E>
bool gdwg::Graph<N,E>::DeleteNode(const N& deletee) {

    /* no point doing the below edges stuff if it doesn't exist */
    if (!IsNode(deletee)) return false;

    /* firstly need to delete the nodes edges */
    for (auto edges_it = edges_.begin(); edges_it != edges_.end(); ) {
        /* if the deletee node is part of this edge then delete it */
        if ((*edges_it)->src_->value_ == deletee
            || (*edges_it)->dst_->value_ == deletee)  {
            edges_it = edges_.erase(edges_it);
        } else { // otherwise just continue
            ++edges_it;
        }
    }

    /* now I can delete the node */
    auto deletee_it = this->nodes_.find(deletee);
    if (deletee_it == this->nodes_.end()) return false;
    nodes_.erase(deletee_it);
    return true;
}

template <typename N, typename E>
void gdwg::Graph<N,E>::Clear() {
    nodes_.clear();
    edges_.clear();
}

template <typename N, typename E>
bool gdwg::Graph<N,E>::IsNode(const N& val) const {
    auto node_it = this->nodes_.find(val);
    return node_it != this->nodes_.end();
}

template <typename N, typename E>
bool gdwg::Graph<N,E>::IsConnected(const N& src, const N& dst) {
    /* see if both nodes exists in the graph, if not throw */
    auto src_it = this->nodes_.find(src);
    auto dst_it = this->nodes_.find(dst);
    if (src_it == this->nodes_.end() || dst_it == this->nodes_.end()) {
        throw std::runtime_error("Cannot call Graph::IsConnected if src or dst node don't exist in the graph");
    }
    /* loop through all the outgoing edges of src */
    for (auto oe_it = (*src_it)->outgoing_.begin();
        oe_it != (*src_it)->outgoing_.end(); ++oe_it) {
        if (oe_it->expired()) {
            oe_it = (*src_it)->outgoing_.erase(oe_it);
            continue;
        } else {
            auto edge_sp = oe_it->lock();
            if (edge_sp->dst_->value_ == dst) {
                return true;
            }
        }
    }
    return false;
}

template <typename N, typename E>
std::vector<N> gdwg::Graph<N,E>::GetNodes() const {
    std::vector<N> node_vec {};
    for (auto& node : nodes_) {
        node_vec.push_back(node->value_);
    }
    return node_vec;
}

template <typename N, typename E>
std::vector<N> gdwg::Graph<N,E>::GetConnected(const N& src) {
    /* see if the src node even exists, throw if not */
    auto src_it = this->nodes_.find(src);
    if (src_it == this->nodes_.end()) {
        throw std::out_of_range("Cannot call Graph::GetConnected if src doesn't exist in the graph");
    }
    std::set<N> node_set {};
    /* loop through all the outgoing edges of src */
    for (auto oe_it = (*src_it)->outgoing_.begin();
        oe_it != (*src_it)->outgoing_.end(); ++oe_it) {
        if (oe_it->expired()) {
            oe_it = (*src_it)->outgoing_.erase(oe_it);
            continue;
        } else {
            auto edge_sp = oe_it->lock();
            node_set.insert(edge_sp->dst_->value_);
        }
    }
    return std::vector<N>{node_set.begin(), node_set.end()};
}

// TODO: refactor this and ^ search part into a templated method if have time
template <typename N, typename E>
std::vector<E> gdwg::Graph<N,E>::GetWeights(const N& src, const N& dst) {
    /* see if both nodes exists in the graph, if not throw */
    auto src_it = this->nodes_.find(src);
    if (src_it == this->nodes_.end() || !IsNode(dst)) {
        throw std::out_of_range("Cannot call Graph::GetWeights if src or dst node don't exist in the graph");
    }
    std::set<E> edge_set {};
    /* loop through all the outgoing edges of src */
    for (auto oe_it = (*src_it)->outgoing_.begin();
        oe_it != (*src_it)->outgoing_.end(); ++oe_it) {
        if (oe_it->expired()) {
            oe_it = (*src_it)->outgoing_.erase(oe_it);
            continue;
        } else {
            auto edge_sp = oe_it->lock();
            if (edge_sp->dst_->value_ == dst) {
                edge_set.insert(edge_sp->value_);
            }
        }
    }
    return std::vector<E>{edge_set.begin(), edge_set.end()};
}

template <typename N, typename E>
typename gdwg::Graph<N,E>::const_iterator 
gdwg::Graph<N,E>::find(const N& src, const N& dst, const E& w) const {
    for (auto it = cbegin(); it != cend(); ++it) {
        if (src == std::get<0>(*it) && dst == std::get<1>(*it) && w == std::get<2>(*it)) {
            //std::cout << "node found was: " << std::get<0>(*it) << ", " << std::get<1>(*it) << ", " << std::get<2>(*it) << "\n";
            return it;
        }
    }
    //std::cout << "didn't find node, returning cend()\n";
    return cend();
}

template <typename N, typename E>
bool gdwg::Graph<N,E>::erase(const N& src, const N& dst, const E& w) {
    auto edge_it = find(src, dst, w);
    if (edge_it == cend()) return false;
    erase(edge_it); return true;
}

template <typename N, typename E>
typename gdwg::Graph<N,E>::const_iterator 
gdwg::Graph<N,E>::erase(const_iterator it) {
    if (it == end()) return end();
    /* save a copy of the current iterator as we need to delete it,
     * and increment it because we need to return it */
    auto deletee_it = it++;
    /* now search the set of shared_ptr to Edges until we find this
     * edge, then delete it */
    for (auto edge_it = edges_.begin();
        edge_it != edges_.end(); ++edge_it) {
        
        auto src = (*edge_it)->src_->value_;
        auto dst = (*edge_it)->dst_->value_;
        auto w = (*edge_it)->value_;
        if (src == std::get<0>(*deletee_it) 
            && dst == std::get<1>(*deletee_it) 
            && w == std::get<2>(*deletee_it)) 
        {
            edges_.erase(edge_it);
            break;
        }
    }
    return it;
}

template <typename N, typename E>
typename gdwg::Graph<N,E>::const_iterator gdwg::Graph<N,E>::cbegin() const {
    // std::cout << "const_iterator cbegin called\n";
    /* nodes_it is a up<Node> iterator. 1 deref gives up<Node>, 2 gives Node */
    for (auto nodes_it = nodes_.begin(); nodes_it != nodes_.end(); ++nodes_it) {

        auto edges_it = (*nodes_it)->outgoing_.begin();
        auto edges_end = (*nodes_it)->outgoing_.end();

        /* loop through the current nodes outgoing edges */
        while (edges_it != edges_end) {
            /* remove any expired wp's as we iterate through */
            if (edges_it->expired()) {
                //std::cout << "edges_it in cbegin is expired, removing it!\n";
                edges_it = ((*nodes_it)->outgoing_).erase(edges_it);
                continue;
            } else { /* not expired, so this is the first valid edge */
                //std::cout << "values are: " << nodes_it << " " << edges_it << " " << nodes_.end() << "\n";
                //std::cout << "edges_it in cbegin is not expired, returning it!\n";
                return {nodes_it, edges_it, nodes_.end(), nodes_.begin()};
            }
            ++edges_it;
        }
    }
    return cend();
}

template <typename N, typename E>
typename gdwg::Graph<N,E>::const_iterator gdwg::Graph<N,E>::cend() const {
    // std::cout << "const_iterator cend called\n";
    return {nodes_.end(), {}, nodes_.end(), nodes_.begin()};
}

// TODO: factor out similarity of this with cbegin()
template <typename N, typename E>
typename gdwg::Graph<N,E>::const_iterator& gdwg::Graph<N,E>::const_iterator::operator++() {
    //std::cout << "const_iterator operator++ called\n";
    // /* we need to increment the edge iterator */
    // ++edge_it_;
    // /* if the edge iterator got to the end of the edges for
    //  * the current node, OR it is an expired edge,
    //  *  then we need to go to the next node */
    // if (edge_it_ == (*node_it_)->outgoing_.end()
    //     || edge_it_->expired()) {
    //     if (edge_it_ == (*node_it_)->outgoing_.end()) {
    //         ++node_it_;
    //     }
    //     for (; node_it_ != node_end_it_; ++node_it_) {

    //         auto edges_it = (*node_it_)->outgoing_.begin();
    //         //auto edges_end = (*node_it_)->outgoing_.end();

    //         /* loop through the current nodes outgoing edges */
    //         for (; edges_it != (*node_it_)->outgoing_.end();) {
    //             /* remove any expired wp's as we iterate through */
    //             if (edges_it->expired()) {
    //                 std::cout << "edges_it in operator++ is expired, removing it!\n";
    //                 edges_it = ((*node_it_)->outgoing_).erase(edges_it);
    //                 continue;
    //             } else { /* not expired, so this is the first valid edge */
    //                 std::cout << "edges_it in operator++ is not expired, returning it!\n";
    //                 edge_it_ = edges_it;
    //                 return *this;
    //             }
    //             ++edges_it;
    //         }
    //     }
    //     /* edge it needs to be {} so it compares == with end now */ // TODO: is this correct?
    //     edge_it_ = typename std::set<std::weak_ptr<Edge>>::iterator{};
    // }
    // return *this;

    /* increment the edge iterator */
    ++edge_it_;

    /* it is now possible that edge_it_ is actually the end() iterator
     * of the current edges container (i.e. node->outgoing set) we are up
     * to OR it is an expired edge (weak pointer it refers to is expired) */
    while (edge_it_ == (*node_it_)->outgoing_.end() || edge_it_->expired()) {
        if (edge_it_ == (*node_it_)->outgoing_.end()) {
            ++node_it_;
            if (node_it_ == node_end_it_) {
                edge_it_ = typename std::set<std::weak_ptr<Edge>>::iterator{};
                return *this;
            }
            edge_it_ = (*node_it_)->outgoing_.begin();
        } else {
            edge_it_ = (*node_it_)->outgoing_.erase(edge_it_);
        }
    }
    return *this;
}

template <typename N, typename E>
typename gdwg::Graph<N,E>::const_iterator& gdwg::Graph<N,E>::const_iterator::operator--() {
    std::cout << "-- was called\n";
    /* if the edge iterator is at begin() of its set than it cannot be 
     * decremented */
    if (edge_it_ != (*node_it_)->outgoing_.begin()) {
        --edge_it_;
    }

    /* if we are at the start of a set (begin) or current edge
     * is expired then we need to keep decrementing */
    while (node_it_ != node_rend_it_ && (edge_it_ == (*node_it_)->outgoing_.begin() 
            || edge_it_->expired())) {  

        /* if the edge is expired and it's the begin edge */
        if (edge_it_ == (*node_it_)->outgoing_.begin()
            && edge_it_->expired()) {
            (*node_it_)->outgoing_.erase(edge_it_);
            --node_it_;
            edge_it_ = --(*node_it_)->outgoing_.end();
        /* if the edge was beginning edge but not expired */
        } else if (edge_it_ == (*node_it_)->outgoing_.begin()) {
            --node_it_;
            edge_it_ = --(*node_it_)->outgoing_.end();
        /* else it's expired, but not the beginning edge */
        } else {
            edge_it_ = --(*node_it_)->outgoing_.erase(edge_it_);
        }
    }
    return *this;
}

template <typename N, typename E>
typename gdwg::Graph<N,E>::const_iterator::reference 
gdwg::Graph<N,E>::const_iterator::operator*() const {
    const N& src_val = (*node_it_)->value_;
    // std::cout << "deref'ing for src was fine\n";
    if (edge_it_->expired()) std::cout << "edge_it_ is expired!\n";
    auto edge_sp = (*edge_it_).lock();
    // std::cout << "locking  was fine\n";
    const N& dst_val = edge_sp->dst_->value_;
    // std::cout << "deref'ing for dst was fine\n";
    const E& edge_val = edge_sp->value_;
    // std::cout << "deref'ing for edge was fine\n";
    return {src_val, dst_val, edge_val};
}