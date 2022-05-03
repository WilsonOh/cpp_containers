#include "simple_string.hpp"
#include <algorithm>
#include <cstring>
#include <initializer_list>
#include <iostream>

simple_string::simple_string(const char *s) {
  // std::cout << "Constructed using const char * copy\n";
  _size = strlen(s);
  _str = new char[_size + 1];
  std::copy(s, s + _size, _str);
  _str[_size] = 0;
}

simple_string::simple_string(std::initializer_list<char> il) {
  // std::cout << "Constructed using initializer list!\n";
  _size = il.size();
  _str = new char[_size + 1];
  std::copy(il.begin(), il.end(), _str);
  _str[_size] = 0;
}

simple_string::simple_string(const simple_string &other) {
  // std::cout << "Copied!\n";
  _size = other._size;
  _str = new char[_size + 1];
  std::copy(other._str, other._str + _size, _str);
  _str[_size] = 0;
}

simple_string::simple_string(simple_string &&other) {
  // std::cout << "Moved!\n";
  _size = other._size;
  _str = other._str;
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

simple_string &simple_string::operator=(const simple_string &other) {
  std::cout << "copy assigned!\n";
  std::copy(other._str, other._str + other._size, _str);
  _size = other._size;
  _str[_size] = 0;
  return *this;
}

simple_string &simple_string::operator=(simple_string &&other) {
  std::cout << "Move Assigned!\n";
  _str = other._str;
  _size = other._size;
  other._str = nullptr;
  other._size = 0;
  return *this;
}

bool simple_string::operator==(const simple_string &other) const {
  return strcmp(_str, other._str) == 0;
}

simple_string simple_string::operator+(const simple_string &other) {
  std::size_t new_size = _size + other._size;
  char *new_str = new char[new_size + 1];
  std::copy(_str, _str + _size, new_str);
  std::copy(other._str, other._str + other._size, new_str + _size);
  new_str[new_size] = 0;
  return {new_str};
}

char &simple_string::operator[](const int &idx) { return _str[idx]; }

simple_string_iterator simple_string::begin() { return {*this, 0}; }

simple_string_iterator simple_string::end() { return {*this, _size}; }

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
  return _s == other._s && _pos == other._pos;
}

bool simple_string_iterator::operator!=(
    const simple_string_iterator &other) const {
  return !(*this == other);
}

char &simple_string_iterator::operator*() { return _s[_pos]; }

char &simple_string_iterator::operator*() const { return _s._str[_pos]; }
