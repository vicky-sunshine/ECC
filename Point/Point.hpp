#ifndef Point_hpp
#define Point_hpp

#include <iostream>
#include "BigNumber.hpp"

class Point{
public:
  // member
  BigNumber x;
  BigNumber y;
  
  // constructors
  Point();
  Point(const BigNumber& x, const BigNumber& y);
  
  // logical operation
  friend bool operator==(const Point&, const Point&);
  friend bool operator!=(const Point&, const Point&);

  // output format
  friend std::ostream& operator<<(std::ostream&, const Point&);
  
};

#endif /* Point_hpp */
