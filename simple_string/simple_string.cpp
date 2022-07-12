#include <cctype>

#include "simple_string.hpp"
#include <algorithm>
#include <cstring>
#include <fmt/core.h>
#include <initializer_list>
#include <iostream>
#include <stdexcept>

/**
 * ------------------------------------------------------------------------------------------------
 *  simple_string start
 * ------------------------------------------------------------------------------------------------
 */

char *simple_string::strnew(const char *s) {
  if (s != nullptr) {
    _size = strlen(s);
    return strcpy(new char[_size + 1], s);
  }
  return nullptr;
}

simple_string::simple_string() : _str(strnew("")) {}

simple_string::simple_string(const char *s) : _str(strnew(s)) {
  // std::cout << "Constructed using const char * copy\n";
}

simple_string::simple_string(std::initializer_list<char> il) {
  // std::cout << "Constructed using initializer list!\n";
  _size = il.size();
  _str = new char[_size + 1];
  std::copy(il.begin(), il.end(), _str);
  _str[_size] = 0;
}

simple_string::simple_string(const simple_string &other) noexcept
    : _str(strnew(other._str)) {
  // std::cout << "Copied!\n";
}

simple_string::simple_string(simple_string &&other) noexcept {
  // std::cout << "Moved!\n";
  _str = other._str;
  _size = other._size;
  other._str = nullptr;
  other._size = 0;
}

simple_string::~simple_string() {
  // std::cout << "deleted " << *this << '\n';
  delete[] _str;
}

std::size_t simple_string::size() { return _size; }

char *simple_string::c_str() { return _str; }

char *simple_string::c_str() const { return _str; }

std::ostream &operator<<(std::ostream &_stream, const simple_string &ss) {
  _stream << ss.c_str();
  return _stream;
}

void simple_string::swap(simple_string &other) noexcept {
  std::swap(_str, other._str);
  std::swap(_size, other._size);
}

simple_string &simple_string::operator=(const simple_string &other) noexcept {
  // std::cout << "copy assigned!\n";
  if (&other != this) {
    simple_string(other).swap(*this);
  }
  return *this;
}

simple_string &simple_string::operator=(simple_string &&other) {
  // std::cout << "Move Assigned!\n";
  if (&other != this) {
    _size = other._size;
    delete[] _str;
    _str = other._str;
    other._str = nullptr;
    other._size = 0;
  }
  return *this;
}

bool simple_string::operator==(const simple_string &other) const {
  return strcmp(_str, other._str) == 0;
}

bool simple_string::operator<(const simple_string &other) const {
  return strcmp(_str, other._str) < 0;
}

simple_string simple_string::operator+(const simple_string &other) const {
  std::size_t new_size = _size + other._size;
  char *new_str = new char[new_size + 1];
  std::copy(_str, _str + _size, new_str);
  std::copy(other._str, other._str + other._size, new_str + _size);
  new_str[new_size] = 0;
  simple_string ret(new_str);
  delete [] new_str;
  return ret;
}

void simple_string::operator+=(const simple_string &other) {
  std::size_t new_size = _size + other._size;
  char *new_str = new char[new_size + 1];
  std::copy(_str, _str + _size, new_str);
  std::copy(other._str, other._str + other._size, new_str + _size);
  new_str[new_size] = 0;
  *this = {new_str};
  delete[] new_str;
}

char &simple_string::operator[](std::size_t idx) {
  if (idx >= _size) {
    throw std::invalid_argument(
        fmt::format("idx {} out of bounds for string of size {}", idx, _size));
  }
  return _str[idx];
}

const char &simple_string::operator[](std::size_t idx) const {
  if (idx >= _size) {
    throw std::invalid_argument(
        fmt::format("idx {} out of bounds for string of size {}", idx, _size));
  }
  return _str[idx];
}

simple_string simple_string::upper() const {
  simple_string ret = *this;
  std::transform(this->cbegin(), this->cend(), ret.begin(),
                 [](const char &c) { return toupper(c); });
  return ret;
}

simple_string simple_string::lower() const {
  simple_string ret = *this;
  std::transform(this->cbegin(), this->cend(), ret.begin(),
                 [](const char &c) { return tolower(c); });
  return ret;
}

simple_string simple_string::substr(const std::size_t &start,
                                    const std::size_t &offset) const {
  if (start + offset > _size) {
    throw std::invalid_argument("out of bounds substring");
  }
  char *tmp = new char[offset + 1];
  std::memcpy(tmp, _str + start, offset);
  tmp[offset] = 0;
  simple_string ret(tmp);
  delete[] tmp;
  return ret;
}

