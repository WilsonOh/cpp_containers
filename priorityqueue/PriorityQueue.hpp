#include "Iterable.hpp"
#include <functional>
#include <ostream>
#include <vector>

template<typename T> class PriorityQueue {


  void heap_swap_children(std::vector<T> &heap, std::size_t heap_size, std::size_t root_idx) {
    std::size_t largest = root_idx;
    std::size_t left_child_idx = (root_idx * 2) + 1;
    std::size_t right_child_idx = (root_idx * 2) + 2;

    if (left_child_idx < heap_size && comparator(heap[left_child_idx], heap[largest])) { largest = left_child_idx; }

    if (right_child_idx < heap_size && comparator(heap[right_child_idx], heap[largest])) { largest = right_child_idx; }

    if (largest != root_idx) {
      std::swap(heap[largest], heap[root_idx]);
      heap_swap_children(heap, heap_size, largest);
    }
  }

  void heapify() {
    for (int i = items.size() - 1; i >= 0; --i) { heap_swap_children(items, items.size(), i); }
  }

  using Comparator = std::function<bool(int, int)>;

  std::vector<int> items;

  Comparator comparator = std::greater<T>();


public:
  PriorityQueue(std::initializer_list<T> il) {
    items = std::vector(il.begin(), il.end());
    heapify();
  }

  PriorityQueue() = default;

  void push(int val) {
    items.push_back(val);
    std::size_t idx = items.size() - 1;
    while (idx > 0) {
      std::size_t parent_idx = idx / 2;
      if (comparator(items[idx], items[parent_idx])) {
        std::swap(items[idx], items[parent_idx]);
        idx = parent_idx;
      } else {
        break;
      }
    }
  }

  int pop() {
    std::iter_swap(items.begin(), items.end() - 1);
    heap_swap_children(items, items.size() - 1, 0);
    int ret = items.back();
    items.pop_back();
    return ret;
  }

  bool empty() const { return items.empty(); }

  friend std::ostream &operator<<(std::ostream &out, const PriorityQueue &pq) {
    for (const auto &i : pq._l) { out << i << ' '; }
    return out;
  }
};
