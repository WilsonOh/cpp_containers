#include "Range.hpp"
#include "simple_string.hpp"
#include <algorithm>
#include <cctype>
#include <functional>
#include <iostream>
#include <numeric>
#include <vector>

class Foo {
  simple_string s;

public:
  Foo(const simple_string &_s) : s(_s) {}
  Foo(simple_string &&_s) : s(std::move(_s)) {}
};

simple_string uppercase(simple_string s) {
  std::transform(s.begin(), s.end(), s.begin(),
                 [](char &c) { return toupper(c); });
  return s;
}

int main(void) {
  simple_string s("hell");
  std::cout << uppercase(s) << '\n';
}
