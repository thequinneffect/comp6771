#include "assignments/ev/euclidean_vector.h"

#include <algorithm>  // these are helpful https://en.cppreference.com/w/cpp/algorithm
#include <cassert>
#include <iterator>
#include <list>
#include <memory>
#include <sstream>
#include <utility>
#include <vector>

/* ctors */
EuclideanVector::EuclideanVector(int i) : EuclideanVector(i, 0.0) {}

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

EuclideanVector::EuclideanVector(const EuclideanVector& original) :
    dimensions_ {original.dimensions_},
    magnitudes_ { std::make_unique<double[]>(original.dimensions_) }
{
    std::copy_n(original.magnitudes_.get(), original.dimensions_, magnitudes_.get());
}

EuclideanVector::EuclideanVector(EuclideanVector&& recyclee) noexcept : 
    dimensions_ {recyclee.dimensions_}, // don't need to std::move a primitive type
    magnitudes_ { std::move(recyclee.magnitudes_) }
{
    recyclee.dimensions_ = 0;
}

/* operator overloads */
EuclideanVector& EuclideanVector::operator=(const EuclideanVector& original)
{
    dimensions_ = original.dimensions_;
    magnitudes_.reset(nullptr);
    magnitudes_ = std::make_unique<double[]>(original.dimensions_);
    std::copy_n(original.magnitudes_.get(), original.dimensions_, magnitudes_.get());
    return *this;
}

EuclideanVector& EuclideanVector::operator=(EuclideanVector&& recyclee) noexcept
{
    dimensions_ = recyclee.dimensions_;
    magnitudes_.reset(nullptr);
    magnitudes_ = std::move(recyclee.magnitudes_);
    recyclee.dimensions_ = 0;
    return *this;
}

/*
ASK TUTOR: why in the provided move assign do they not delete magnitudes_ current memory? see below
            is there an implicit delete happening here?

operator=(EuclideanVector&& o) noexcept {
  magnitudes_ = std::move(o.magnitudes_);
  size_ = o.size_;
  o.size = 0;
}
 */

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

/*
#include <algorithm>
 
template <class Type, size_t size>
void addArrays(const Type(&a)[size],
               const Type(&b)[size],
               Type(&result)[size])
{
    std::transform(a, a + size, b, result, std::plus<Type>());
} */

EuclideanVector& EuclideanVector::operator+=(const EuclideanVector& other)
{
    if (dimensions_ != other.dimensions_) 
    {
        std::stringstream ss;
        ss << "Dimensions of LHS(" << dimensions_ << ") and RHS(" << other.dimensions_ << ") do not match";
        throw EuclideanVectorError(ss.str());
    }
    std::transform(magnitudes_.get(), magnitudes_.get() + dimensions_, other.magnitudes_.get(), magnitudes_.get(), std::plus<double>());
    return *this;
}

EuclideanVector& EuclideanVector::operator-=(const EuclideanVector& other)
{
    if (dimensions_ != other.dimensions_) 
    {
        std::stringstream ss;
        ss << "Dimensions of LHS(" << dimensions_ << ") and RHS(" << other.dimensions_ << ") do not match";
        throw EuclideanVectorError(ss.str());
    }
    std::transform(magnitudes_.get(), magnitudes_.get() + dimensions_, other.magnitudes_.get(), magnitudes_.get(), std::minus<double>());
    return *this;
}

EuclideanVector& EuclideanVector::operator*=(const double scaler)
{
    //std::transform(myarray.begin(), myarray.end(), myarray.begin(), [myconstant](auto& c){return c*myconstant;});
    std::transform(magnitudes_.get(), magnitudes_.get() + dimensions_, magnitudes_.get(), [scaler](double& mag){return mag*=scaler;});
    return *this;
}

EuclideanVector& EuclideanVector::operator/=(const double scaler)
{
    if (scaler == 0.0) throw EuclideanVectorError("Invalid vector division by 0");
    std::transform(magnitudes_.get(), magnitudes_.get() + dimensions_, magnitudes_.get(), [scaler](double& mag){return mag/=scaler;});
    return *this;
}

EuclideanVector::operator std::vector<double>()
{
    std::vector<double> vec;
    std::copy(magnitudes_.get(), magnitudes_.get()+dimensions_, vec.begin());
    return vec;
}

EuclideanVector::operator std::list<double>()
{
    std::list<double> list;
    std::copy(magnitudes_.get(), magnitudes_.get()+dimensions_, list.begin());
    return list;
}

/* methods */
int EuclideanVector::GetNumDimensions() const
{
    return this->dimensions_;
}