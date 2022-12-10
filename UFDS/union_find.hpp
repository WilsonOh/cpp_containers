#include <numeric>
#include <vector>

class UnionFind {
  std::vector<std::size_t> sets;
  std::vector<std::size_t> ranks;
  std::vector<std::size_t> set_sizes;
  std::size_t size;

public:
  UnionFind(std::size_t n)
      : sets(std::vector<std::size_t>(n)), ranks(std::vector<std::size_t>(n)),
        set_sizes(std::vector<std::size_t>(n, 1)), size(n) {
    std::iota(sets.begin(), sets.end(), 0);
  }

  std::size_t find(std::size_t n) {
    return (sets[n] == n) ? n : (sets[n] = find(sets[n]));
  }

  std::size_t num_disjoint_sets() { return size; }

  bool is_same_set(std::size_t i, std::size_t j) { return find(i) == find(j); }

  void unionSet(std::size_t i, std::size_t j) {
    if (is_same_set(i, j)) {
      return;
    }
    int x = find(i);
    int y = find(j);
    if (ranks[x] > ranks[y]) {
      std::swap(x, y);
    }
    sets[x] = y;
    if (ranks[x] == ranks[y]) {
      ++ranks[y];
    }
    set_sizes[y] += set_sizes[x];
    --size;
  }

  std::size_t get_rank(std::size_t n) { return ranks[find(n)]; }

  std::size_t get_size(std::size_t n) { return set_sizes[find(n)]; }
};
