#include <functional>
#include <ostream>
#include <vector>

template<typename T, typename Comparator = std::greater_equal<T>> class PriorityQueue {
  void heap_swap_children(std::vector<T> &heap, std::size_t heap_size, std::size_t root_idx) {
    std::size_t largest = root_idx;
    std::size_t left_child_idx = (root_idx * 2) + 1;
    std::size_t right_child_idx = (root_idx * 2) + 2;

    if (left_child_idx < heap_size && Comparator()(heap[left_child_idx], heap[largest])) { largest = left_child_idx; }

    if (right_child_idx < heap_size && Comparator()(heap[right_child_idx], heap[largest])) {
      largest = right_child_idx;
    }

    if (largest != root_idx) {
      std::swap(heap[largest], heap[root_idx]);
      heap_swap_children(heap, heap_size, largest);
    }
  }

  void heapify() {
    for (int i = int(items.size() - 1); i >= 0; --i) { heap_swap_children(items, items.size(), i); }
  }

  std::vector<int> items;


public:
  using container_type = std::vector<T>;
  using value_compare = Comparator;
  using value_type = T;
  using size_type = typename std::vector<T>::size_type;
  using reference = typename std::vector<T>::reference;
  using const_reference = typename std::vector<T>::const_reference;

  PriorityQueue(std::initializer_list<T> il) {
    items = std::vector(il.begin(), il.end());
    heapify();
  }

  PriorityQueue() = default;

  std::size_t size() { return items.size(); }

  const int &top() { return items[0]; }

  void push(int val) {
    items.push_back(val);
    std::size_t idx = items.size() - 1;
    while (idx > 0) {
      std::size_t parent_idx = idx / 2;
      if (Comparator()(items[idx], items[parent_idx])) {
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

  template<typename Container> Container to_sorted_container() {
    std::vector<T> tmp(items);
    std::vector<T> buf;
    buf.reserve(size());
    while (!empty()) { buf.push_back(pop()); }
    items = tmp;
    return { buf.rbegin(), buf.rend() };
  }

  friend std::ostream &operator<<(std::ostream &out, const PriorityQueue &pq) {
    for (const auto &i : pq.items) { out << i << ' '; }
    return out;
  }
};
