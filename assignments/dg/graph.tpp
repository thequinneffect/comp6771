//template <typename N, typename E>
//gdwg::Graph<N,E>::Graph() {}

#include <algorithm>
#include <initializer_list>
#include <iostream>
#include <memory>
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
        std::cout << "trying to tuple insert [" << src << ", " << dst << ", " << weight << "]: ";
        std::cout << InsertEdge(src, dst, weight) << "\n";
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
    std::cout << "inserted edge was: " << src_it->get()  << " " << dst_it->get() << " " << (*new_edge_it)->value_ << "\n";
    bool out_succ = (*src_it)->outgoing_.insert(std::weak_ptr<Edge>(*new_edge_it)).second;
    bool in_succ = (*dst_it)->incoming_.insert(std::weak_ptr<Edge>(*new_edge_it)).second;
    std::cout << "updating src node success: " << out_succ << ", updating dst node success: " << in_succ << "\n";
    return (out_succ && in_succ);
}

template <typename N, typename E>
bool gdwg::Graph<N,E>::IsNode(const N& val) const {
    auto node_it = this->nodes_.find(val);
    return node_it != this->nodes_.end();
}

template <typename N, typename E>
typename gdwg::Graph<N,E>::const_iterator 
gdwg::Graph<N,E>::find(const N& src, const N& dst, const E& w) {
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
typename gdwg::Graph<N,E>::const_iterator::reference 
gdwg::Graph<N,E>::const_iterator::operator*() const {
    const N& src_val = (*node_it_)->value_;
    auto edge_sp = (*edge_it_).lock();
    const N& dst_val = edge_sp->dst_->value_;
    const E& edge_val = edge_sp->value_;
    return {src_val, dst_val, edge_val};
}

template <typename N, typename E>
typename gdwg::Graph<N,E>::const_iterator gdwg::Graph<N,E>::cbegin() {
    /* nodes_it is a up<Node> iterator. 1 deref gives up<Node>, 2 gives Node */
    for (auto nodes_it = nodes_.begin(); nodes_it != nodes_.end(); ++nodes_it) {
        auto edges_it = (*nodes_it)->outgoing_.begin();
        auto edges_end = (*nodes_it)->outgoing_.end();

        /* loop through the current nodes outgoing edges */
        while (edges_it != edges_end) {
            /* remove any expired wp's as we iterate through */
            if (edges_it->expired()) {
                edges_it = ((*nodes_it)->outgoing_).erase(edges_it);
            } else { /* not expired, so this is the first valid edge */
                //std::cout << "values are: " << nodes_it << " " << edges_it << " " << nodes_.end() << "\n";
                return {nodes_it, edges_it, nodes_.end()};
            }
            ++edges_it;
        }
    }
    return cend();
}

template <typename N, typename E>
typename gdwg::Graph<N,E>::const_iterator gdwg::Graph<N,E>::cend() {
    return {nodes_.end(), {}, nodes_.end()};
}

// TODO: factor out similarity of this with cbegin()
template <typename N, typename E>
typename gdwg::Graph<N,E>::const_iterator& gdwg::Graph<N,E>::const_iterator::operator++() {
    /* we need to increment the edge iterator */
    ++edge_it_;
    /* if the edge iterator got to the end of the edges for
     * the current node, then we need to go to the next node */
    if (edge_it_ == (*node_it_)->outgoing_.end()) {
        for (++node_it_; node_it_ != node_end_it_; ++node_it_) {

            auto edges_it = (*node_it_)->outgoing_.begin();
            //auto edges_end = (*node_it_)->outgoing_.end();

            /* loop through the current nodes outgoing edges */
            for (; edges_it != (*node_it_)->outgoing_.end(); ++edges_it) {
                /* remove any expired wp's as we iterate through */
                if (edges_it->expired()) {
                    edges_it = ((*node_it_)->outgoing_).erase(edges_it);
                } else { /* not expired, so this is the first valid edge */
                    edge_it_ = edges_it;
                    return *this;
                }
            }
        }
        /* edge it needs to be {} so it compares == with end now */ // TODO: is this correct?
        edge_it_ = typename std::set<std::weak_ptr<Edge>>::iterator{};
    }
    return *this;
}