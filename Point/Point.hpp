#ifndef Point_hpp
#define Point_hpp

#include <iostream>
#include "BigNumber.hpp"
#include "FiniteFieldElement.hpp"

class Point{
public:
  // member
  FiniteFieldElement x;
  FiniteFieldElement y;
  
  // constructors
  Point();
  Point(const FiniteFieldElement& x, const FiniteFieldElement& y);
  
  // logical operation
  friend bool operator==(const Point&, const Point&);
  friend bool operator!=(const Point&, const Point&);

  // output format
  friend std::ostream& operator<<(std::ostream&, const Point&);
  
};

#endif /* Point_hpp */
