#include "tutorials/week8/rope.h"

#include <algorithm>

Rope::iterator::reference Rope::iterator::operator*() const {
    return *inner_;
}

Rope::iterator& Rope::iterator::operator++() {
    /* we need to increment inner, but if inner gets to
    inner end then we need to go to the next outer
    BUT if the next outer is end or gets to an empty inner
    then we need to return or skip accordingly
    end */
    ++inner_;
    /* outer is an iterator to a string, so access the string
     and call the end method of that string to get an end() iterator of that string */
    if (inner_ == outer_->end()) { //(*outer).end() is string.end() of string that inner refers to
        do {
            ++outer_;
        // whilst we haven't got to the end && we are encountering empty inners
        } while (outer_ != outer_end_ && outer_->begin() == outer_->end());
        /* now we have either found a valid inner or got to the end */
        if (outer_ != outer_end_) {
            inner_ = outer_->begin();
        }
    }
    // we've updated the iterator, now return it
    return *this;
}

Rope::iterator Rope::begin() {
    auto first_valid_rope = std::find_if(rope_.begin(), rope_.end(), [] (const std::string& s) { return !s.empty(); });
    if (first_valid_rope != rope_.end()) {
        return Rope::iterator(first_valid_rope, first_valid_rope->begin(), rope_.end());
    }
    return end();
}

Rope::iterator Rope::end() {
    return Rope::iterator(rope_.end(), {}, rope_.end());
}