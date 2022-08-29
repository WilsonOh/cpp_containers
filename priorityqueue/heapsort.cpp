#include <algorithm>
#include <iostream>
#include <vector>

void print_vec(const std::vector<int> &vec) {
  for (const auto &i : vec) {
    std::cout << i << ' ';
  }
  std::cout << '\n';
}

void heappush(std::vector<int> &heap, int val) {
  heap.push_back(val);
  int idx = heap.size() - 1;
  while (idx > 0) {
    int parent_idx = idx / 2;
    if (heap[idx] > heap[parent_idx]) {
      std::swap(heap[idx], heap[parent_idx]);
      idx = parent_idx;
    } else {
      break;
    }
  }
}

void heap_swap_children(std::vector<int> &heap, std::size_t heap_size,
                        std::size_t root_idx) {
  std::size_t largest = root_idx;
  std::size_t left_child_idx = (root_idx * 2) + 1;
  std::size_t right_child_idx = (root_idx * 2) + 2;

  if (left_child_idx < heap_size && heap[left_child_idx] > heap[largest]) {
    largest = left_child_idx;
  }

  if (right_child_idx < heap_size && heap[right_child_idx] > heap[largest]) {
    largest = right_child_idx;
  }

  if (largest != root_idx) {
    std::swap(heap[largest], heap[root_idx]);
    heap_swap_children(heap, heap_size, largest);
  }
}

int heappop(std::vector<int> &heap) {
  std::iter_swap(heap.begin(), heap.end() - 1);
  heap_swap_children(heap, heap.size() - 1, 0);
  int ret = heap.back();
  heap.pop_back();
  return ret;
}

void heapify(std::vector<int> &vec) {
  for (int i = vec.size() - 1; i >= 0; --i) {
    heap_swap_children(vec, vec.size(), i);
  }
}

void myheapsort(std::vector<int> &vec) {
  heapify(vec);
  for (int i = vec.size() - 1; i >= 0; --i) {
    std::swap(vec[i], vec[0]);
    heap_swap_children(vec, i, 0);
  }
}

int main() {
  std::vector<int> vec{10, 20, 15, 12, 40, 25, 18};
  heapify(vec);
  while (!vec.empty()) {
    std::cout << heappop(vec) << ' ';
  }
  print_vec(vec);
}
