#ifndef RANGE_HPP
#define RANGE_HPP

#include <iterator>

class RangeIterator : public std::iterator<std::input_iterator_tag, long> {
  long _num;
  long _step;
  long _end;

public:
  RangeIterator(long num, long step, long end);

  RangeIterator &operator++();

  RangeIterator operator++(int);

  bool operator==(const RangeIterator &other) const;

  bool operator!=(const RangeIterator &other) const;

  long operator*() const;
};

class Range {
  long _start;
  long _end;
  long _step;

public:
  Range(long start, long end, long step);
  Range(long start, long end);
  explicit Range(long end);

  RangeIterator begin();
  RangeIterator end();
};

#endif // !RANGE_HPP
