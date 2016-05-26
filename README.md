# Elliptic Curve Cryptography

Implementation of Elliptic Curve Cryptography over Finite Field Zp.
Use EC-ElGamal for encryption/decryption.

## Overview
```shell
├── BigNumber          ## BigNumber Operation
├── FiniteFieldElement ## FiniteFieldElement Operation based on BigNumber
├── Point              ## Point element of Elliptic Curve
└── EllipticCurve      ## Main Cryptography method
```

## Data Embedded Method

When we get plaintext, we have to convert it to the `Point` Type.

In this code, we assume that the plaintext is 38-byte long, a little shorter than our BigNumber , which are in 40-Byte long.

```
Input: (m-8)-bit binary data M
Output: Point (Mx,My) on the elliptic curve

Mx = append(d,00)
while ( (Mx, My) is not on curve)
	increment Mx
	compute My (where My % 2 == 1)
return (Mx,My)
```

You can define your own embedded method



## Data Types and Conversions

Point Type is sometimes redundant. Once we have `x` coordinate of a Point, you can get `y` coordinate through the Elliptic Curve Equation: ` y^2 = x^3 + ax + b`.

There's standard  [spec of Data Conversion](http://www.secg.org/sec1-v2.pdf) (at Ch 2.3)



## A Roundtrip Example

You can also see this in my `EllipticCurve/main.cpp`

Notice that all my BigNumbers are in hex

```C++

// The most important thing
// You Need to Define the Prime for Finite Field
BigNumber p =  BigNumber("FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF7FFFFFFF");
// since my mod_prime for FiniteFieldElement class is static
// so you have to set it first
FiniteFieldElement::mod_prime = p;


// Define E (p, a, b, G, n)
// y^2 = x^3 + ax + b
FiniteFieldElement a =  FiniteFieldElement("FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF7FFFFFFC");
FiniteFieldElement b =  FiniteFieldElement("1C97BEFC54BD7A8B65ACF89F81D4D4ADC565FA45");
FiniteFieldElement Gx = FiniteFieldElement("4A96B5688EF573284664698968C38BB913CBFC82");
FiniteFieldElement Gy = FiniteFieldElement("23A628553168947D59DCC912042351377AC5FB32");
Point g = Point(Gx,Gy);
BigNumber n =  BigNumber("0100000000000000000001F4C8F927AED3CA752257");
EllipticCurve ec = EllipticCurve(p,a,b,g,n);

// encrypt input
std::string m = "FECDFF7C8E6F2C1DC6F7D5C3987AEDFC324DF6";
std::string Pa_encode = "039994C5C16070EE878F89A6143CE865AC2EC7EC5D"; // public key
BigNumber nk = BigNumber("5487CF3D6F9E4F1C3DAEF5C3CF7D6FC33C675DC6");
std::cout << "Plaintext M: " << m << std::endl;
std::cout << "Pa: " << Pa_encode << std::endl;
std::cout << "nk: " << nk << std::endl;

// encrypt
Point Pm = ec.data_embedded(m);	 // embedded plaintext to Point
Point Pa = ec.decode(Pa_encode); //
std::vector<Point> Cm =  ec.elgamel_encrypt(Pm, nk, Pa);
std::cout << "Pk: " << ec.encode(Cm[0]) << std::endl;
std::cout << "Pb: " << ec.encode(Cm[1]) << std::endl;


// decrypt input
BigNumber na = BigNumber("3C870C3E99245E0D1C06B747DEB3124DC843BB8B"); // private key
std::cout << "na: " << na << std::endl;

// decrypt
Point Pm_after = ec.elgamel_decrypt(Cm, na);
FiniteFieldElement m_after = ec.data_unembedded(Pm_after);
std::cout << "Plaintext:\n" << m_after << std::endl;
```
