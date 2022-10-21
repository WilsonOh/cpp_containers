#ifndef HASHTABLE_HPP
#define HASHTABLE_HPP

#include <concepts>
#include <functional>
#include <list>
#include <optional>
#include <ostream>
#include <stdexcept>
#include <type_traits>
#include <utility>
#include <vector>

#define INITIAL_LOAD 97

template<typename T>
concept Hashable = requires(T t) {
  { std::hash<T>{}(t) } -> std::convertible_to<std::size_t>;
};


template<typename Key, typename Value> class hashtableIterator;

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

  void insert(Key &&key, Value &&value) {
    std::size_t idx = std::hash<Key>{}(key) % table.size();
    if (!table[idx]) {
      table[idx] = std::list<std::pair<Key, Value>>{ { std::forward<Key>(key), std::forward<Value>(value) } };
    } else {
      table[idx]->emplace_back(std::forward<Key>(key), std::forward<Value>(value));
    }
  }

  Value operator[](Key &&key) {
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

  void erase(Key &&key) {
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

  bool contains(Key &&key) {
    std::size_t idx = std::hash<Key>{}(key) % table.size();
    auto &item = table[idx];
    if (item) {
      return std::find_if(item->begin(), item->end(), [&key](const std::pair<Key, Value> &p) { return p.first == key; })
             != item->end();
    }
    return false;
  }

  hashtableIterator<Key, Value> begin() { return hashtableIterator(table, table.begin()); }

  hashtableIterator<Key, Value> end() { return hashtableIterator(table, table.end(), true); }
};

template<typename Key, typename Value> class hashtableIterator {
  using hashiterator = typename std::vector<std::optional<std::list<std::pair<Key, Value>>>>::iterator;
  using tabletype = std::vector<std::optional<std::list<std::pair<Key, Value>>>>;
  using listtype = std::list<std::pair<Key, Value>>;
  using listiterator = typename std::list<std::pair<Key, Value>>::iterator;
  hashiterator m_table_iter;
  tabletype m_table;
  listiterator m_list_iter;
  listtype m_list;

public:
  using value_type = std::pair<Key, Value>;

  explicit hashtableIterator(tabletype table, hashiterator iter, bool end = false)
    : m_table(table), m_table_iter(iter) {
    if (!end) {
      for (; m_table_iter != m_table.end(); ++m_table_iter) {
        if (m_table_iter->has_value()) {
          m_list = m_table_iter->value();
          m_list_iter = m_list.begin();
          break;
        }
      }
    }
  }

  hashtableIterator &operator++() {
    if (++m_list_iter == m_list.end()) {
      for (; m_table_iter != m_table.end(); ++m_table_iter) {
        if (m_table_iter->has_value()) {
          m_list = m_table_iter->value();
          m_list_iter = m_list.begin();
          break;
        }
      }
    }
    return *this;
  }

  hashtableIterator operator++(int) {
    hashiterator ret = *this;
    ++*this;
    return ret;
  }

  bool operator==(const hashtableIterator &other) const { return m_table_iter == other.m_table_iter; }

  bool operator!=(const hashtableIterator &other) const { return !(*this == other); }

  const value_type &operator*() const { return *m_list_iter; }

  value_type &operator*() { return *m_list_iter; }
};

#endif // !HASHTABLE_HPP
