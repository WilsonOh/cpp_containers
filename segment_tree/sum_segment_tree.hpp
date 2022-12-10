#include <cmath>
#include <initializer_list>
#include <iterator>
#include <limits>
#include <span>
#include <utility>
#include <vector>

class SumSegmentTree {

  std::vector<int> elems;

  std::vector<int> tree;

  const int empty_node = std::numeric_limits<int>::max();

  static std::size_t get_segment_tree_array_size(std::size_t num_elems) {
    return static_cast<std::size_t>(2 * std::pow(2, std::ceil(std::log2(num_elems))) - 1);
  }

  int construct_tree(std::size_t curr_idx, std::size_t low, std::size_t high) {
    if (low == high) { return tree[curr_idx] = elems[low]; }
    std::size_t mid = low + (high - low) / 2;
    return tree[curr_idx] =
             construct_tree((2 * curr_idx) + 1, low, mid) + construct_tree((2 * curr_idx) + 2, mid + 1, high);
  }

  int sum_range_helper(const std::size_t query_low,
    const std::size_t query_high,
    const std::size_t segment_low,
    const std::size_t segment_high,
    const std::size_t tree_idx) const {
    if (query_low <= segment_low && query_high >= segment_high) { return tree[tree_idx]; }
    if (segment_high < query_low || segment_low > query_high) { return 0; }
    std::size_t mid = segment_low + (segment_high - segment_low) / 2;
    return sum_range_helper(query_low, query_high, segment_low, mid, (2 * tree_idx) + 1)
           + sum_range_helper(query_low, query_high, mid + 1, segment_high, (2 * tree_idx) + 2);
  }

  void update_helper(const std::size_t segment_low,
    const std::size_t segment_high,
    const std::size_t update_idx,
    const std::size_t tree_idx,
    const int diff_val) {
    if (update_idx < segment_low || update_idx > segment_high) { return; }
    tree[tree_idx] += diff_val;
    if (segment_low == segment_high) { return; }
    std::size_t mid = segment_low + (segment_high - segment_low) / 2;
    update_helper(segment_low, mid, update_idx, (2 * tree_idx) + 1, diff_val);
    update_helper(mid + 1, segment_high, update_idx, (2 * tree_idx) + 2, diff_val);
  }

public:
  SumSegmentTree(const std::initializer_list<int> il)
    : elems(std::vector(il.begin(), il.end())),
      tree(std::vector<int>(get_segment_tree_array_size(il.size()), empty_node)) {
    construct_tree(0, 0, elems.size() - 1);
  }

  SumSegmentTree(const std::span<int> sp)
    : elems(std::vector(sp.begin(), sp.end())),
      tree(std::vector<int>(get_segment_tree_array_size(sp.size()), empty_node)) {
    construct_tree(0, 0, elems.size() - 1);
  }

  SumSegmentTree(const std::forward_iterator auto begin, const std::forward_iterator auto end)
    : elems(std::vector(begin, end)),
      tree(std::vector<int>(get_segment_tree_array_size(std::distance(begin, end)), empty_node)) {
    construct_tree(0, 0, elems.size() - 1);
  }

  const std::vector<int> &get_tree() const { return tree; }

  int range_sum(const std::size_t low, const std::size_t high) const {
    return sum_range_helper(low, high, 0, elems.size() - 1, 0);
  }

  void update(const std::size_t idx, const int new_val) {
    update_helper(0, elems.size() - 1, idx, 0, new_val - elems[idx]);
  }
};
