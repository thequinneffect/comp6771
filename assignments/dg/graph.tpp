//template <typename N, typename E>
//gdwg::Graph<N,E>::Graph() {}

#include <algorithm>
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
bool gdwg::Graph<N,E>::InsertNode(const N& val)
{
    return this->nodes_.insert(std::make_unique<Node>(val)).second;
}