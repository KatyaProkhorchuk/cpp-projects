#pragma once
#include <cstring>
#include <iostream>
#include <vector>
class String {
 public:
  String();
  String(size_t size, char character);
  String(const char* k_str);
  String(const String& k_string_cpy);
  size_t Size() const;
  void Clear();
  void PushBack(char character);
  void PopBack();
  void Resize(size_t size_new);
  void Resize(size_t size_new, char character);
  void Reserve(size_t new_cap);
  size_t Capacity() const;
  void ShrinkToFit();
  void Swap(String& other);
  char Front() const;
  char& Front();
  char& Back();
  char Back() const;
  bool Empty() const;
  const char* Data() const;
  String& operator=(const String& k_copy);
  char& operator[](size_t id);
  char operator[](size_t id) const;
  bool operator<(const String& k_str) const;
  bool operator<=(const String& k_str) const;
  bool operator>(const String& k_str) const;
  bool operator>=(const String& k_str) const;
  bool operator==(const String& k_str) const;
  bool operator!=(const String& k_str) const;
  String& operator+=(const String& k_str);
  String operator+(const String& k_str) const;
  String operator*(size_t val) const;
  String& operator*=(size_t val);
  std::vector<String> Split(const String& k_delim = " ");
  String Join(const std::vector<String>& str) const;
  friend std::istream& operator>>(std::istream& is, String& str);
  friend std::ostream& operator<<(std::ostream& os, const String& str);
  ~String();

 private:
  size_t size_{0};
  size_t capacity_{0};
  char* str_{nullptr};
};