#include <concepts>
#include <ranges>

template<typename Container>
concept Iterable = requires(Container c) {
  c.begin();
  c.end();
  c.cbegin();
  c.cend();
};