simple_string simple_string::substr(const std::size_t &start) const {
  if (start > _size) {
    throw std::invalid_argument("out of bounds substring");
  }
  std::size_t len = _size - start;
  char *tmp = new char[len + 1];
  std::memcpy(tmp, _str + start, len);
  tmp[len] = 0;
  simple_string ret(tmp);
  delete[] tmp;
  return ret;
}

std::size_t simple_string::length() const { return _size; }

bool simple_string::empty() const { return _size == 0; }

int simple_string::find(const simple_string &needle,
                        const std::size_t &start) const {
  if (needle.empty()) {
    throw std::invalid_argument("cannot find empty string");
  }
  for (std::size_t i = start; i < _size - needle.length() + 1; ++i) {
    if (_str[i] == needle[0]) {
      if (needle == this->substr(i, needle.length())) {
        return i;
      }
    }
  }
  return -1;
}

std::vector<simple_string>
simple_string::split(const simple_string &delimiter) const {
  size_t pos_start = 0;
  size_t pos_end;
  size_t delim_len = delimiter.length();
  simple_string token;
  std::vector<simple_string> res;

  while ((pos_end = this->find(delimiter, pos_start)) != std::string::npos) {
    token = this->substr(pos_start, pos_end - pos_start);
    pos_start = pos_end + delim_len;
    res.emplace_back(token);
  }

  simple_string last = this->substr(pos_start);
  if (!last.empty())
    res.emplace_back(last);
  return res;
}

simple_string simple_string::join(const simple_string &delim,
                                  const std::vector<simple_string> &words) {
  simple_string ret;
  for (std::size_t i = 0; i < words.size(); ++i) {
    ret += words[i];
    if (i < words.size() - 1) {
      ret += delim;
    }
  }
  return ret;
}

simple_string_iterator simple_string::begin() { return {*this, 0}; }

simple_string_const_iterator simple_string::begin() const { return {*this, 0}; }

simple_string_const_iterator simple_string::cbegin() const {
  return {*this, 0};
}

simple_string_iterator simple_string::end() { return {*this, _size}; }

simple_string_const_iterator simple_string::end() const {
  return {*this, _size};
}

simple_string_const_iterator simple_string::cend() const {
  return {*this, _size};
}

/**
 * ------------------------------------------------------------------------------------------------
 *  simple_string_iterator start
 * ------------------------------------------------------------------------------------------------
 */

simple_string_iterator::simple_string_iterator(simple_string &s,
                                               std::size_t pos)
    : _s(s), _pos(pos) {}

simple_string_iterator &simple_string_iterator::operator++() {
  ++_pos;
  return *this;
}

simple_string_iterator simple_string_iterator::operator++(int) {
  return {_s, _pos++};
}

simple_string_iterator &simple_string_iterator::operator--() {
  --_pos;
  return *this;
}

simple_string_iterator simple_string_iterator::operator--(int) {
  return {_s, _pos--};
}

bool simple_string_iterator::operator==(
    const simple_string_iterator &other) const {
  return &_s == &other._s && _pos == other._pos;
}

bool simple_string_iterator::operator!=(
    const simple_string_iterator &other) const {
  return !(*this == other);
}

char &simple_string_iterator::operator*() { return _s[_pos]; }

const char &simple_string_iterator::operator*() const { return _s[_pos]; }

/**
 * ------------------------------------------------------------------------------------------------
 *  simple_string_const_iterator start
 * ------------------------------------------------------------------------------------------------
 */

simple_string_const_iterator::simple_string_const_iterator(
    const simple_string &s, std::size_t pos)
    : _s(s), _pos(pos) {}

simple_string_const_iterator &simple_string_const_iterator::operator++() {
  ++_pos;
  return *this;
}

simple_string_const_iterator simple_string_const_iterator::operator++(int) {
  return {_s, _pos++};
}

simple_string_const_iterator &simple_string_const_iterator::operator--() {
  --_pos;
  return *this;
}

simple_string_const_iterator simple_string_const_iterator::operator--(int) {
  return {_s, _pos--};
}

bool simple_string_const_iterator::operator==(
    const simple_string_const_iterator &other) const {
  return &_s == &other._s && _pos == other._pos;
}

bool simple_string_const_iterator::operator!=(
    const simple_string_const_iterator &other) const {
  return !(*this == other);
}

const char &simple_string_const_iterator::operator*() const { return _s[_pos]; }
