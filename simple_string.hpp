#ifndef SIMPLE_STRING_HPP
#define SIMPLE_STRING_HPP

#include <cstddef>
#include <initializer_list>
#include <iostream>
#include <ostream>
#include <tuple>

class simple_string_iterator;

class simple_string {
  char *_str;
  std::size_t _size;
  friend class simple_string_iterator;

public:
  simple_string(const char *s);
  simple_string(std::initializer_list<char> il);
  simple_string(const simple_string &other);
  simple_string(simple_string &&other);
  ~simple_string();

  char *c_str() const;
  char *c_str();

  simple_string &operator=(const simple_string &other);
  simple_string &operator=(simple_string &&other);
  simple_string operator+(const simple_string &other);
  bool operator==(const simple_string &other) const;
  char &operator[](const int &idx);

  std::size_t size();
  friend std::ostream &operator<<(std::ostream &_stream,
                                  const simple_string &ss);

  simple_string_iterator begin();
  simple_string_iterator end();
};

class simple_string_iterator {
  using iterator_category = std::bidirectional_iterator_tag;
  using value_type = char;
  using difference_type = std::ptrdiff_t;
  using pointer = char *;
  using reference = char &;

  friend class simple_string;

  simple_string _s;
  std::size_t _pos;
  simple_string_iterator(simple_string &s, std::size_t pos);

public:
  simple_string_iterator &operator++();
  simple_string_iterator operator++(int);
  simple_string_iterator &operator--();
  simple_string_iterator operator--(int);
  bool operator==(const simple_string_iterator &other) const;
  bool operator!=(const simple_string_iterator &other) const;
  reference operator*();
  reference operator*() const;
};

#endif //
