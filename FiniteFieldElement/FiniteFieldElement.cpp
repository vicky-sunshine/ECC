#include "FiniteFieldElement.hpp"
// Initial Finite Field Prime Number
BigNumber FiniteFieldElement::mod_prime = 1;

// private method
/*
 decryption
 use extend eculidean algorithm to find a, b s.t ap + bq = gcd(a,b)
 */
std::vector<BigNumber> FiniteFieldElement::extend_eculidean(BigNumber a, BigNumber b) const {
  BigNumber q,x,lastx,y,lasty,temp1,temp2,temp3;
  x=0;
  y=1;
  lastx=1;
  lasty=0;
  while (b!=0) {
    q= a/b;
    temp1= a%b;
    a=b;
    b=temp1;

    temp2 = x;
    x=lastx-q*x;
    lastx = temp2;

    temp3 = y;
    y = lasty-q*y;
    lasty=temp3;

  }
  std::vector<BigNumber> result{lastx, lasty};
  return result;
}



// constructors
FiniteFieldElement::FiniteFieldElement() {
  this->i = BigNumber(0);
}

FiniteFieldElement::FiniteFieldElement(long long input) {
  this->i = input % mod_prime;
}

FiniteFieldElement::FiniteFieldElement(const std::string& input) {
  this->i = BigNumber(input) % mod_prime;

}

FiniteFieldElement::FiniteFieldElement(BigNumber input) {
  this->i = input % mod_prime;
}


// overloaded logical operators as member functions
bool operator==(const FiniteFieldElement& lhs, const FiniteFieldElement& rhs) {
  return lhs.i == rhs.i;
}

bool operator!=(const FiniteFieldElement& lhs, const FiniteFieldElement& rhs) {
  return lhs.i != rhs.i;
}

bool operator>(const FiniteFieldElement& lhs, const FiniteFieldElement& rhs) {
  return lhs.i > rhs.i;
}

bool operator<(const FiniteFieldElement& lhs, const FiniteFieldElement& rhs) {
  return lhs.i < rhs.i;
}

bool operator>=(const FiniteFieldElement& lhs, const FiniteFieldElement& rhs) {
  return lhs.i >= rhs.i;
}

bool operator<=(const FiniteFieldElement& lhs, const FiniteFieldElement&rhs) {
  return lhs.i <= rhs.i;
}


// arithmetic
const FiniteFieldElement operator+(const FiniteFieldElement& lhs, const FiniteFieldElement& rhs) {
  BigNumber add = lhs.i + rhs.i;
  if (add < 0) {
    add = rhs.mod_prime + add;
  }

  return FiniteFieldElement(add % rhs.mod_prime);
}

const FiniteFieldElement operator-(const FiniteFieldElement& lhs, const FiniteFieldElement& rhs) {
  BigNumber sub = lhs.i - rhs.i;
  if (sub < 0) {
    sub = rhs.mod_prime + sub ;
  }
  return FiniteFieldElement(sub% rhs.mod_prime);
}

const FiniteFieldElement operator*(const FiniteFieldElement& lhs, const FiniteFieldElement& rhs) {
  BigNumber mul = lhs.i * rhs.i;
  if (mul < 0) {
    mul = rhs.mod_prime + mul;
  }
  return FiniteFieldElement(mul % rhs.mod_prime);
}

const FiniteFieldElement operator/(const FiniteFieldElement& lhs, const FiniteFieldElement& rhs) {
  BigNumber rhs_inv = inv(rhs).i;
  BigNumber div = lhs.i * rhs_inv % rhs.mod_prime;
  return FiniteFieldElement(div);
}

const FiniteFieldElement operator%(const FiniteFieldElement& lhs, const FiniteFieldElement& rhs) {
  BigNumber mod = lhs.i % rhs.i;
  if (mod < 0) {
    mod = rhs.mod_prime + mod;
  }
  return FiniteFieldElement(mod % rhs.mod_prime);
}

const FiniteFieldElement operator-(const FiniteFieldElement& rhs) {
  BigNumber sub = (rhs.mod_prime - rhs.i) % rhs.mod_prime;
  return FiniteFieldElement(sub);
}

const FiniteFieldElement inv(const FiniteFieldElement& rhs) {
  std::vector<BigNumber> result = rhs.extend_eculidean(rhs.i, rhs.mod_prime);
  BigNumber inv = result[0] % rhs.mod_prime;
  if (inv < 0 ) {
    inv =  (rhs.mod_prime + inv) % rhs.mod_prime;
  }

  return FiniteFieldElement(inv);
}

std::vector<FiniteFieldElement> sqrt(FiniteFieldElement& input) {

  BigNumber power = (input.mod_prime+1)/4;
  FiniteFieldElement r1 = pow(input, power);
  FiniteFieldElement r2 = (FiniteFieldElement::mod_prime - r1);
  std::vector<FiniteFieldElement> root{r1,r2};

  return root;
}

FiniteFieldElement pow(FiniteFieldElement& base, BigNumber& power) {
  FiniteFieldElement sum = base;
  if  (power ==0) return 1;

  FiniteFieldElement odd(1);
  while (base!=1 && power>1){
    if (power % 2 != 0){ // odd
      odd = odd * base;
    }
    power = power / 2;
    base = base * base;
  }

  return base * odd;
}

// output
std::ostream& operator<<(std::ostream& os, const FiniteFieldElement& rhs) {
  os << rhs.i;
  return os;
}

std::string FiniteFieldElement::to_string() const{
  std::stringstream ss;
  std::string str;
  ss << (*this).i;
  ss >> str;
  return str;
}
