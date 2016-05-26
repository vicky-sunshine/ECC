#include "EllipticCurve.hpp"

// private format conversion handler
const std::vector<Point> EllipticCurve::depressToPoint(const FiniteFieldElement& input) {
  
  FiniteFieldElement sum = input * input * input + a * input + b;
  std::vector<FiniteFieldElement> root = sqrt(sum);
  
  std::vector<Point> result = {Point(input,root[0]),Point(input,root[1])};
  return result;
}

// EllipticCurve Constructor
EllipticCurve::EllipticCurve(BigNumber& mod_prime, FiniteFieldElement& a, FiniteFieldElement& b, Point& generator, BigNumber& order) {
  FiniteFieldElement::mod_prime = mod_prime;
  this->mod_prime = mod_prime;
  this->a = a;
  this->b = b;
  this->generator = generator;
  this->order = order;
}

// Point Operation
const bool EllipticCurve::isOnCurve(const Point& p) {
  FiniteFieldElement sum;

  // y^2 mod prime =? x^3 + ax + b mod prime
  return (p.y * p.y) == ( p.x * p.x * p.x + a * p.x + b );
}

const bool EllipticCurve::isZero(const Point& p) {
  return p.x==0 && p.y==0;
}

const Point EllipticCurve::negative(const Point& p) {
  return Point(p.x, -p.y);
}

const Point EllipticCurve::addition(const Point& p, const Point& q) {
  Point r;
  FiniteFieldElement s;

  if (p == q) return doubling(p);

  if (isZero(p)) {
    return q;
  }
  if (isZero(q)) {
    return p;
  }

  if (p.x==q.x && p.y == -1*q.y) {
    return Point(0,0);
  }

  s = (p.y - q.y) /  (p.x - q.x);
  r.x = (s * s - p.x - q.x);
  r.y = (s * (p.x - r.x) - p.y);

  return r;
}

const Point EllipticCurve::doubling(const Point& p) {
  Point r;
  FiniteFieldElement s;

  if (isZero(p)) {
    return p;
  }
  
  s = (3 * p.x * p.x + a ) /  (2 * p.y);
  r.x = s * s - 2 * p.x;
  r.y = s * (p.x - r.x) - p.y;
  return r;
}

const Point EllipticCurve::mult_n(const Point& p, const BigNumber& n) {
  if (n == 1) return p;
  if (n == 2) return doubling(p);

  // init odd as ZERO
  Point sum = p;
  Point odd = Point (0,0);
  BigNumber num = n;
  while (num > 1){
    if (num % 2 != 0){ // odd
      odd = EllipticCurve::addition(sum ,odd);
    }
    num = num / 2;
    sum = EllipticCurve::doubling(sum);
  }
  sum = EllipticCurve::addition(sum, odd);

  return sum;
}


// Data embedded and unembedded
const Point EllipticCurve::data_embedded(std::string& data) {
  
  FiniteFieldElement Mx = FiniteFieldElement(data) * 256;

  std::vector<Point> M;
  while(1){
    M = depressToPoint(Mx);


    if(isOnCurve(M[0]) && (M[0].y % 2 == 1)) {
      return M[0];
    }
    if(isOnCurve(M[1]) && (M[1].y % 2 == 1)) {
      return M[1];
    }
    Mx = Mx+1;
  }

}

const std::string EllipticCurve::data_unembedded(Point& plaintext) {
  
  std::string result = plaintext.x.to_string();
  result.erase(result.end()-2, result.end());
  return result;
}


// format conversion
const Point EllipticCurve::decode(const std::string& input) {

  char ch = input[1]; // choose prefix
  
  // remove prfix to get x of Point
  std::string tempstr = input;
  tempstr.erase(tempstr.begin(),tempstr.begin()+2);
  FiniteFieldElement num = FiniteFieldElement(tempstr);
  
  // get two possible points
  std::vector<Point> result = depressToPoint(num);

  // choose y depends on prefix, 03->odd, 02->even
  if (result[0].y % 2 == 1) {
    // root[0] is odd
    if (ch=='3') {
      return Point(num,result[0].y);
    } else {
      return Point(num,result[1].y);
    }
  } else {
    // root[0] is even
    if (ch=='3') {
      return Point(num,result[1].y);
    } else {
      return Point(num,result[0].y);
    }
  }
}

const std::string EllipticCurve::encode(const Point& input) {
  std::string str = input.x.to_string();

  if (input.y % 2 == 0) {
    str.insert(0, "02");
  } else {
    str.insert(0, "03");
  }
  
  return str;
}


const std::vector<Point> EllipticCurve::elgamel_encrypt(Point& plaintext, BigNumber& nk, Point& public_key) {
  std::vector<Point> cipher;

  Point Pk = EllipticCurve::mult_n(generator, nk);
  Point P_pa_nk = EllipticCurve::mult_n(public_key, nk);
  Point Pb = EllipticCurve::addition(plaintext, P_pa_nk);

  // std::cout << c1 << std::endl;
  // std::cout << c2 << std::endl;
  cipher.push_back(Pk);
  cipher.push_back(Pb);

  return cipher;
}

const Point EllipticCurve::elgamel_decrypt(std::vector<Point>& ciphertext, BigNumber& private_key) {
  Point pk = ciphertext[0];
  Point pb = ciphertext[1];
  
  // Pm= Pb - na * Pk
  Point neg_pk = negative(mult_n(pk,private_key));
  Point pm = addition(pb, neg_pk);
  return pm;
}
