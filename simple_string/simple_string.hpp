#ifndef SIMPLE_STRING_HPP
#define SIMPLE_STRING_HPP

#include <cstddef>
#include <initializer_list>
#include <ostream>
#include <vector>

/* Forward Declations so that we can reference the iterators
 * in the simple string class
 */
class simple_string_iterator;
class simple_string_const_iterator;

/**
 * -----------------------------------------------------------
 *  simple_string start
 * -----------------------------------------------------------
 */
class simple_string {
  char *_str;
  std::size_t _size;
  char *strnew(const char *s);
  /* make the string iterator a friend class so that it can access the private
   * members of simple string */
  friend class simple_string_iterator;

public:
  /* default constructor -> initializes the char array to null and size to 0 */
  simple_string();
  /* normal constructor from a string literal */
  simple_string(const char *s);
  /* initializer list constructor for whatever reason */
  simple_string(std::initializer_list<char> il);
  /* copy constructor, takes in an lvalue reference to another simple string */
  simple_string(const simple_string &other) noexcept;
  /* move constructor, takes in an rvalue reference of a simple string */
  simple_string(simple_string &&other) noexcept;
  /* destructor, frees the underlying char array */
  ~simple_string();

  char *c_str() const;
  char *c_str();
  std::size_t length() const;
  bool empty() const;

  /* copy assignment operator: takes in an lvalue reference to another simple
   * string, makes a copy of it and swaps its contents with the current simple
   * string. Hence, the "copy and swap" idiom
   */
  simple_string &operator=(const simple_string &other) noexcept;
  /* move assignment operator: takes in a rvalue reference to a simple string,
  basically does the same thing as the move constructor just that we are using
  the "=" assignment operator to move the rvalue instead */
  simple_string &operator=(simple_string &&other);
  simple_string operator+(const simple_string &other) const;
  void operator+=(const simple_string &other);
  bool operator==(const simple_string &other) const;
  char &operator[](std::size_t idx);
  const char &operator[](std::size_t idx) const;
  bool operator<(const simple_string &other) const;

  /* internal swap function which takes in a lvalue reference instead of a const
   * reference, used for the copy assignment operator
   */
  void swap(simple_string &other) noexcept;
  std::size_t size();
  /* cout overriding */
  friend std::ostream &operator<<(std::ostream &_stream,
                                  const simple_string &ss);

  /* there are const and non-const versions of the iterator begin and end */
  simple_string_iterator begin();
  simple_string_const_iterator begin() const;
  simple_string_const_iterator cbegin() const;
  simple_string_iterator end();
  simple_string_const_iterator end() const;
  simple_string_const_iterator cend() const;

  /* Other nice string manipulation methods */
  simple_string upper() const;
  simple_string lower() const;
  simple_string substr(const std::size_t &start,
                       const std::size_t &offset) const;
  simple_string substr(const std::size_t &start) const;
  int find(const simple_string &needle, const std::size_t &start = 0) const;
  std::vector<simple_string> split(const simple_string &delimiter) const;
  static simple_string join(const simple_string &delim, const std::vector<simple_string> &words);
};

/**
 * -----------------------------------------------------------
 *  simple_string_iterator start
 * -----------------------------------------------------------
 */
class simple_string_iterator {
  /* tags for the compiler to know what kind of iterator this is. not inheriting
  from the base iterator class as it is getting deprecated */
  using iterator_category = std::bidirectional_iterator_tag;
  using value_type = char;
  using difference_type = std::ptrdiff_t;
  using pointer = char *;
  using reference = char &;

  friend class simple_string;

  /* IMPT: MAKE SURE to declare the simple string member as a reference!! */
  simple_string &_s;
  mutable std::size_t _pos;
  simple_string_iterator(simple_string &s, std::size_t pos);

public:
  simple_string_iterator &operator++();
  simple_string_iterator operator++(int);
  simple_string_iterator &operator--();
  simple_string_iterator operator--(int);
  bool operator==(const simple_string_iterator &other) const;
  bool operator!=(const simple_string_iterator &other) const;
  /* the mutable non-const iterator has both the const deference and non-const
   * deference operators */
  char &operator*();
  const char &operator*() const;
};

/**
 * -----------------------------------------------------------
 *  simple_string_const_iterator start
 * -----------------------------------------------------------
 */
class simple_string_const_iterator {
  /* mostly the same as the non-const iterator */
  using iterator_category = std::bidirectional_iterator_tag;
  using value_type = char;
  using difference_type = std::ptrdiff_t;
  using pointer = char *;
  using reference = char &;

  friend class simple_string;

  /* MAIN difference: const iterator has a CONST reference to a simple string
   * instead */
  const simple_string &_s;
  mutable std::size_t _pos;
  /* IMPT: the const iterator constructor takes in a const reference to a simple
   * string! */
  simple_string_const_iterator(const simple_string &s, std::size_t pos);

public:
  simple_string_const_iterator &operator++();
  simple_string_const_iterator operator++(int);
  simple_string_const_iterator &operator--();
  simple_string_const_iterator operator--(int);
  bool operator==(const simple_string_const_iterator &other) const;
  bool operator!=(const simple_string_const_iterator &other) const;
  /* Another difference: the const iterator only has the const deference
   * operator! */
  const char &operator*() const;
};
#endif //
