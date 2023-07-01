#include "big_integer.hpp"

#include "string.h"
BigInt::BigInt(bool is_neg, const std::vector<size_t>& number) {
  sign_ = is_neg;
  for (size_t i = 0; i < number.size(); ++i) {
    this->number_.push_back(number[i]);
  }
}
BigInt::BigInt(const std::vector<size_t>& number) {
  for (size_t i = 0; i < number.size(); ++i) {
    this->number_.push_back(number[i]);
  }
}
BigInt::BigInt(int64_t number) {
  if (number == 0) {
    this->number_.push_back(0);
  } else {
    if (number < 0) {
      sign_ = false;
      if (std::numeric_limits<int64_t>::min() == number) {
        number = std::numeric_limits<int64_t>::max();
        for (; number != 0; number /= kRank) {
          int val = int64_t(number % kRank);
          number_.push_back(val);
        }
        --*this;
      } else {
        number = -number;
      }
    }
    for (; number != 0; number /= kRank) {
      int val = int64_t(number % kRank);
      number_.push_back(val);
    }
  }
}
BigInt::BigInt(bool sign, int64_t number) {
  if (number == 0) {
    this->number_.push_back(0);
  } else {
    sign_ = sign;
    for (; number != 0; number /= kRank) {
      int val = int64_t(number % kRank);
      number_.push_back(val);
    }
  }
}
BigInt::BigInt(const std::string& number) {
  if (number.empty()) {
    return;
  }
  size_t index = 0;
  if (number[0] == '-') {
    sign_ = false;
    ++index;
  }
  size_t num_count = (number.length() - index) / kCountRank;
  size_t num;
  const int kTen = 10;
  for (size_t i = 0; i < num_count; ++i) {
    num = 0;
    size_t j = 0;
    size_t blocks = number.length() - (i + 1) * kCountRank + j;
    while (j != kCountRank && blocks < number.length()) {
      num = num * kTen + number[blocks] - '0';
      ++j;
      blocks = number.length() - (i + 1) * kCountRank + j;
    }
    number_.push_back(num);
  }
  num = 0;
  for (size_t i = index; i < number.length() - num_count * kCountRank; ++i) {
    num = num * kTen + number[i] - '0';
  }
  if (num != 0 || number_.empty()) {
    number_.push_back(num);
  }
}
BigInt::BigInt(const BigInt& bi) {
  sign_ = bi.sign_;
  number_ = bi.number_;
}
BigInt& BigInt::operator=(const BigInt& num) {
  if (this == &num) {
    return *this;
  }
  sign_ = num.sign_;
  number_ = num.number_;
  return *this;
}
BigInt BigInt::operator++() {
  BigInt tmp = *this;
  *this = *this + BigInt(1);
  return *this;
}
BigInt BigInt::operator--() {
  BigInt tmp = *this;
  *this = *this - BigInt(1);
  return *this;
}
BigInt BigInt::operator++(int) {
  BigInt tmp = *this;
  *this = *this + BigInt(1);
  return tmp;
}
BigInt BigInt::operator--(int) {
  BigInt tmp = *this;
  *this = *this - BigInt(1);
  return tmp;
}
bool BigInt::operator==(const BigInt& num) const {
  if (this->number_.size() != num.number_.size()) {
    return false;
  }
  if (this->sign_ != num.sign_) {
    if (this->number_.size() == 1 && num.number_.size() == 1) {
      if (this->number_[0] == 0 && num.number_[0] == 0) {
        return true;
      }
    }
    return false;
  }
  for (size_t i = 0; i < num.number_.size(); ++i) {
    if (this->number_[i] != num.number_[i]) {
      return false;
    }
  }
  return true;
}
bool BigInt::operator!=(const BigInt& num) const { return !(*this == num); }
bool BigInt::operator<(const BigInt& num) const {
  if (!sign_ && !num.sign_) {
    return -*this > -num;
  }
  if (!sign_ && num.sign_) {
    return true;
  }
  if (sign_ && !num.sign_) {
    return false;
  }

  if (number_.size() != num.number_.size()) {
    return number_.size() < num.number_.size();
  }
  for (int i = number_.size() - 1; i >= 0; --i) {
    if (number_[i] > num.number_[i]) {
      return !this->sign_;
    }
    if (number_[i] < num.number_[i]) {
      return this->sign_;
    }
  }
  return false;
}
bool BigInt::operator>(const BigInt& num) const { return num < *this; }
BigInt BigInt::operator-() const { return BigInt(!sign_, number_); }
std::ostream& operator<<(std::ostream& os, const BigInt& out) {
  if (!out.sign_) {
    os << "-";
  }
  const int kTen = 10;
  os << out.number_.back();
  for (int i = out.number_.size() - 2; i >= 0; --i) {
    int zeros = 0;
    int number = out.number_[i];
    for (size_t j = 0; j < out.kCountRank; ++j) {
      number /= kTen;
      if (number == 0) {
        zeros = out.kCountRank - j - 1;
        break;
      }
    }
    for (; zeros > 0; --zeros) {
      os << "0";
    }
    os << out.number_[i];
  }
  return os;
}
BigInt& BigInt::operator+=(const BigInt& num) {
  BigInt tmp = Plus(num);
  *this = tmp;
  return *this;
}
static BigInt MinusResult(bool sign, std::vector<size_t> result) {
  if (result.empty()) {
    return BigInt(0);
  }
  while (result.size() > 1 && result[result.size() - 1] == 0) {
    result.resize(result.size() - 1);
  }
  return BigInt(sign, result);
}

