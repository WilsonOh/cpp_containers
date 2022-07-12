#ifndef LINKED_LIST_HPP
#define LINKED_LIST_HPP

#include <initializer_list>
#include <iostream>
#include <iterator>
#include <memory>
#include <ostream>
#include <string>

template <typename T> class LinkedListIterator;
template <typename T> class LinkedListConstIterator;

template <typename T> struct Node {
  T data;
  std::shared_ptr<Node> next;
  std::shared_ptr<Node> prev;

  Node(T _data) : data(_data) {}
};

template <typename T> class LinkedList {

  std::shared_ptr<Node<T>> head;
  std::shared_ptr<Node<T>> tail;
  std::size_t _size = 0;

public:
  LinkedList(std::initializer_list<T> l) {
    for (const T &val : l) {
      push_right(val);
    }
  }

  LinkedList() = default;

  LinkedList(const LinkedList &other) {
    for (const auto &i : other) {
      push_right(i);
    }
  }

  void push_right(const T &data) {
    _size++;
    Node<T> *new_node = new Node(data);
    if (empty()) {
      head.reset(new_node);
      tail = head;
    } else {
      tail->next.reset(new_node);
      tail->next->prev = tail;
      tail = tail->next;
    }
  }

  void push_left(const T &data) {
    _size++;
    Node<T> *new_node = new Node(data);
    if (empty()) {
      head.reset(new_node);
      tail = head;
    } else {
      head->prev.reset(new_node);
      head->prev->next = head;
      head = head->prev;
    }
  }

  T pop_right() {
    if (empty()) {
      throw std::runtime_error("cannot pop from empty list");
    }
    T ret = tail->data;
    if (_size == 1) {
      head.reset();
      _size -= 1;
      return ret;
    }
    tail = tail->prev;
    tail->next.reset();
    _size -= 1;
    return ret;
  }

  T pop_left() {
    if (empty()) {
      throw std::runtime_error("cannot pop from empty list");
    }
    T ret = head->data;
    if (_size == 1) {
      head.reset();
      _size -= 1;
      return ret;
    }
    _size -= 1;
    head = head->next;
    return ret;
  }

  T first() { return head->data; }

  T last() { return tail->data; }

  bool empty() const { return head == nullptr; }

  LinkedList operator+(const LinkedList &other) const {
    LinkedList tmp(*this);
    for (const auto &i : other) {
      tmp.push_right(i);
    }
    return tmp;
  }

  LinkedList &operator+=(const LinkedList &other) {
    for (const auto &i : other) {
      push_right(i);
    }
    return *this;
  }

  T &operator[](const std::size_t &idx) {
    if (idx >= _size) {
      throw std::invalid_argument("cannot access index " + std::to_string(idx) +
                                  " for list of size " + std::to_string(_size));
    }
    if (idx > _size / 2) {
      std::size_t curr_idx = _size - idx - 1;
      auto curr = tail;
      while (curr_idx--) {
        curr = curr->prev;
      }
      return curr->data;
    }
    std::size_t curr_idx = idx;
    auto curr = head;
    while (curr_idx--) {
      curr = curr->next;
    }
    return curr->data;
  }

  const T &operator[](const std::size_t &idx) const {
    if (idx >= _size) {
      throw std::invalid_argument("cannot access index " + std::to_string(idx) +
                                  " for list of size " + std::to_string(_size));
    }
    if (idx > _size / 2) {
      std::size_t curr_idx = _size - idx - 1;
      auto curr = tail;
      while (curr_idx--) {
        curr = curr->prev;
      }
      return curr->data;
    }
    std::size_t curr_idx = idx;
    auto curr = head;
    while (curr_idx--) {
      curr = curr->next;
    }
    return curr->data;
  }

  bool operator==(const LinkedList &other) const {
    if (_size != other._size)
      return false;
    auto it_a = cbegin();
    auto it_b = other.cbegin();
    while (it_a != cend() || it_b != other.cend()) {
      if (*it_a != *it_b)
        return false;
      ++it_b;
      ++it_a;
    }
    return true;
  }

  std::size_t size() { return _size; }

  LinkedListIterator<T> begin() { return {head}; }
  LinkedListConstIterator<T> begin() const { return {head}; }

  LinkedListIterator<T> end() { return {tail->next}; }
  LinkedListConstIterator<T> end() const { return {tail->next}; }

  LinkedListIterator<T> rbegin() { return {tail}; }
  LinkedListConstIterator<T> rbegin() const { return {tail}; }

  LinkedListIterator<T> rend() { return {head->prev}; }
  LinkedListConstIterator<T> rend() const { return {head->prev}; }

  LinkedListConstIterator<T> rcbegin() const { return {tail}; }
  LinkedListConstIterator<T> rcend() const { return {head->prev}; }

  LinkedListConstIterator<T> cbegin() const { return {head}; }
  LinkedListConstIterator<T> cend() const { return {tail->next}; }

  friend std::ostream &operator<<(std::ostream &out, const LinkedList &ll) {
    out << "[ ";
    for (const auto &i : ll) {
      out << i << ' ';
    }
    out << ']';
    return out;
  }
};

template <typename T> class LinkedListIterator {
  std::shared_ptr<Node<T>> curr;

  using iterator_category = std::bidirectional_iterator_tag;
  using value_type = T;

public:
  LinkedListIterator(std::shared_ptr<Node<T>> node) : curr(node) {}

  LinkedListIterator &operator++() {
    curr = curr->next;
    return *this;
  }

  LinkedListIterator operator++(int) {
    std::unique_ptr<Node<T>> tmp = curr;
    curr = curr->next;
    return {curr};
  }

  LinkedListIterator &operator--() {
    curr = curr->prev;
    return *this;
  }

  LinkedListIterator operator--(int) {
    std::unique_ptr<Node<T>> tmp = curr;
    curr = curr->prev;
    return {curr};
  }

  bool operator==(const LinkedListIterator &other) const {
    return other.curr == this->curr;
  }

  bool operator!=(const LinkedListIterator &other) const {
    return !(*this == other);
  }

  value_type &operator*() { return curr->data; }

  const value_type &operator*() const { return curr->data; }
};

template <typename T> class LinkedListConstIterator {
  std::shared_ptr<Node<T>> curr;

  using iterator_category = std::bidirectional_iterator_tag;
  using value_type = T;

public:
  LinkedListConstIterator(std::shared_ptr<Node<T>> node) : curr(node) {}

  LinkedListConstIterator &operator++() {
    curr = curr->next;
    return *this;
  }

  LinkedListConstIterator operator++(int) {
    std::unique_ptr<Node<T>> tmp = curr;
    curr = curr->next;
    return {curr};
  }

  LinkedListConstIterator &operator--() {
    curr = curr->prev;
    return *this;
  }

  LinkedListConstIterator operator--(int) {
    std::unique_ptr<Node<T>> tmp = curr;
    curr = curr->prev;
    return {curr};
  }

  bool operator==(const LinkedListConstIterator &other) const {
    return other.curr == this->curr;
  }

  bool operator!=(const LinkedListConstIterator &other) const {
    return !(*this == other);
  }

  const value_type &operator*() const { return curr->data; }
};

#endif // !LINKED_LIST_HPP
