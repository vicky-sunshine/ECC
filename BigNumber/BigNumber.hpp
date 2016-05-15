#ifndef BigNumber_hpp
#define BigNumber_hpp

#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdint>

#define BIGGER 1
#define SMALLER -1
#define EQUAL 0

// Big number class definition
class BigNumber{
private:
  bool sgn;
  std::vector<int8_t> data;
  static int abs_compare(const BigNumber&, const BigNumber&);
  static void discard_leading_zero(std::vector<int8_t>&);

public:
  // constructors
  BigNumber();
  BigNumber(long long); // directly convert from an int
  BigNumber(const std::string&);
  BigNumber(bool, const std::vector<int8_t>&);
  BigNumber(bool, std::vector<int8_t>&&);

  // overloaded logical operators as member functions
  friend bool operator==(const BigNumber&, const BigNumber&);
  friend bool operator!=(const BigNumber&, const BigNumber&);
  friend bool operator>(const BigNumber&, const BigNumber&);
  friend bool operator<(const BigNumber&, const BigNumber&);
  friend bool operator>=(const BigNumber&, const BigNumber&);
  friend bool operator<=(const BigNumber&, const BigNumber&);

  // overloaded arithmetic operators as member functions
  friend const BigNumber operator+(const BigNumber&, const BigNumber&);
  friend const BigNumber operator-(const BigNumber&, const BigNumber&);
  friend const BigNumber operator*(const BigNumber&, const BigNumber&);
  friend const BigNumber operator/(const BigNumber&, const BigNumber&);
  friend const BigNumber operator%(const BigNumber&, const BigNumber&);

  // ouput format for BigNumber
  friend std::ostream& operator<<(std::ostream&, const BigNumber&);
};


#endif /* BigNumber_hpp */
