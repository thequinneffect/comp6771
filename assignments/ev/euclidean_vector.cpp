#include "assignments/ev/euclidean_vector.h"

#include <algorithm>  // these are helpful https://en.cppreference.com/w/cpp/algorithm
#include <cassert>
#include <iterator>
#include <memory>
#include <vector>

/* ctors */
EuclideanVector::EuclideanVector(int i) : EuclideanVector(i, 0) {}

EuclideanVector::EuclideanVector(int dimensions, double magnitudes) : 
    dimensions_ {dimensions},
    magnitudes_ { std::make_unique<double[]>(dimensions) } // value initialises i.e. 0.0 values
{
    if (magnitudes) std::fill_n(magnitudes_.get(), dimensions_, magnitudes); // ASK TUTOR: this works, but is it okay for .get to not be an iterator?
}

EuclideanVector::EuclideanVector(std::vector<double>::const_iterator begin, 
                                    std::vector<double>::const_iterator end) :
    dimensions_ { static_cast<int>(std::distance(begin, end)) },
    magnitudes_ { std::make_unique<double[]>(dimensions_) }
{
    std::copy_n(begin, dimensions_, magnitudes_.get()); // ASK TUTOR: this works, but is it okay for .get to not be an iterator?
}

EuclideanVector::EuclideanVector(const EuclideanVector& original) {
    
}


/* operator overloads */
double& EuclideanVector::operator[](int i) // setter
{
    assert(i >= 0 && i < dimensions_);
    return magnitudes_.get()[i];
}
double EuclideanVector::operator[](int i) const // getter
{
    assert(i >= 0 && i < dimensions_);
    return magnitudes_.get()[i];
}

/* methods */
int EuclideanVector::GetNumDimensions() const
{
    return this->dimensions_;
}