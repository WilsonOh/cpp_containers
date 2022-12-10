#ifndef CIRCULAR_BUFFER_HPP
#define CIRCULAR_BUFFER_HPP

#include <array>
#include <cstddef>
#include <optional>
#include <stdexcept>
#include <utility>

template<typename T, std::size_t N> class circular_buffer {
  std::array<T, N> a{};
  std::size_t read_head{};
  std::size_t write_head{};
  std::size_t _size{};

public:
  circular_buffer() = default;

  std::optional<T> front() const {
    if (empty()) return std::nullopt;
    return a[read_head];
  }

  void queue(T &&val) {
    if (_size == N) return;
    a[write_head] = std::forward<T>(val);
    write_head = (write_head + 1) % N;
    _size++;
  }

  std::optional<T> dequeue() {
    if (empty()) { return std::nullopt; }
    T ret = a[read_head];
    read_head = (read_head + 1) % N;
    _size--;
    return ret;
  }

  std::size_t size() const { return _size; }

  bool empty() const { return _size == 0; }
};

#endif // !CIRCULAR_BUFFER_HPP
