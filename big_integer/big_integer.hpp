#pragma once
#include <iostream>
#include <limits>
#include <numeric>
#include <sstream>
#include <string>
#include <type_traits>
#include <vector>
class BigInt {
 public:
  BigInt() = default;
  BigInt(bool is_neg, const std::vector<size_t>& number);
  BigInt(const std::vector<size_t>& number);
  BigInt(int64_t number);
  BigInt(const std::string& number);
  BigInt(const BigInt& bi);
  BigInt(bool sign, int64_t number);
  void ShiftLeft(size_t n);
  void ShiftRight(size_t n);
  BigInt Slice(size_t l, size_t r) const;
  BigInt& operator=(const BigInt& num);
  BigInt Div(size_t num) const;
  bool operator==(const BigInt& num) const;
  BigInt operator%(const BigInt& num) const;
  bool operator!=(const BigInt& num) const;
  bool operator<(const BigInt& num) const;
  bool operator>(const BigInt& num) const;
  BigInt operator+(const BigInt& num) const;
  BigInt operator*(const BigInt& num) const;
  BigInt operator/(const BigInt& num) const;
  BigInt& operator-=(const BigInt& num);
  BigInt& operator+=(const BigInt& num);
  BigInt Minus(const BigInt& num);
  BigInt Plus(const BigInt& num);
  BigInt operator-(const BigInt& num) const;
  BigInt operator-() const;
  BigInt ResultMult(const BigInt& num, BigInt res, size_t xy_shift) const;
  BigInt Mult(const BigInt& num);
  BigInt Division(const BigInt& num);
  BigInt& operator*=(const BigInt& num);
  BigInt& operator/=(const BigInt& num);
  BigInt DivResult(int range, const BigInt& num) const;
  bool operator<=(const BigInt& num) const;
  bool operator>=(const BigInt& num) const;
  BigInt operator++();
  BigInt operator--();
  BigInt operator++(int);
  BigInt operator--(int);
  size_t ToInt() const;
  friend std::ostream& operator<<(std::ostream& os, const BigInt& out);
  friend std::istream& operator>>(std::istream& in, BigInt& num);

 private:
  std::vector<size_t> number_;
  bool sign_ = true;
  const int64_t kRank = 1e9;
  const size_t kCountRank = 9;
};
