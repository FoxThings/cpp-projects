/**
 * @file String.hpp
 * @author Nikita Zvezdin
 * @date 25.10.2021
 */
#pragma once

#include <stdlib.h>
#include <string.h>

#include <iostream>
#include <vector>

class String {
 public:
  String();
  String(size_t size, char character);
  String(const char* cstring);
  String(const String& value);
  ~String();

  void Clear();
  void PushBack(char character);
  void PopBack();
  void Resize(size_t new_size);
  void Resize(size_t new_size, char character);
  void Reserve(size_t new_cap);
  void ShrinkToFit();
  void Swap(String& other);
  char& Front();
  const char& Front() const;
  char& Back();
  const char& Back() const;
  bool Empty() const;
  size_t Size() const;
  size_t Capacity() const;
  const char* Data() const;

  std::vector<String> Split(const String& delim = " ");
  String Join(const std::vector<String>& strings) const;

  char& operator[](int index);
  const char& operator[](int index) const;
  String& operator=(const String& value);
  String& operator+=(const String& value);
  String& operator*=(size_t num);

  void Print();

 private:
  char* string_;
  size_t size_;
  size_t capacity_;
};

String operator*(const String& value, size_t num);
String operator+(const String& left, const String& right);

bool operator<(const String& left, const String& right);
bool operator>(const String& left, const String& right);
bool operator<=(const String& left, const String& right);
bool operator>=(const String& left, const String& right);
bool operator==(const String& left, const String& right);
bool operator!=(const String& left, const String& right);

std::ostream& operator<<(std::ostream& out, const String& string);
std::istream& operator>>(std::istream& in, String& string);
