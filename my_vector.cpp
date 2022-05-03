#include <cstddef>
#include <initializer_list>
#include <iterator>
#define FMT_HEADER_ONLY

#include <algorithm>
#include <cstring>
#include <fmt/core.h>
#include <iostream>
#include <ostream>
#include <string>
#include <type_traits>
#include <utility>
#include <functional>

template <class T> class vector {
  T *vals;
  std::size_t _size;

public:
  vector<T>(void) {
    _size = 0;
    vals = new T[0];
  }
  vector<T>(std::initializer_list<T> il) {
    vals = new T[il.size()];
    std::copy(il.begin(), il.end(), vals);
    _size = il.size();
  }
  vector<T>(std::size_t init_size) : _size(init_size) {
    vals = new T[init_size];
    _size = init_size;
  }
  vector<T>(const vector<T> &v) {
    std::cout << "copied!" << '\n';
    vals = new T[v._size];
    std::memcpy(vals, v.vals, v._size);
    _size = v._size;
  }
  vector<T>(vector<T> &&v) noexcept {
    std::cout << "moved!" << '\n';
    vals = v.vals;
    _size = v._size;
    v.vals = nullptr;
    v._size = 0;
  }
  ~vector<T>(void) { delete[] vals; }

  std::size_t size(void) { return _size; }

  vector<T> operator=(vector<T> other) {
    std::cout << "assigned!" << '\n';
    std::swap(vals, other.vals);
    std::swap(_size, other._size);
    return *this;
  }

  void push_back(T val) {
    T *new_vals = new T[_size + 1];
    std::copy(vals, vals + _size, new_vals);
    new_vals[_size] = val;
    delete[] vals;
    vals = new_vals;
    _size++;
  }

  T operator[](std::size_t idx) {
    if (idx >= _size) {
      auto err_str = fmt::format(
          "index {} out of bounds for vector of size {}\n", idx, _size);
      throw std::out_of_range(err_str);
    }
    return vals[idx];
  }

  vector<T> filter(std::function<bool(int)> predicate) {
    vector<T> ret;
    for (std::size_t i = 0; i < _size; i++) {
      if (predicate(vals[i])) {
        ret.push_back(vals[i]);
      }
    }
    return ret;
  }

  vector<T> map(std::function<int(int)> transformer) {
    vector<T> ret;
    for (std::size_t i = 0; i < _size; i++) {
      ret.push_back(transformer(vals[i]));
    }
    return ret;
  }

  vector<T> operator+(const vector<T> &other) {
    vector<T> ret(_size + other._size);
    memcpy(ret.vals, vals, _size * sizeof(T));
    memcpy(ret.vals + _size, other.vals, other._size * sizeof(T));
    ret._size = _size + other._size;
    return ret;
  }

  friend std::ostream &operator<<(std::ostream &_stream, const vector<T> &v) {
    _stream << "[";
    for (std::size_t i = 0; i < v._size; i++) {
      _stream << v.vals[i];
      if (i < v._size - 1) {
        _stream << ' ';
      }
    }
    _stream << "]";
    return _stream;
  }
};

template <class T> class Foo {
  vector<T> x;

public:
  Foo(const vector<T> &v) : x(v) {}
  Foo(vector<T> &&v) : x(std::move(v)) {}
};

bool isEven(int i) {
  return !(i % 2);
}

int addOne(int i) {
  return i + 1;
}

auto addX(const int &x) {
  return [x](const int &y) { return x + y; };
}

int main(void) {
  vector<int> v{1, 2, 3, 4, 5, 6};
  std::cout << v << '\n';
  v = v.filter(isEven).map(addOne).filter([](const int &x){ return x > 3; });
  std::cout << v << '\n';
}
