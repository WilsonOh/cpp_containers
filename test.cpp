#include "Range.hpp"
#include "simple_string.hpp"
#include <algorithm>
#include <cctype>
#include <functional>
#include <iostream>
#include <iterator>
#include <numeric>
#include <vector>

class Foo {
  simple_string s;

public:
  Foo(const simple_string &_s) : s(_s) {}
  Foo(simple_string &&_s) : s(std::move(_s)) {}
};

simple_string uppercase(simple_string s) {
  std::transform(s.cbegin(), s.cend(), s.begin(),
                 [](const char &c) { return toupper(c); });
  return s;
}

template <typename T>
std::ostream &operator<<(std::ostream &out, const std::vector<T> &v) {
  out << "[ ";
  for (const simple_string &word : v) {
    out << word << " ";
  }
  out << "]";
  return out;
}

int main(void) {
  simple_string s("hello, this, is, a, test");
  auto words = s.split(", ");
  std::cout << simple_string::join("::", words);
}
