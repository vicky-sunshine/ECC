# Elliptic Curve Cryptography

Implementation of Elliptic Curve Cryptography over Finite Field Zp

## Overview
```shell
├── BigNumber          ## BigNumber Operation
├── FiniteFieldElement ## FiniteFieldElement Operation based on BigNumber
├── Point              ## Point element of Elliptic Curve
└── EllipticCurve      ## Main Cryptography method
```

## A Roundtrip Example
```C++

// The most important thing
// You Need to Define the Prime for Finite Field
BigNumber p =  BigNumber("FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF7FFFFFFF");
// since my mod_prime for FiniteFieldElement class is static...
// so you have to set it first
FiniteFieldElement::mod_prime = p;

// y^2 = x^3 + ax + b
FiniteFieldElement a =  FiniteFieldElement("FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF7FFFFFFC");
FiniteFieldElement b =  FiniteFieldElement("1C97BEFC54BD7A8B65ACF89F81D4D4ADC565FA45");

// 
FiniteFieldElement Gx = FiniteFieldElement("4A96B5688EF573284664698968C38BB913CBFC82");
FiniteFieldElement Gy = FiniteFieldElement("23A628553168947D59DCC912042351377AC5FB32");
BigNumber n =  BigNumber("0100000000000000000001F4C8F927AED3CA752257");
Point g = Point(Gx,Gy);
EllipticCurve ec = EllipticCurve(p,a,b,g,n);


std::cout << "<EC-ElGamal encryption>" << std::endl;

// encrypt input
std::string m = "FECDFF7C8E6F2C1DC6F7D5C3987AEDFC324DF6";
std::string Pa_encode = "039994C5C16070EE878F89A6143CE865AC2EC7EC5D"; // public key
BigNumber nk = BigNumber("5487CF3D6F9E4F1C3DAEF5C3CF7D6FC33C675DC6");
std::cout << "Plaintext M: " << m << std::endl;
std::cout << "Pa: " << Pa_encode << std::endl;
std::cout << "nk: " << nk << std::endl;

// encrypt
Point Pm = ec.data_embedded(m);
std::cout << "Mx: " << Pm.x << std::endl;
std::cout << "My: " << Pm.y << std::endl;


Point Pa = ec.decode(Pa_encode); // public key
std::vector<Point> Cm =  ec.elgamel_encrypt(Pm, nk, Pa);
std::cout << "Pk: " << ec.encode(Cm[0]) << std::endl;
std::cout << "Pb: " << ec.encode(Cm[1]) << std::endl;


std::cout << "\n<EC-ElGamal decryption>" << std::endl;

// decrypt input
BigNumber na = BigNumber("3C870C3E99245E0D1C06B747DEB3124DC843BB8B"); // private key
std::cout << "na: " << na << std::endl;

// decrypt
Point Pm_after = ec.elgamel_decrypt(Cm, na);
FiniteFieldElement m_after = ec.data_unembedded(Pm_after);
std::cout << "Plaintext:\n" << m_after << std::endl;

```
