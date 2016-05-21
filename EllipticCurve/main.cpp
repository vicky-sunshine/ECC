#include "BigNumber.hpp"
#include "Point.hpp"
#include "EllipticCurve.hpp"

int main(int argc, char** argv){

  //demo
  BigNumber p =  BigNumber("FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF7FFFFFFF");
  BigNumber a =  BigNumber("FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF7FFFFFFC");
  BigNumber b =  BigNumber("1C97BEFC54BD7A8B65ACF89F81D4D4ADC565FA45");
  BigNumber Gx = BigNumber("4A96B5688EF573284664698968C38BB913CBFC82");
  BigNumber Gy = BigNumber("23A628553168947D59DCC912042351377AC5FB32");
  BigNumber n =  BigNumber("0100000000000000000001F4C8F927AED3CA752257");
  Point g = Point(Gx,Gy);
  EllipticCurve ec = EllipticCurve(p,a,b,g,n);

  BigNumber na = BigNumber("3C870C3E99245E0D1C06B747DEB3124DC843BB8B"); // private key
  BigNumber nk = BigNumber("A61F035A7D0938251F5DD4CBFC96F5453B130D89"); 
  BigNumber mx = BigNumber("2923BE84E16CD6AE529049F1F1BBE9EBB3A6DB01");
  BigNumber my = BigNumber("A38CC9D23D61B446A4F51B2C8DA6BC6FC5CA2BAC");
  // Point m = Point(mx,my);
  // ec.elgamel_encrypt(m, ec.mult_n(g, na));
  BigNumber sum1 = (mx*mx*mx + a*mx+b)%p;
  BigNumber sum2 = (my*my) % p;
  std::cout << sum1 << std::endl;
  std::cout << sum2 << std::endl;
  BigNumber m = BigNumber("2923BE84E16CD6AE529049F1F1BBE9EBB3A6DB00");
  
  

  return 0;
}
