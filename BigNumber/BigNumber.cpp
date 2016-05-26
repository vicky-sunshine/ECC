#include "BigNumber.hpp"

// constructor
BigNumber::BigNumber() {
  BigNumber(0);
}

BigNumber::BigNumber(long long input_number) {
  long long unsign_number;

  // determine its positive(true) or negative(false)
  sgn = !(input_number < 0);

  // make number positive
  unsign_number = (input_number < 0)? -input_number:input_number;

  // turn this integer to hex and store it to data
  while (unsign_number >= 16) {
    data.push_back(unsign_number & 15); // mod 16
    unsign_number = unsign_number >> 4; // div 16
  }
  data.push_back(unsign_number);
}

BigNumber::BigNumber(const std::string& input_string) {
  sgn = !(input_string.front() == '-');

  for (auto i = input_string.rbegin(), end = input_string.rend(); i != end; ++i) {
    if (*i >= '0' && *i <= '9') {
      data.push_back(*i - '0');
    } else if (*i >= 'A' && *i <= 'F') {
      data.push_back(*i - 'A' + 10);
    } else if (*i >= 'a' && *i <= 'f') {
      data.push_back(*i - 'a' + 10);
    }
  }
}

BigNumber::BigNumber(bool input_sgn, const std::vector<int8_t>& input_data) {
  sgn = input_sgn;
  data = input_data;
}

BigNumber::BigNumber(bool input_sgn, std::vector<int8_t>&& input_data) {
  sgn = input_sgn;
  data = std::move(input_data);
}


// logical operators
bool operator==(const BigNumber& lhs, const BigNumber& rhs) {
  return (lhs.sgn == rhs.sgn) && (BigNumber::abs_compare(lhs, rhs) == EQUAL);
}

bool operator!=(const BigNumber& lhs, const BigNumber& rhs) {
  return !(lhs == rhs);
}

bool operator>(const BigNumber& lhs, const BigNumber& rhs) {
  int abs_cmp;

  if (lhs.sgn == rhs.sgn) {
    abs_cmp = BigNumber::abs_compare(lhs, rhs);
    return ((lhs.sgn && abs_cmp == BIGGER) || (!lhs.sgn && abs_cmp == SMALLER));
  } else {
    return lhs.sgn;
  }
}

bool operator<(const BigNumber& lhs, const BigNumber& rhs) {
  return rhs > lhs;
}

bool operator>=(const BigNumber& lhs, const BigNumber& rhs) {
  return !(lhs < rhs);
}

bool operator<=(const BigNumber& lhs, const BigNumber& rhs) {
  return !(lhs > rhs);
}


// private method
int BigNumber::abs_compare(const BigNumber& lhs, const BigNumber& rhs) {
  if (lhs.data.size() > rhs.data.size()) {
    return BIGGER;
  } else if (lhs.data.size() < rhs.data.size()) {
    return SMALLER;
  }

  // same size
  for (auto i = lhs.data.rbegin(), j = rhs.data.rbegin(), end = lhs.data.rend(); i != end; ++i, ++j) {
    if(*i > *j) {
      return BIGGER;
    } else if (*i < *j) {
      return SMALLER;
    }
  }

  return EQUAL;
}

void BigNumber::discard_leading_zero(std::vector<int8_t>& input) {
  while (input.back() == 0 && input.size()!=1) {
    input.pop_back();
  }
}


// arithmetic operators
const BigNumber operator+(const BigNumber& lhs, const BigNumber& rhs) {
  bool sgn;
  std::vector<int8_t> abs_result;
  unsigned long min_size;
  int8_t carry, sum;

  min_size = (lhs.data.size() < rhs.data.size())? lhs.data.size():rhs.data.size();

  // check is add or sub
  if (lhs.sgn == rhs.sgn) {
    // same sign
    sgn = lhs.sgn;
    carry = 0;

    // add all first
    for (unsigned long i = 0; i < min_size; i++) {
      sum = lhs.data[i] + rhs.data[i];
      abs_result.push_back(sum);
    }

    // insert remain digit
    if (lhs.data.size() > rhs.data.size()) {
      for (unsigned long i = min_size; i < lhs.data.size(); i++) {
        abs_result.push_back(lhs.data[i]);
      }
    } else {
      for (unsigned long i = min_size; i < rhs.data.size(); i++) {
        abs_result.push_back(rhs.data[i]);
      }
    }

    // handle carry
    carry = 0;
    for (unsigned long i = 0; i < abs_result.size(); i++) {
      sum = abs_result[i] + carry;
      abs_result[i] = sum & 15;
      carry = sum >> 4;
    }
    if (carry > 0) {
      abs_result.push_back(carry);
    }
  } else {
    if (!lhs.sgn) {
      return rhs - BigNumber(!lhs.sgn, lhs.data);
    } else {
      return lhs - BigNumber(!rhs.sgn, rhs.data);
    }
  }

  return BigNumber(sgn, std::move(abs_result));
}

