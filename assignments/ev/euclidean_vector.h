// TODO(you): Include header guards

#include <algorithm>
#include <exception>
#include <experimental/iterator>
#include <iterator>
#include <list>
#include <memory>
#include <numeric>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

class EuclideanVectorError : public std::exception {
 public:
  explicit EuclideanVectorError(const std::string& what) : what_(what) {}
  const char* what() const noexcept{ return what_.c_str(); }
 private:
  std::string what_;
};

class EuclideanVector {
 public:
  /* ctors */
  explicit EuclideanVector(int dimensions = 1); // default dimension 1 ctor (i dimensions, all magnitudes 0)
  EuclideanVector(int dimensions, double magnitudes); // i dimensions, all magnitudes double
  EuclideanVector(std::vector<double>::const_iterator begin, std::vector<double>::const_iterator end); // iterate over vector
  EuclideanVector(const EuclideanVector& original); // copy construct
  EuclideanVector(EuclideanVector&& recyclee) noexcept; // move construct ((1) noexcept as a failed move does not guarantee valid state)

  /* dtor */
  ~EuclideanVector() noexcept = default; // TODO: is this correct?

  /* operator overloads */
  EuclideanVector& operator=(const EuclideanVector& original); // copy assign
  EuclideanVector& operator=(EuclideanVector&& recyclee) noexcept; // move assign (see (1))
  double& operator[](int i); // return reference to value at index i (TODO: is double correct?)
  double operator[](int i) const; // return value at index i (TODO: is double correct?)
  EuclideanVector& operator+=(const EuclideanVector& other);
  EuclideanVector& operator-=(const EuclideanVector& other);
  EuclideanVector& operator*=(const double scaler);
  EuclideanVector& operator/=(const double scaler);
  explicit operator std::vector<double>();
  explicit operator std::list<double>();

  /* friend operator overloads */
  friend bool operator==(const EuclideanVector& a, const EuclideanVector& b) 
  {
    if (&a == &b) return true;
    if (a.dimensions_ != b.dimensions_) return false;
    return std::equal(a.magnitudes_.get(), a.magnitudes_.get()+a.dimensions_, b.magnitudes_.get());
  }

  friend bool operator!=(const EuclideanVector& a, const EuclideanVector& b)
  {
    return !(a == b);
  }

  friend EuclideanVector operator+(const EuclideanVector& a, const EuclideanVector& b)
  {
    if (a.dimensions_ != b.dimensions_) 
    {
        std::stringstream ss;
        ss << "Dimensions of LHS(" << a.dimensions_ << ") and RHS(" << b.dimensions_ << ") do not match";
        throw EuclideanVectorError(ss.str());
    }
    EuclideanVector new_ev {a.dimensions_};
    std::transform(a.magnitudes_.get(), a.magnitudes_.get() + a.dimensions_, b.magnitudes_.get(), new_ev.magnitudes_.get(), std::plus<double>());
    return new_ev;
  }

  friend EuclideanVector operator-(const EuclideanVector& a, const EuclideanVector& b)
  {
    if (a.dimensions_ != b.dimensions_) 
    {
        std::stringstream ss;
        ss << "Dimensions of LHS(" << a.dimensions_ << ") and RHS(" << b.dimensions_ << ") do not match";
        throw EuclideanVectorError(ss.str());
    }
    EuclideanVector new_ev {a.dimensions_};
    std::transform(a.magnitudes_.get(), a.magnitudes_.get()+a.dimensions_, b.magnitudes_.get(), new_ev.magnitudes_.get(), std::minus<double>());
    return new_ev;
  }

  friend double operator*(const EuclideanVector& a, const EuclideanVector& b)
  {
    if (a.dimensions_ != b.dimensions_) 
    {
        std::stringstream ss;
        ss << "Dimensions of LHS(" << a.dimensions_ << ") and RHS(" << b.dimensions_ << ") do not match";
        throw EuclideanVectorError(ss.str());
    }
    std::vector<double> sums;
    sums.reserve(a.dimensions_);
    std::transform(a.magnitudes_.get(), a.magnitudes_.get()+a.dimensions_, b.magnitudes_.get(), std::back_inserter(sums), std::multiplies<double>());
    return std::accumulate(sums.begin(), sums.end(), 0.0);
  }

  friend EuclideanVector operator*(const EuclideanVector& ev, double scaler)
  {
    EuclideanVector scaled_ev {ev.dimensions_};
    std::transform(ev.magnitudes_.get(), ev.magnitudes_.get()+ev.dimensions_, scaled_ev.magnitudes_.get(), [scaler](double mag){return mag*=scaler;});
    return scaled_ev;
  }

  friend EuclideanVector operator*(double scaler, const EuclideanVector& ev)
  {
    return (ev * scaler);
  }

  friend EuclideanVector operator/(const EuclideanVector& ev, double scaler)
  {
    if (scaler == 0) throw EuclideanVectorError("Invalid vector division by 0");
    EuclideanVector scaled_ev {ev.dimensions_};
    std::transform(ev.magnitudes_.get(), ev.magnitudes_.get()+ev.dimensions_, scaled_ev.magnitudes_.get(), [scaler](double mag){return mag/=scaler;});
    return scaled_ev;
  }

  friend std::ostream& operator<<(std::ostream& os, const EuclideanVector& ev)
  {
    os << "[";
    std::copy(ev.magnitudes_.get(), ev.magnitudes_.get()+ev.dimensions_, std::experimental::make_ostream_joiner(os, " "));
    os << "]";
    return os;
  }

  /* methods */
  double at(int i) const;
  double& at(int i);
  int GetNumDimensions() const;
  double GetEuclideanNorm() const;
  EuclideanVector CreateUnitVector() const;
  
  // TODO(you): add more
 
 private:
  int dimensions_;
  std::unique_ptr<double[]> magnitudes_;
  // TODO(you): add more
};
