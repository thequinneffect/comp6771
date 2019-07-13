#include "assignments/ev/euclidean_vector.h"

#include <algorithm>  // these are helpful https://en.cppreference.com/w/cpp/algorithm
#include <cassert>
#include <cmath>
#include <functional>
#include <iterator>
#include <list>
#include <memory>
#include <numeric>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

/* ctors */
EuclideanVector::EuclideanVector(int i) : EuclideanVector(i, 0.0) {}

EuclideanVector::EuclideanVector(int dimensions, double magnitude)
  : dimensions_{dimensions}, magnitudes_{std::make_unique<double[]>(dimensions)} {
  std::fill_n(magnitudes_.get(), dimensions_, magnitude);
}

EuclideanVector::EuclideanVector(std::vector<double>::const_iterator begin,
                                 std::vector<double>::const_iterator end)
  : dimensions_{static_cast<int>(std::distance(begin, end))},
    magnitudes_{std::make_unique<double[]>(dimensions_)} {
  std::copy_n(begin, dimensions_, magnitudes_.get());
}

EuclideanVector::EuclideanVector(const EuclideanVector& original)
  : dimensions_{original.dimensions_}, magnitudes_{
                                           std::make_unique<double[]>(original.dimensions_)} {
  std::copy_n(original.magnitudes_.get(), original.dimensions_, magnitudes_.get());
}

EuclideanVector::EuclideanVector(EuclideanVector&& recyclee) noexcept
  : dimensions_{recyclee.dimensions_}, magnitudes_{std::move(recyclee.magnitudes_)} {
  recyclee.dimensions_ = 0;
}

/* operator overloads */
EuclideanVector& EuclideanVector::operator=(const EuclideanVector& original) {
  dimensions_ = original.dimensions_;
  magnitudes_ = std::make_unique<double[]>(original.dimensions_);
  std::copy_n(original.magnitudes_.get(), original.dimensions_, magnitudes_.get());
  return *this;
}

EuclideanVector& EuclideanVector::operator=(EuclideanVector&& recyclee) noexcept {
  dimensions_ = recyclee.dimensions_;
  magnitudes_ = std::move(recyclee.magnitudes_);
  recyclee.dimensions_ = 0;
  return *this;
}

double& EuclideanVector::operator[](int i) {
  assert(i >= 0 && i < dimensions_);
  return magnitudes_.get()[i];
}
double EuclideanVector::operator[](int i) const {
  assert(i >= 0 && i < dimensions_);
  return magnitudes_.get()[i];
}

EuclideanVector& EuclideanVector::operator+=(const EuclideanVector& other) {
  if (dimensions_ != other.dimensions_) {
    std::stringstream ss;
    ss << "Dimensions of LHS(" << dimensions_ << ") and RHS(" << other.dimensions_
       << ") do not match";
    throw EuclideanVectorError(ss.str());
  }
  std::transform(magnitudes_.get(), magnitudes_.get() + dimensions_, other.magnitudes_.get(),
                 magnitudes_.get(), std::plus<double>());
  return *this;
}

EuclideanVector& EuclideanVector::operator-=(const EuclideanVector& other) {
  if (dimensions_ != other.dimensions_) {
    std::stringstream ss;
    ss << "Dimensions of LHS(" << dimensions_ << ") and RHS(" << other.dimensions_
       << ") do not match";
    throw EuclideanVectorError(ss.str());
  }
  std::transform(magnitudes_.get(), magnitudes_.get() + dimensions_, other.magnitudes_.get(),
                 magnitudes_.get(), std::minus<double>());
  return *this;
}

EuclideanVector& EuclideanVector::operator*=(const double scaler) {
  std::transform(magnitudes_.get(), magnitudes_.get() + dimensions_, magnitudes_.get(),
                 [scaler](double& mag) { return mag *= scaler; });
  return *this;
}

EuclideanVector& EuclideanVector::operator/=(const double scaler) {
  if (scaler == 0)
    throw EuclideanVectorError("Invalid vector division by 0");
  std::transform(magnitudes_.get(), magnitudes_.get() + dimensions_, magnitudes_.get(),
                 [scaler](double& mag) { return mag /= scaler; });
  return *this;
}

EuclideanVector::operator std::vector<double>() const {
  std::vector<double> vec;
  vec.reserve(dimensions_);
  std::copy(magnitudes_.get(), magnitudes_.get() + dimensions_, std::back_inserter(vec));
  return vec;
}

EuclideanVector::operator std::list<double>() const {
  std::list<double> list;
  std::copy(magnitudes_.get(), magnitudes_.get() + dimensions_, std::back_inserter(list));
  return list;
}

/* methods */
double EuclideanVector::at(int i) const {
  if (i < 0 || i >= dimensions_) {
    std::stringstream ss;
    ss << "Index " << i << " is not valid for this EuclideanVector object";
    throw EuclideanVectorError(ss.str());
  }
  return magnitudes_.get()[i];
}

double& EuclideanVector::at(int i) {
  if (i < 0 || i >= dimensions_) {
    std::stringstream ss;
    ss << "Index " << i << " is not valid for this EuclideanVector object";
    throw EuclideanVectorError(ss.str());
  }
  return magnitudes_.get()[i];
}

int EuclideanVector::GetNumDimensions() const {
  return this->dimensions_;
}

double EuclideanVector::GetEuclideanNorm() const {
  if (dimensions_ == 0)
    throw EuclideanVectorError("EuclideanVector with no dimensions does not have a norm");
  std::vector<double> squares;
  squares.reserve(dimensions_);
  std::transform(magnitudes_.get(), magnitudes_.get() + dimensions_, std::back_inserter(squares),
                 [](double mag) { return mag * mag; });
  return std::sqrt(std::accumulate(squares.begin(), squares.end(), 0.0));
}

EuclideanVector EuclideanVector::CreateUnitVector() const {
  if (dimensions_ == 0)
    throw EuclideanVectorError("EuclideanVector with no dimensions does not have a unit vector");
  double norm = this->GetEuclideanNorm();
  if (norm == 0)
    throw EuclideanVectorError(
        "EuclideanVector with euclidean normal of 0 does not have a unit vector");
  EuclideanVector unit_vec{dimensions_};
  std::transform(magnitudes_.get(), magnitudes_.get() + dimensions_, unit_vec.magnitudes_.get(),
                 [norm](double mag) { return mag / norm; });
  return unit_vec;
}
