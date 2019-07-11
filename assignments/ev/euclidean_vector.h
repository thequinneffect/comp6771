// TODO(you): Include header guards

#include <exception>
#include <list>
#include <string>
#include <memory>
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
  friend bool operator==(const EuclideanVector& a, const EuclideanVector& b);
  friend bool operator!=(const EuclideanVector& a, const EuclideanVector& b); // NOTE: this is just inverse of above, re-use code 
  friend EuclideanVector operator+(const EuclideanVector& a, const EuclideanVector& b);
  friend EuclideanVector operator-(const EuclideanVector& a, const EuclideanVector& b);
  friend double operator*(const EuclideanVector& a, const EuclideanVector& b);
  friend EuclideanVector operator*(const EuclideanVector& ev, double scaler);
  friend EuclideanVector operator/(const EuclideanVector& ev, double scaler);
  friend std::ostream& operator<<(std::ostream& os, const EuclideanVector& ev);

  /* methods */
  double at(int i) const;
  double& at(int i);
  int GetNumDimensions() const;
  double GetEuclideanNorm();
  EuclideanVector CreateUnitVector();
  
  // TODO(you): add more
 
 private:
  int dimensions_;
  std::unique_ptr<double[]> magnitudes_;
  // TODO(you): add more
};