const BigNumber operator-(const BigNumber& lhs, const BigNumber& rhs) {
  bool sgn;
  std::vector<int8_t> abs_result;
  unsigned long min_size;

  // check is add or sub
  if (lhs.sgn != rhs.sgn) {
    // equal to do ADD operation
    return lhs + BigNumber(!rhs.sgn, rhs.data);
  }
  else {
    if (BigNumber::abs_compare(lhs, rhs) == EQUAL) {
      return BigNumber(0);
    }
    else if (BigNumber::abs_compare(lhs, rhs) == BIGGER) {
      sgn = lhs.sgn;
      // sub all first
      min_size = rhs.data.size();
      abs_result.resize(lhs.data.size());
      std::transform(lhs.data.begin(), lhs.data.begin() + min_size, rhs.data.begin(), abs_result.begin(), std::minus<int8_t>());
      // insert remain digit
      std::copy(lhs.data.begin() + min_size, lhs.data.end(), abs_result.begin() + min_size);
    }
    else {
      sgn = !rhs.sgn;
      // sub all first
      min_size = lhs.data.size();
      abs_result.resize(rhs.data.size());
      std::transform(rhs.data.begin(), rhs.data.begin() + min_size, lhs.data.begin(), abs_result.begin(), std::minus<int8_t>());
      // insert remain digit
      std::copy(rhs.data.begin() + min_size, rhs.data.end(), abs_result.begin() + min_size);
    }
    // handle borrow
    for (unsigned long i = 0; i < abs_result.size(); i++) {
      if (abs_result[i] < 0){
        abs_result[i] += 16;
        abs_result[i + 1]--;
      }
    }

    //discard redundant zero
    BigNumber::discard_leading_zero(abs_result);
  }

  return BigNumber(sgn, std::move(abs_result));
}

const BigNumber operator*(const BigNumber& lhs, const BigNumber& rhs) {
  bool sgn;
  std::vector<int8_t> abs_result;

  if(lhs == 0 || rhs == 0){
    return BigNumber(0);
  }

  sgn = (lhs.sgn == rhs.sgn);

  abs_result.resize(lhs.data.size() + rhs.data.size(), 0);
  int16_t sum = 0;
  for(unsigned long i = 0; i < lhs.data.size(); i++){
    for(unsigned long j = 0; j < rhs.data.size(); j++){
      sum = abs_result[i + j] + lhs.data[i] * rhs.data[j];
      abs_result[i + j] = sum % 16; // sum
      abs_result[i + j + 1] += sum / 16; // carry
    }
  }
  //discard redundant zero
  BigNumber::discard_leading_zero(abs_result);

  return BigNumber(sgn, abs_result);
}

const BigNumber operator/(const BigNumber& lhs, const BigNumber& rhs) {
  BigNumber temp(0);
  BigNumber remainder(true, lhs.data);
  BigNumber divisor(true, rhs.data);
  BigNumber quotient(0);
  quotient.sgn = (lhs.sgn == rhs.sgn);

  int8_t count = 0;
  while (remainder.data.size() != 0) {
    while(temp < divisor && remainder.data.size() != 0) {
      temp.data.insert(temp.data.begin(), remainder.data.back());
      remainder.data.pop_back();
      quotient.data.insert(quotient.data.begin(), 0);
    }
    BigNumber::discard_leading_zero(temp.data);

    count = 0;
    while (temp >= divisor) {
      count ++;
      temp = temp - divisor;
    }
    quotient.data.front() = count;
  }
  BigNumber::discard_leading_zero(quotient.data);

  // make -0 -> +0
  if (quotient.data.size()==1 && quotient.data.back()==0) {
    quotient.sgn = true;
  }

  return quotient;
}

const BigNumber operator%(const BigNumber& lhs, const BigNumber& rhs) {
  BigNumber temp(0);
  BigNumber remainder(true, lhs.data);
  BigNumber divisor(true, rhs.data);

  if (BigNumber::abs_compare(lhs, rhs)==EQUAL) {
    return BigNumber(0);
  }
  if (BigNumber::abs_compare(lhs, rhs)==SMALLER) {
    return lhs;
  }
  while (remainder >= divisor) {
    temp.data.assign(remainder.data.end() - divisor.data.size(), remainder.data.end());
    remainder.data.erase(remainder.data.end() - divisor.data.size(), remainder.data.end());
    if (temp < divisor){
      temp.data.insert(temp.data.begin(), remainder.data.back());
      remainder.data.pop_back();
    }

    BigNumber::discard_leading_zero(temp.data);

    while (temp >= divisor) {
      temp = temp - divisor;
    }
    remainder.data.insert(remainder.data.end(), temp.data.begin(), temp.data.end());
    temp.data.clear();
  }
  BigNumber::discard_leading_zero(remainder.data);
  remainder.sgn = lhs.sgn;
  // make -0 -> +0
  if (remainder.data.size()==1 && remainder.data.back()==0) {
    remainder.sgn = true;
  }
  return remainder;
}


// output format
std::ostream& operator<<(std::ostream& os, const BigNumber& rhs) {
  if (!rhs.sgn) {
    os << "-";
  }

  for (auto i = rhs.data.rbegin(); i!= rhs.data.rend(); ++i ) {
    // i is a pointer, point to a certain position in the vector (rhs.data)
    // *i is the value store in THAT position
    if (*i >= 10) {
      os << static_cast<char>(*i + 'A' - 10); // 10 -> 'a'
    } else {
      os << static_cast<char>(*i + '0'); // 1 -> '1'
    }
  }

  return os;
}
