#ifndef FiniteFieldElement_hpp
#define FiniteFieldElement_hpp

#include <iostream>
#include <sstream>
#include "BigNumber.hpp"

class FiniteFieldElement{
private:
  std::vector<BigNumber> extend_eculidean(BigNumber a, BigNumber b) const;
public:
  // member
  static BigNumber mod_prime; // let all Finite Field Share the same memory
  BigNumber i;

  // constructors
  FiniteFieldElement();
  FiniteFieldElement(long long); // directly convert from an int
  FiniteFieldElement(const std::string&);
  FiniteFieldElement(BigNumber);

  // overloaded logical operators as member functions
  friend bool operator==(const FiniteFieldElement&, const FiniteFieldElement&);
  friend bool operator!=(const FiniteFieldElement&, const FiniteFieldElement&);
  friend bool operator>(const FiniteFieldElement&, const FiniteFieldElement&);
  friend bool operator<(const FiniteFieldElement&, const FiniteFieldElement&);
  friend bool operator>=(const FiniteFieldElement&, const FiniteFieldElement&);
  friend bool operator<=(const FiniteFieldElement&, const FiniteFieldElement&);

  // overloaded arithmetic operators as member functions
  friend const FiniteFieldElement operator+(const FiniteFieldElement&, const FiniteFieldElement&);
  friend const FiniteFieldElement operator-(const FiniteFieldElement&, const FiniteFieldElement&);
  friend const FiniteFieldElement operator*(const FiniteFieldElement&, const FiniteFieldElement&);
  friend const FiniteFieldElement operator/(const FiniteFieldElement&, const FiniteFieldElement&);
  friend const FiniteFieldElement operator%(const FiniteFieldElement&, const FiniteFieldElement&);
  friend const FiniteFieldElement operator-(const FiniteFieldElement&);

  friend const FiniteFieldElement inv(const FiniteFieldElement&);
  friend std::vector<FiniteFieldElement> sqrt(FiniteFieldElement&);
  friend FiniteFieldElement pow(FiniteFieldElement& base, BigNumber& power);

  // ouput format for FiniteFieldElement
  friend std::ostream& operator<<(std::ostream&, const FiniteFieldElement&);
  std::string to_string() const;

};

#endif /* FiniteFieldElement_hpp */
