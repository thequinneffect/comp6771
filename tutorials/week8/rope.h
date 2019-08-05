#ifndef TUTORIALS_WEEK8_ROPE_H_
#define TUTORIALS_WEEK8_ROPE_H_

#include <string>
#include <utility>
#include <vector>

class Rope {
  public:
    explicit Rope(std::vector<std::string> rope): rope_{std::move(rope)} {}

  class iterator {
    public:
      
      /* we define these aliases so that the std::iterator_traits<T> can
       * take our iterator in (i.e. std::iterator_traits<iterator>)
       * and set its members via them (it is an API);
       * - difference_type	Iter::difference_type
       * - value_type	Iter::value_type
       * - pointer	Iter::pointer
       * - reference	Iter::reference
       * - iterator_category	Iter::iterator_category
       * */

      /* the iterator tags are just empty structs 
       * (see https://en.cppreference.com/w/cpp/iterator/iterator_tags)
       * that act as a identifier/tag (hence the name) for the type
       * of iterator that you've implemented. This allows for the 
       * most optimum versions of algorithms to be used on your
       * iterator (e.g. forward vs random for advance() means using
       * "for (amount; amount--) fwd_it++;" vs "rnd_it += amount")  */
      using iterator_category = std::forward_iterator_tag;
      using value_type = char;
      using reference = char&;
      using pointer = char*;
      using difference_type = int; // i don't really understand this?

      reference operator*() const;
      iterator& operator++();
      iterator operator++(int) {
        auto copy{*this};
        ++(*this);
        return copy;
      }
      // This one isn't strictly required, but it's nice to have.
      pointer operator->() const { return &(operator*()); }

      friend bool operator==(const iterator& lhs, const iterator& rhs) {
        /* for the iterators to be considered equal (pointing at the same thing), then
         * the outer_ and inner_ iterators must be pointing to the same thing OR outer are
         * pointing to the same thing and both inner pointers are invalid because outer is
         * at the end. Note: order of checking here is important. Semantically it makes sense
         * to check outer first, but you HAVE to check that outer is outer end or not before
         * accessing inner because accessing inner when outer is end is bad */
        return lhs.outer_ == rhs.outer_ && (lhs.outer_ == lhs.outer_end_ || lhs.inner_ == rhs.inner_);
      }
      friend bool operator!=(const iterator& lhs, const iterator& rhs) { return !(lhs == rhs); }

  private:
    /* we need a vector<string> iterator to iterate over the strings as
     * a whole (each string in vector), but also a string iterator to 
     * iterate over the current string (each char in string).  */
    std::vector<std::string>::iterator outer_;
    std::string::iterator inner_;
    /* if we didn't have this, then how would we know we have reached
     * the end? also const because we don't want people to be able to
     * change the end of it. But what if someone adds a new string
     * to the vector of strings? the end will have to change. Yes, it
     * will, but this current iterator is INVALIDATED, so using it
     * is wrong by default, they would have to make a new iterator
     * to get the new updated end */
    const std::vector<std::string>::iterator outer_end_;

    friend class Rope;
    iterator(
      const decltype(outer_)& outer, // instead of writing "const std::vector<std::string>::iterator& outer" 
      const decltype(inner_)& inner,  // ditto
      const decltype(outer_end_)& outer_end) // ditto
      : outer_{outer}
      , inner_{inner}
      , outer_end_{outer_end} {}
  };

  /* an iterator must provide a method to get the iterating start point */
  iterator begin();
  /* must provide a method to get the point to stop iterating at */
  iterator end();

  private:
    std::vector<std::string> rope_;
};


#endif  // TUTORIALS_WEEK8_ROPE_H_
