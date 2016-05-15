//
//  EllipticCurve.hpp
//  EllipticCurve
//
//  Created by Hsnl on 2016/5/14.
//  Copyright © 2016年 Hsnl. All rights reserved.
//

#ifndef EllipticCurve_hpp
#define EllipticCurve_hpp

#include <iostream>
#include "BigNumber.hpp"

class Point{
public:
  // member
  BigNumber x;
  BigNumber y;
  
  //constructors
  Point();
  Point(const BigNumber& x, const BigNumber& y);
  friend std::ostream& operator<<(std::ostream&, const Point&);
  
  friend bool operator==(const Point&, const Point&);
  friend bool operator!=(const Point&, const Point&);
};

// EllipticCurve class definition
class EllipticCurve{
private:
  BigNumber mod_prime;
  BigNumber a;
  BigNumber b;
  Point generator;
  BigNumber order;
  
public:
  // constructors
  // p,a,b,G,n
  EllipticCurve(BigNumber& mod_prime, BigNumber& a, BigNumber& b, Point& G, BigNumber& order);
  
  const bool isOnCurve(const Point&);
  const Point addition(const Point&, const Point&);
  const Point negative(const Point&);
  const Point doubling(const Point&);
  const Point mult_n(const Point&, const BigNumber&);

  const std::vector<Point> elgamel_encrypt(Point m, Point public_key);
  // ouput format for BigNumber
  friend std::ostream& operator<<(std::ostream&, const EllipticCurve&);
};

#endif /* EllipticCurve_hpp */
