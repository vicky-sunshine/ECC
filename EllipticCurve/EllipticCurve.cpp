#include "EllipticCurve.hpp"


// EllipticCurve class
EllipticCurve::EllipticCurve(BigNumber& mod_prime, BigNumber& a, BigNumber& b, Point& generator, BigNumber& order) {
  this->mod_prime = mod_prime;
  this->a = a;
  this->b = b;
  this->generator = generator;
  this->order = order;
}

const bool EllipticCurve::isOnCurve(const Point& p) {
  BigNumber sum;
  
  // y^2 mod prime =? x^3 + ax + b mod prime
  return (p.y * p.y) % mod_prime == ( p.x * p.x * p.x + a * p.x + b ) % mod_prime;
}

const Point EllipticCurve::negative(const Point& p) {
  return Point(p.x, (-1 * p.y) % mod_prime);
}

const Point EllipticCurve::addition(const Point& p, const Point& q) {
  Point r;
  BigNumber s;
  
  if ( p == q) return doubling(p);
  
  s = ((p.y - q.y) / (p.x -q.x)) % mod_prime;
  r.x = (s * s - p.x - q.x) % mod_prime;
  r.y = (s * (p.x - r.x) - p.y) % mod_prime;
  
  return r;
}

const Point EllipticCurve::doubling(const Point& p) {
  Point r;
  BigNumber s;
  s = (3 * p.x * p.x + a) / (2 * p.y ) % mod_prime;
  r.x = (s * s - 2 * p.x) % mod_prime;
  r.y = (s * (p.x - r.x) - 1 * p.y) % mod_prime;
  return r;
}

const Point EllipticCurve::mult_n(const Point& p, const BigNumber& n) {
  if (n == 1) return p;
  if (n == 2) return doubling(p);
  
  // init odd as ZERO
  Point sum = p;
  Point odd = p;
  BigNumber num = n - 1;

  while (num > 2){
    if (num % 2 != 0){ // odd
      odd = EllipticCurve::addition(sum ,odd);
    }
    num = num / 2;
    sum = EllipticCurve::doubling(sum);
  }
  sum = EllipticCurve::addition(sum, odd);
  return sum;
}

const std::vector<Point> EllipticCurve::elgamel_encrypt(Point m, Point public_key) {
  std::vector<Point> cipher;
  
  // random nk
  BigNumber nk = BigNumber("A61F035A7D0938251F5DD4CBFC96F5453B130D89");
  Point c1 = EllipticCurve::mult_n(generator, nk);
  Point c2 = EllipticCurve::addition(m, EllipticCurve::mult_n(public_key, nk));
  
  // std::cout << c1 << std::endl;
  // std::cout << c2 << std::endl;
  cipher.push_back(c1);
  cipher.push_back(c2);
  
  return cipher;
}

