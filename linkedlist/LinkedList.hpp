#ifndef LINKED_LIST_HPP
#define LINKED_LIST_HPP

#include <algorithm>
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
  std::unique_ptr<Node> next;
  Node *prev;

  Node(T _data) : data(_data) {}
};

template <typename T> class LinkedList {

  std::unique_ptr<Node<T>> head;
  Node<T> *tail;
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
      tail = new_node;
      head->prev = nullptr;
    } else {
      tail->next.reset(new_node);
      tail->next->prev = tail;
      tail = new_node;
    }
  }

  void push_left(const T &data) {
    _size++;
    Node<T> *new_node = new Node(data);
    if (empty()) {
      head.reset(new_node);
      tail = new_node;
      head->prev = nullptr;
    } else {
      head->prev = new_node;
      head->prev->next.reset(new_node);
      head.swap(head->prev->next);
      head->prev = nullptr;
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
    head = std::move(head->next);
    head->prev = nullptr;
    return ret;
  }

  void insert(std::size_t pos, LinkedList<T> other) {
    Node<T> *curr = head.get();
    while (pos--) {
      curr = curr->next.get();
    }
    curr->next->prev = other.tail;
    curr->next.swap(other.tail->next);
    curr->next = std::move(other.head);
    curr->next->prev = curr;
  }

  void insert(std::size_t pos, T val) {
    if (pos < 0 || pos > this->size()) {
      return;
    }
    if (pos == 0) {
      push_left(val);
      return;
    }
    if (pos == this->size()) {
      push_right(val);
      return;
    }
    Node<T> *new_node = new Node(val);
    Node<T> *curr = head.get();
    while (pos--) {
      curr = curr->next.get();
    }
    curr->prev->next.swap(new_node->next);
    curr->prev->next.reset(new_node);
    new_node->prev = curr->prev;
    new_node->next->prev = new_node;
    _size++;
  }

  void remove(std::size_t pos) {
    if (pos < 0 || pos > this->size()) {
      return;
    }
    if (pos == 0) {
      pop_left();
      return;
    }
    if (pos == this->size()) {
      pop_right();
      return;
    }
    Node<T> *curr = head.get();
    while (pos--) {
      curr = curr->next.get();
    }
    curr->next->prev = curr->prev;
    curr->prev->next.swap(curr->next);
    _size--;
  }

  void remove(std::size_t start, std::size_t num_elems) {
    Node<T> *start_ptr = head.get();
    while (start--) {
      start_ptr = start_ptr->next.get();
    }
    Node<T> *end_ptr = start_ptr->prev;
    while (num_elems--) {
      end_ptr = end_ptr->next.get();
    }
    end_ptr->next->prev = start_ptr->prev;
    start_ptr->prev->next.swap(end_ptr->next);
  }

  void print_rev() {
    Node<T> *curr = tail;
    while (curr != nullptr) {
      std::cout << curr->data << ' ';
      curr = curr->prev;
    }
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
      Node<T> *curr = tail;
      while (curr_idx--) {
        curr = curr->prev;
      }
      return curr->data;
    }
    std::size_t curr_idx = idx;
    Node<T> *curr = head.get();
    while (curr_idx--) {
      curr = curr->next.get();
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
      Node<T> *curr = tail;
      while (curr_idx--) {
        curr = curr->prev;
      }
      return curr->data;
    }
    std::size_t curr_idx = idx;
    Node<T> *curr = head.get();
    while (curr_idx--) {
      curr = curr->next.get();
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

  LinkedListIterator<T> begin() { return {head.get()}; }
  LinkedListConstIterator<T> begin() const { return {head.get()}; }

  LinkedListIterator<T> end() { return {tail->next.get()}; }
  LinkedListConstIterator<T> end() const { return {tail->next.get()}; }

  LinkedListIterator<T> rbegin() { return {tail}; }
  LinkedListConstIterator<T> rbegin() const { return {tail}; }

  LinkedListIterator<T> rend() { return {head->prev}; }
  LinkedListConstIterator<T> rend() const { return {head->prev}; }

  LinkedListConstIterator<T> rcbegin() const { return {tail}; }
  LinkedListConstIterator<T> rcend() const { return {head->prev}; }

  LinkedListConstIterator<T> cbegin() const { return {head.get()}; }
  LinkedListConstIterator<T> cend() const { return {tail->next.get()}; }

  friend std::ostream &operator<<(std::ostream &out, const LinkedList &ll) {
    out << "[ ";
    for (auto &i : ll) {
      out << i << ' ';
    }
    out << ']';
    return out;
  }
};

template <typename T> class LinkedListIterator {
  Node<T> *curr;

  using iterator_category = std::bidirectional_iterator_tag;
  using value_type = T;

public:
  LinkedListIterator(Node<T> *node) : curr(node) {}

  LinkedListIterator &operator++() {
    curr = curr->next.get();
    return *this;
  }

  LinkedListIterator operator++(int) {
    Node<T> *tmp = curr;
    curr = curr->next.get();
    return {curr};
  }

  LinkedListIterator &operator--() {
    curr = curr->prev;
    return *this;
  }

  LinkedListIterator operator--(int) {
    Node<T> *tmp = curr;
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
  Node<T> *curr;

  using iterator_category = std::bidirectional_iterator_tag;
  using value_type = T;

public:
  LinkedListConstIterator(Node<T> *node) : curr(node) {}

  LinkedListConstIterator &operator++() {
    curr = curr->next.get();
    return *this;
  }

  LinkedListConstIterator operator++(int) {
    Node<T> *tmp = curr;
    curr = curr->next.get();
    return {curr};
  }

  LinkedListConstIterator &operator--() {
    curr = curr->prev;
    return *this;
  }

  LinkedListConstIterator operator--(int) {
    Node<T> *tmp = curr;
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
