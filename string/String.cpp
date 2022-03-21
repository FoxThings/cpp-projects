#include "String.hpp"

#include <stdio.h>

template <typename T>
T static Max(T a, T b) {
  return (a > b) ? a : b;
}

template <typename T>
T static Min(T a, T b) {
  return (a < b) ? a : b;
}

String::String() {
  size_ = 0;
  capacity_ = 1;
  string_ = (char*)calloc(capacity_, sizeof(char));
  string_[capacity_ - 1] = '\0';
}

String::String(size_t size, char character) {
  size_ = size;
  capacity_ = size_ + 1;

  string_ = (char*)calloc(capacity_, sizeof(char));
  memset(string_, character, size_);
  string_[capacity_ - 1] = '\0';
}

String::String(const char* cstring) {
  size_ = strlen(cstring);
  capacity_ = size_ + 1;

  string_ = (char*)calloc(capacity_, sizeof(char));
  memcpy(string_, cstring, size_);
  string_[capacity_ - 1] = '\0';
}

String::~String() { free(string_); }

void String::Reserve(size_t new_cap) {
  if (new_cap <= capacity_ - 1) {
    return;
  }

  capacity_ = new_cap + 1;
  string_ = (char*)realloc(string_, capacity_ * sizeof(char));
  string_[capacity_ - 1] = '\0';
}

void String::Clear() {
  size_ = 0;
  string_[size_] = '\0';
}

void String::PushBack(char character) {
  if (size_ == capacity_ - 1) {
    Reserve(Max<size_t>((capacity_ - 1) * 2, 1));
  }

  string_[size_++] = character;
  string_[size_] = '\0';
}

void String::PopBack() {
  if (size_ > 0) {
    --size_;
    string_[size_] = '\0';
  }
}

void String::Resize(size_t new_size) {
  if (new_size > capacity_ - 1) {
    Reserve(new_size);
  }

  size_ = new_size;
  string_[size_] = '\0';
}

void String::Resize(size_t new_size, char character) {
  if (new_size > capacity_ - 1) {
    Reserve(new_size);
  }

  if (new_size > size_) {
    memset(string_ + size_, character, new_size - size_);
  }

  size_ = new_size;
  string_[size_] = '\0';
}

void String::ShrinkToFit() {
  if (capacity_ - 1 > size_) {
    capacity_ = size_ + 1;
    string_[capacity_ - 1] = '\0';
  }
}

void String::Swap(String& other) {
  if (this != &other) {
    char* temp_str = string_;
    string_ = other.string_;
    other.string_ = temp_str;

    size_t temp_capacity = capacity_;
    capacity_ = other.capacity_;
    other.capacity_ = temp_capacity;

    size_t temp_size = size_;
    size_ = other.size_;
    other.size_ = temp_size;
  }
}

char& String::Front() { return string_[0]; }

const char& String::Front() const { return string_[0]; }

char& String::Back() { return string_[size_ - 1]; }

const char& String::Back() const { return string_[size_ - 1]; }

bool String::Empty() const { return size_ == 0; }

size_t String::Size() const { return size_; }

size_t String::Capacity() const { return capacity_ - 1; }

const char* String::Data() const { return string_; }

String::String(const String& value) {
  size_ = value.size_;
  capacity_ = value.capacity_;
  string_ = (char*)calloc(capacity_, sizeof(char));
  memcpy(string_, value.string_, value.capacity_);
}

String& String::operator=(const String& value) {
  if (this != &value) {
    free(string_);
    size_ = value.size_;
    capacity_ = value.capacity_;
    string_ = (char*)calloc(value.capacity_, sizeof(char));
    memcpy(string_, value.string_, value.capacity_);
  }

  return *this;
}

String& String::operator+=(const String& value) {
  size_t temp_size = value.size_;
  for (size_t i = 0; i < temp_size; ++i) {
    this->PushBack(value.string_[i]);
  }

  return *this;
}

String operator+(const String& left, const String& right) {
  String answer(left);
  answer += right;
  return answer;
}

String& String::operator*=(size_t num) {
  if (num == 0) {
    this->Clear();
  } else {
    String temp(*this);
    for (size_t i = 0; i < num - 1; ++i) {
      *this += temp;
    }
  }

  return *this;
}

String operator*(const String& value, size_t num) {
  String answer(value);
  answer *= num;
  return answer;
}

bool operator<(const String& left, const String& right) {
  return strcmp(left.Data(), right.Data()) < 0;
}

bool operator>(const String& left, const String& right) {
  return strcmp(left.Data(), right.Data()) > 0;
}

bool operator<=(const String& left, const String& right) {
  return strcmp(left.Data(), right.Data()) <= 0;
}

bool operator>=(const String& left, const String& right) {
  return strcmp(left.Data(), right.Data()) >= 0;
}

bool operator==(const String& left, const String& right) {
  return strcmp(left.Data(), right.Data()) == 0;
}

bool operator!=(const String& left, const String& right) {
  return strcmp(left.Data(), right.Data()) != 0;
}

char& String::operator[](int index) { return string_[index]; }

const char& String::operator[](int index) const { return string_[index]; }

std::ostream& operator<<(std::ostream& out, const String& string) {
  out << string.Data();

  return out;
}

std::istream& operator>>(std::istream& in, String& string) {
  char character;
  while (in.get(character) && character != '\n') {
    string.PushBack(character);
  }

  return in;
}

String String::Join(const std::vector<String>& strings) const {
  String str;
  for (size_t i = 0; i < strings.size(); ++i) {
    str += strings[i];
    if (i < strings.size() - 1) {
      str += string_;
    }
  }

  return str;
}

std::vector<String> String::Split(const String& delim) {
  std::vector<String> result;

  String temp;
  for (size_t i = 0; i < size_; ++i) {
    temp.PushBack(string_[i]);
    if (strncmp(string_ + i, delim.Data(), delim.Size()) == 0) {
      temp.PopBack();
      result.push_back(temp);
      i += delim.Size() - 1;
      temp.Clear();
    }
  }

  result.push_back(temp);

  return result;
}

void String::Print() {
  printf("\nstring: %s\n\tcapacity: %ld\n\tsize: %ld\n\n", string_, capacity_,
         size_);
}
