#ifndef ARRAY_LIST_HPP
#define ARRAY_LIST_HPP

#include <algorithm>
#include <initializer_list>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>

template <typename T> class ArrayListIterator;
template <typename T> class ArrayListConstIterator;

template <typename T> class ArrayList {
  std::unique_ptr<T[]> data;
  std::size_t _size = 0;

public:
  ArrayList() = default;
  ArrayList(std::initializer_list<T> l) {
    data = std::make_unique<T[]>(l.size());
    std::copy(l.begin(), l.end(), data.get());
    _size = l.size();
  }

  ArrayList(const std::string &s) {
    data = std::make_unique<T[]>(s.size());
    std::copy(s.cbegin(), s.cend(), data.get());
    _size = s.size();
  }

  ArrayList(ArrayList &&other) {
    data = std::move(other.data);
    _size = other._size;
    other._size = 0;
  }

  ArrayList(const ArrayList &other) {
    data = std::make_unique<T[]>(other._size);
    T *tmp = other.data.get();
    std::copy(tmp, tmp + other._size, data.get());
    _size = other._size;
  }

  ArrayList &operator=(const ArrayList &other) {
    ArrayList tmp(other);
    data = std::move(tmp.data);
    _size = tmp._size;
    return *this;
  }

  ArrayList &operator=(ArrayList &&other) {
    data = std::move(other.data);
    _size = other._size;
    other._size = 0;
    return *this;
  }

  friend std::ostream &operator<<(std::ostream &out, const ArrayList &al) {
    out << "[ ";
    for (std::size_t i = 0; i < al._size; ++i) {
      out << al.data[i] << ' ';
    }
    out << "]";
    return out;
  }

  bool operator==(const ArrayList &other) const {
    if (_size != other._size)
      return false;
    for (std::size_t i = 0; i < _size; ++i) {
      if (other.data[i] != data[i])
        return false;
    }
    return true;
  }

  T &operator[](std::size_t idx) { return data[idx]; }

  ArrayList operator+(const ArrayList &other) const {
    ArrayList ret;
    ret.data = std::make_unique<T[]>(other._size + _size);
    ret._size = _size + other._size;
    std::copy(begin(), end(), ret.data.get());
    std::copy(other.begin(), other.end(), ret.data.get() + _size);
    return ret;
  }

  ArrayList &operator+=(const ArrayList &other) {
    std::unique_ptr<T[]> new_data = std::make_unique<T[]>(other._size + _size);
    std::copy(begin(), end(), new_data.get());
    std::copy(other.begin(), other.end(), new_data.get() + _size);
    data = std::move(new_data);
    _size += other._size;
    return *this;
  }

  bool contains(const T &item) const {
    return std::find(begin(), end(), item) != end();
  }

  void insert(std::size_t index, const T &item) {
    if (index >= _size) {
      throw std::invalid_argument(
          "cannot insert item at index " + std::to_string(index) +
          " for ArrayList of size " + std::to_string(_size));
    }
    std::unique_ptr<T[]> tmp = std::make_unique<T[]>(_size + 1);
    std::copy(data.get(), data.get() + index, tmp.get());
    tmp[index] = item;
    std::copy(data.get() + index, data.get() + _size, tmp.get() + index + 1);
    data = std::move(tmp);
    _size++;
  }

  void insert(std::size_t index, const ArrayList &items) {
    if (index >= _size) {
      throw std::invalid_argument(
          "cannot insert items at index " + std::to_string(index) +
          " for ArrayList of size " + std::to_string(_size));
    }
    if (items.empty())
      return;
    std::unique_ptr<T[]> tmp = std::make_unique<T[]>(_size + items._size);
    std::copy(data.get(), data.get() + index, tmp.get());
    std::copy(items.begin(), items.end(), tmp.get() + index);
    std::copy(data.get() + index, data.get() + _size,
              tmp.get() + index + items._size);
    data = std::move(tmp);
    _size += items._size;
  }

  void remove(std::size_t index) {
    if (index >= _size) {
      throw std::invalid_argument(
          "cannot remove element at index " + std::to_string(index) +
          " for ArrayList of size " + std::to_string(_size));
    }
    std::unique_ptr<T[]> tmp = std::make_unique<T[]>(_size - 1);
    std::copy(data.get(), data.get() + index, tmp.get());
    std::copy(data.get() + index + 1, data.get() + _size, tmp.get() + index);
    data = std::move(tmp);
    _size--;
  }

  void remove(std::size_t start, std::size_t num_elems) {
    if ((start + num_elems) > _size) {
      throw std::invalid_argument(
          "cannot remove " + std::to_string(num_elems) +
          " elements starting from index " + std::to_string(start) +
          " for ArrayList of size " + std::to_string(_size));
    }
    std::unique_ptr<T[]> tmp = std::make_unique<T[]>(_size - num_elems);
    std::copy(data.get(), data.get() + start, tmp.get());
    std::copy(data.get() + start + num_elems, data.get() + _size,
              tmp.get() + start);
    data = std::move(tmp);
    _size -= num_elems;
  }

  bool empty() const { return _size == 0; }

  ArrayList slice(std::size_t start, std::size_t num_elems) {
    if ((start + num_elems) > _size) {
      throw std::invalid_argument(
          "cannot get slice of " + std::to_string(num_elems) +
          " elements starting from index " + std::to_string(start) +
          " for ArrayList of size " + std::to_string(_size));
    }
    ArrayList ret;
    ret.data = std::make_unique<T[]>(num_elems);
    std::copy(data.get() + start, data.get() + start + num_elems,
              ret.data.get());
    ret._size = num_elems;
    return ret;
  }

  std::size_t size() { return _size; }

  void append(const T &val) { *this += {val}; }

  void appendleft(const T &val) { insert(0, val); }

  T pop() {
    if (empty()) {
      throw std::runtime_error("cannot pop from empty list");
    }
    return data[--_size];
  }

  T popleft() {
    if (empty()) {
      throw std::runtime_error("cannot pop from empty list");
    }
    T ret = data[0];
    std::unique_ptr<T[]> tmp = std::make_unique<T[]>(_size - 1);
    std::copy(data.get() + 1, data.get() + _size, tmp.get());
    _size--;
    data = std::move(tmp);
    return ret;
  }

  ArrayListIterator<T> begin() { return {data.get(), 0}; }
  ArrayListConstIterator<T> begin() const { return {data.get(), 0}; }

  ArrayListIterator<T> end() { return {data.get(), _size}; }
  ArrayListConstIterator<T> end() const { return {data.get(), _size}; }

  ArrayListConstIterator<T> cbegin() const { return {data.get(), 0}; }
  ArrayListConstIterator<T> cend() const { return {data.get(), _size}; }
};

