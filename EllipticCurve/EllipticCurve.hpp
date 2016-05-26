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
#include "FiniteFieldElement.hpp"
#include "BigNumber.hpp"
#include "Point.hpp"


// EllipticCurve class definition
class EllipticCurve{
private:
  BigNumber mod_prime;
  FiniteFieldElement a;
  FiniteFieldElement b;
  Point generator;
  BigNumber order;

  // Format Conversion
  const std::vector<Point> depressToPoint(const FiniteFieldElement&);
  const FiniteFieldElement compressToFiniteFieldElement(const Point&);

public:
  // constructors
  // p,a,b,G,n
  EllipticCurve(BigNumber& mod_prime, FiniteFieldElement& a, FiniteFieldElement& b, Point& G, BigNumber& order);

  // Point Operation in this Elliptic Curve
  const bool isOnCurve(const Point&);
  const bool isZero(const Point&);
  const Point addition(const Point&, const Point&);
  const Point negative(const Point&);
  const Point doubling(const Point&);
  const Point mult_n(const Point&, const BigNumber&);

  // Data embedded and unembedded
  const Point data_embedded(std::string& data);
  const std::string data_unembedded(Point& plaintext);

  // Format Conversion
  const Point decode(const std::string&);
  const std::string encode(const Point&);

  // encrypt and decrypt
  const std::vector<Point> elgamel_encrypt(Point& plaintext, BigNumber& nk, Point& pa);
  const Point elgamel_decrypt(std::vector<Point>& c, BigNumber& public_key);

  // ouput format for FiniteFieldElement
  friend std::ostream& operator<<(std::ostream&, const EllipticCurve&);
};

#endif /* EllipticCurve_hpp */