BigInt BigInt::Minus(const BigInt& num) {
  if (!sign_ && !num.sign_) {
    return -((-*this) - (-num));
  }
  if (!num.sign_) {
    return *this + (-num);
  }
  if (!sign_) {
    return -(-*this + num);
  }
  bool result_sign = (*this >= num);
  BigInt a = *this < num ? num : *this;
  BigInt b = *this < num ? *this : num;
  std::vector<size_t> result;
  int64_t tmp = 0;
  for (size_t i = 0; i < a.number_.size(); ++i) {
    if (i < b.number_.size()) {
      tmp += a.number_[i] - b.number_[i];
    } else {
      tmp += a.number_[i];
    }
    if (tmp < 0) {
      result.push_back(num.kRank + tmp);
      tmp = -1;
    } else {
      result.push_back(tmp % num.kRank);
      tmp /= kRank;
    }
  }
  return MinusResult(result_sign, result);
}

BigInt& BigInt::operator-=(const BigInt& num) {
  BigInt tmp = Minus(num);
  *this = tmp;
  return *this;
}
BigInt BigInt::operator-(const BigInt& num) const {
  BigInt tmp = *this;
  return tmp -= num;
}
void BigInt::ShiftLeft(size_t n) {
  if (*this == BigInt("0") || n == 0) {
    return;
  }
  std::vector<size_t> tmp = number_;
  size_t i = 0;
  while (i < n) {
    number_.push_back(0);
    ++i;
  }
  for (i = 0; i < number_.size() - n; ++i) {
    number_[number_.size() - 1 - i] = number_[number_.size() - n - 1 - i];
  }
  for (i = 0; i < n; ++i) {
    number_[i] = 0;
  }
}
void BigInt::ShiftRight(size_t n) {
  if (n == 0) {
    return;
  }
  for (size_t i = 0; i < number_.size() - n; ++i) {
    number_[i] = number_[i + n];
  }
  number_.resize(number_.size() - n);
}
static BigInt Karatsuba(BigInt x, BigInt y, size_t x_shift, size_t y_shift,
                        size_t fixed) {
  x.ShiftLeft(x_shift);
  y.ShiftLeft(y_shift);
  BigInt a = x.Slice(0, fixed / 2);
  BigInt b = x.Slice(fixed / 2, fixed);
  BigInt c = y.Slice(0, fixed / 2);
  BigInt d = y.Slice(fixed / 2, fixed);
  BigInt first = a * c;
  BigInt second = b * d;
  BigInt third = (a + b) * (c + d);
  BigInt tmp = third - second - first;
  first.ShiftLeft(fixed);
  tmp.ShiftLeft(fixed / 2);
  return first + tmp + second;
}
BigInt BigInt::ResultMult(const BigInt& num, BigInt res,
                          size_t xy_shift) const {
  if (sign_ != num.sign_) {
    res = -res;
  }
  if (res != BigInt("0")) {
    res.ShiftRight(xy_shift);
  }
  return res;
}
BigInt BigInt::Mult(const BigInt& num) {
  BigInt x = *this >= 0 ? *this : -*this;
  BigInt y = num >= 0 ? num : -num;
  if (x == BigInt("0") || y == BigInt("0")) {
    return BigInt("0");
  }
  size_t n =
      x.number_.size() > y.number_.size() ? x.number_.size() : y.number_.size();
  if (n == 1) {
    return BigInt(sign_ == num.sign_, x.ToInt() * y.ToInt());
  }
  size_t x_shift = 0;
  size_t y_shift = 0;
  size_t xy_shift;
  if (x <= y) {
    x_shift = y.number_.size() - x.number_.size();
    xy_shift = x_shift;
  } else {
    y_shift = x.number_.size() - y.number_.size();
    xy_shift = y_shift;
  }
  size_t fixed = n;
  if ((x.number_.size() + x_shift) % 2 != 0) {
    xy_shift += 2;
    ++fixed;
    ++x_shift;
    ++y_shift;
  }
  BigInt res = Karatsuba(x, y, x_shift, y_shift, fixed);
  return ResultMult(num, res, xy_shift);
}
BigInt& BigInt::operator*=(const BigInt& num) {
  BigInt tmp = Mult(num);
  *this = tmp;
  return *this;
}
BigInt BigInt::operator*(const BigInt& num) const {
  BigInt tmp = *this;
  return tmp *= num;
}
BigInt BigInt::Slice(size_t left, size_t right) const {
  std::vector<size_t> result;
  size_t i = left;
  while (i < right && i < number_.size()) {
    result.push_back(number_[number_.size() - 1 - i]);
    ++i;
  }
  for (i = 0; i < result.size() / 2; ++i) {
    std::swap(result[i], result[result.size() - 1 - i]);
  }
  while (result.size() > 1 && result[result.size() - 1] == 0) {
    result.pop_back();
  }
  if (result.empty()) {
    return BigInt("0");
  }
  return BigInt(true, result);
}
static std::string SplitDec(int n) {
  if (n <= 0) {
    return "0";
  }
  std::string result = "1";
  int count = 0;
  while (count < n) {
    result += "000000000";
    ++count;
  }
  return result;
}
BigInt BigInt::Div(size_t num) const {
  std::vector<size_t> result(number_.size(), 0);
  size_t tmp = 0;
  size_t index = 0;
  size_t current = 0;
  for (size_t i = 0; i < number_.size(); ++i) {
    index = number_.size() - 1 - i;
    current = tmp * kRank + number_[index];
    tmp = current % num;
    result[index] = current / num;
  }
  while (result.size() > 1 && result[result.size() - 1] == 0) {
    result.pop_back();
  }
  return BigInt(true, result);
}
BigInt BigInt::operator%(const BigInt& num) const {
  return *this - (*this / num) * num;
}
BigInt BigInt::DivResult(int range, const BigInt& num) const {
  BigInt left(SplitDec(range - 1));
  BigInt right(SplitDec(range + 1));
  BigInt tmp;
  BigInt result;
  BigInt diff = *this - num;
  while (right - left > 1) {
    tmp = left + right;
    tmp = tmp.Div(2);
    result = tmp * num;
    if (result <= *this && result > diff) {
      return tmp;
    }
    if (result > *this) {
      right = tmp;
    } else {
      left = tmp;
    }
  }
  if (right * num > diff && right * num <= *this) {
    return right;
  }
  if (left * num > diff && left * num <= *this) {
    return left;
  }
  return tmp;
}
BigInt BigInt::Division(const BigInt& num) {
  if (num == BigInt("0")) {
    return BigInt("0");
  }
  if (!sign_ && !num.sign_) {
    return (-*this) / (-num);
  }
  if (!sign_ && num.sign_) {
    return -(-*this / num);
  }
  if (sign_ && !num.sign_) {
    return -(*this / (-num));
  }
  if (*this < num) {
    return BigInt("0");
  }
  if (num == BigInt("1")) {
    return *this;
  }
  int range = number_.size() - num.number_.size();
  return DivResult(range, num);
}
BigInt& BigInt::operator/=(const BigInt& num) {
  BigInt tmp = Division(num);
  *this = tmp;
  return *this;
}
BigInt BigInt::operator/(const BigInt& num) const {
  BigInt tmp = *this;
  return tmp /= num;
}
bool BigInt::operator<=(const BigInt& num) const { return !(*this > num); }
bool BigInt::operator>=(const BigInt& num) const { return !(*this < num); }
size_t BigInt::ToInt() const {
  size_t result = 0;
  size_t i = 0;
  while (i < number_.size()) {
    result = result * kCountRank + number_[number_.size() - 1 - i];
    ++i;
  }
  if (sign_) {
    return result;
  }
  return -result;
}
BigInt BigInt::Plus(const BigInt& num) {
  if (!sign_ && num.sign_) {
    return num - (-*this);
  }
  if (sign_ && !num.sign_) {
    return *this - (-num);
  }
  size_t tmp = 0;
  std::vector<size_t> new_num;
  size_t n =
      number_.size() > num.number_.size() ? number_.size() : num.number_.size();
  for (size_t i = 0; i < n; ++i) {
    if (i < number_.size()) {
      tmp += number_[i];
    }
    if (i < num.number_.size()) {
      tmp += num.number_[i];
    }
    new_num.push_back(tmp % kRank);
    tmp -= tmp % kRank;
    tmp /= kRank;
  }
  if (tmp != 0) {
    new_num.push_back(tmp);
  }
  return BigInt(sign_, new_num);
}
BigInt BigInt::operator+(const BigInt& num) const {
  BigInt tmp = *this;
  return tmp += num;
}
std::istream& operator>>(std::istream& in, BigInt& num) {
  std::string str;
  in >> str;
  num = BigInt(str);
  return in;
}
