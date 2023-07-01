#include "string.hpp"

String::String() {
  const size_t kSize = 3;
  capacity_ = kSize;
  str_ = new char[capacity_];
}
String::String(size_t size, char character = '\0') {
  size_ = size;
  capacity_ = size_;
  str_ = new char[capacity_];
  for (size_t i = 0; i < size_; ++i) {
    str_[i] = character;
  }
}
String::String(const char* k_str) {
  size_ = strlen(k_str);
  capacity_ = size_ + 1;
  str_ = new char[size_ + 1];
  for (size_t i = 0; i < size_; ++i) {
    str_[i] = k_str[i];
  }
  str_[size_] = '\0';
}
String::String(const String& k_str) {
  size_ = k_str.size_;
  capacity_ = k_str.capacity_;
  str_ = new char[capacity_];
  for (size_t i = 0; i < size_; ++i) {
    str_[i] = k_str[i];
  }
}
size_t String::Size() const { return size_; }
void String::Clear() {
  for (size_t i = 0; i < size_; ++i) {
    str_[i] = '\0';
  }
  size_ = 0;
}
void String::PushBack(char character) {
  if (capacity_ <= size_) {
    Reserve(2 * size_ + 1);
  }
  str_[size_] = character;
  ++size_;
}
void String::PopBack() {
  if (size_ == 0) {
    return;
  }
  --size_;
  str_[size_] = '\0';
}
void String::Resize(size_t size_new) { Resize(size_new, ' '); }
void String::Resize(size_t size_new, char character) {
  if (size_ > size_new) {
    std::memset(str_ + size_new, '\0', size_ - size_new);
    size_ = size_new;
  } else if (size_ < size_new) {
    if (capacity_ <= size_new) {
      String tmp(size_new * 2, character);
      std::memcpy(tmp.str_, str_, size_);
      Swap(tmp);
      capacity_ = 2 * size_new;
      size_ = size_new;
    } else {
      memset(str_ + size_, '\0', size_new - size_);
      size_ = size_new;
    }
  }
}
void String::Reserve(size_t new_cap) {
  if (new_cap > capacity_) {
    String res;
    res.size_ = size_;
    res.capacity_ = new_cap;
    auto* str = new char[new_cap + 1];
    for (size_t i = 0; i < size_; i++) {
      str[i] = str_[i];
    }
    delete[] res.str_;
    res.str_ = str;
    Swap(res);
  }
}
size_t String::Capacity() const { return capacity_; }
void String::ShrinkToFit() {
  if (capacity_ > size_) {
    capacity_ = size_;
  }
}
void String::Swap(String& other) {
  size_t size_other = other.size_;
  size_t capacity_other = other.capacity_;
  char* str_other = other.str_;
  other.size_ = size_;
  other.capacity_ = capacity_;
  other.str_ = str_;
  size_ = size_other;
  capacity_ = capacity_other;
  str_ = str_other;
}
char& String::Front() { return *str_; }
char String::Front() const { return *str_; }
char String::Back() const {
  if (size_ == 0) {
    Front();
  }
  return *(str_ + size_ - 1);
}
char& String::Back() {
  if (size_ == 0) {
    Front();
  }
  return *(str_ + size_ - 1);
}
bool String::Empty() const { return size_ == 0; }
const char* String::Data() const { return str_; }
String& String::operator=(const String& k_copy) {
  if (this == &k_copy) {
    return *this;
  }
  delete[] str_;
  str_ = new char[k_copy.size_];
  for (size_t i = 0; i < k_copy.size_; ++i) {
    str_[i] = k_copy.str_[i];
  }
  size_ = k_copy.size_;
  capacity_ = k_copy.size_;
  return *this;
}
char& String::operator[](size_t id) { return str_[id]; }
char String::operator[](size_t k_id) const { return str_[k_id]; }
bool String::operator<(const String& str) const {
  size_t count = 0;
  while (count != size_ && count != str.size_ &&
         str_[count] == str.str_[count]) {
    ++count;
  }
  if (count == size_ && count == str.size_) {
    return false;
  }
  if (count == str.size_) {
    return false;
  }
  if (count == size_) {
    return true;
  }
  return (*(str_ + count) < *(str.str_ + count));
}
bool String::operator<=(const String& str) const { return !(*this > str); }
bool String::operator>(const String& str) const { return (str < *this); }
bool String::operator>=(const String& str) const {
  return (*this == str) || (*this > str);
}
bool String::operator==(const String& str) const {
  return !(*this < str || *this > str);
}
bool String::operator!=(const String& str) const {
  return (*this < str || *this > str);
}
String String::operator+(const String& str) const {
  String tmp = *this;
  tmp += str;
  return tmp;
}
String& String::operator+=(const String& str) {
  size_t size = size_;
  Resize(size_ + str.size_);
  std::memcpy((str_ + size), str.str_, sizeof(char) * str.size_);
  return *this;
}
String String::operator*(size_t val) const {
  String tmp = *this;
  tmp *= val;
  return tmp;
}
String& String::operator*=(size_t val) {
  String tmp;
  for (size_t i = 0; i < val; ++i) {
    tmp += *this;
  }
  Swap(tmp);
  return *this;
}
std::vector<String> String::Split(const String& delim) {
  std::vector<String> result;
  size_t i = 0;
  String newstr;
  size_t delim_size = delim.size_;

  while (i <= size_ - delim_size && size_ != 0) {
    String tmp(delim.Size());
    std::memcpy(tmp.str_, str_ + i, delim_size);
    if (tmp != delim) {
      String new_char(sizeof(char));
      std::memcpy(new_char.str_, str_ + i, sizeof(char));
      ++i;
      newstr += new_char;
    } else {
      i += delim_size;
      result.push_back(newstr);
      newstr.Clear();
    }
  }
  if (i != Size()) {
    String tmp(Size() - i);
    std::memcpy(tmp.str_, str_ + i, (Size() - i));
    if (tmp != delim) {
      newstr += tmp;
    }
  }
  result.push_back(newstr);
  return result;
}
String String::Join(const std::vector<String>& str) const {
  String result;
  if (str.empty()) {
    return result;
  }
  size_t i = 0;
  while (i < str.size() - 1) {
    result += str[i];
    result += *this;
    ++i;
  }
  result += str.back();
  return result;
}
std::istream& operator>>(std::istream& is, String& str) {
  char value = '0';
  str.Clear();
  while (is.get(value) && value != '\n') {
    str.PushBack(value);
  }
  return is;
}
std::ostream& operator<<(std::ostream& os, const String& str) {
  for (size_t i = 0; i < str.Size(); ++i) {
    os << str.str_[i];
  }
  return os;
}
String::~String() { delete[] str_; }