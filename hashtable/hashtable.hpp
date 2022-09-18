#ifndef HASHTABLE_HPP
#define HASHTABLE_HPP

#include <concepts>
#include <functional>
#include <list>
#include <optional>
#include <ostream>
#include <stdexcept>
#include <type_traits>
#include <vector>

#define INITIAL_LOAD 97

template<typename T>
concept Hashable = requires(T t) {
  { std::hash<T>{}(t) } -> std::convertible_to<std::size_t>;
};

/**
 * Simple Hashtable which only accepts keys of type T which have std::hash<T> specialized,
 * and uses that hash function.
 *
 * Uses external chaining to deal with hash collisions.
 */
template<Hashable Key, typename Value> class hashtable {

  float loadfactor{};
  std::vector<std::optional<std::list<std::pair<Key, Value>>>> table;

public:
  hashtable() : table(std::vector<std::optional<std::list<std::pair<Key, Value>>>>(INITIAL_LOAD)) {}

  void insert(Key key, Value value) {
    std::size_t idx = std::hash<Key>{}(key) % table.size();
    if (!table[idx]) {
      table[idx] = std::list<std::pair<Key, Value>>{ { key, value } };
    } else {
      table[idx]->emplace_back(key, value);
    }
  }

  Value operator[](Key key) {
    std::size_t idx = std::hash<Key>{}(key) % table.size();
    auto item = table[idx];
    if (item) {
      auto it =
        std::find_if(item->begin(), item->end(), [&key](const std::pair<Key, Value> &p) { return p.first == key; });
      if (it == item->end()) { throw std::out_of_range("key does not exist"); }
      return it->second;
    }
    throw std::out_of_range("key does not exist");
  }

  void erase(Key key) {
    std::size_t idx = std::hash<Key>{}(key) % table.size();
    auto &item = table[idx];
    if (item) {
      auto it =
        std::find_if(item->begin(), item->end(), [&key](const std::pair<Key, Value> &p) { return p.first == key; });
      if (it == item->end()) { throw std::out_of_range("key does not exist"); }
      item->erase(it);
    } else {
      throw std::out_of_range("key does not exist");
    }
  }
};

#endif // !HASHTABLE_HPP