template <typename T> class ArrayListIterator {
  T *data;
  std::size_t idx;

public:
  using iterator_category = std::bidirectional_iterator_tag;
  using value_type = T;

  ArrayListIterator(T *_data, std::size_t _idx) : data(_data), idx(_idx) {}

  ArrayListIterator &operator++() {
    idx++;
    return *this;
  }

  ArrayListIterator operator++(int) { return {data, idx++}; }

  ArrayListIterator &operator--() {
    idx--;
    return *this;
  }

  ArrayListIterator operator--(int) { return {data, idx--}; }

  bool operator==(const ArrayListIterator &other) const {
    return other.idx == idx;
  }

  bool operator!=(const ArrayListIterator &other) const {
    return !(*this == other);
  }

  value_type &operator*() { return data[idx]; }

  const value_type &operator*() const { return data[idx]; }
};

template <typename T> class ArrayListConstIterator {
  T *data;
  std::size_t idx;

public:
  using iterator_category = std::bidirectional_iterator_tag;
  using value_type = T;

  ArrayListConstIterator(T *_data, std::size_t _idx) : data(_data), idx(_idx) {}

  ArrayListConstIterator &operator++() {
    idx++;
    return *this;
  }

  ArrayListConstIterator operator++(int) { return {data, idx++}; }

  ArrayListConstIterator &operator--() {
    idx--;
    return *this;
  }

  ArrayListConstIterator operator--(int) { return {data, idx--}; }

  bool operator==(const ArrayListConstIterator &other) const {
    return other.idx == idx;
  }

  bool operator!=(const ArrayListConstIterator &other) const {
    return !(*this == other);
  }

  const value_type &operator*() const { return data[idx]; }
};

#endif // !ARRAY_LIST_HPP
