#include "Range.hpp"

Range::Range(long start, long end, long step)
    : _start(start), _end(end), _step(step) {
  if (step == 0) {
    throw std::invalid_argument("Cannot set step size to 0!");
  }
}

Range::Range(long start, long end) : _start(start), _end(end) {
  _step = start > end ? -1 : 1;
}

Range::Range(long end) : _end(end) {
  _start = 0;
  _step = 1;
}

RangeIterator Range::begin() { return RangeIterator{_start, _step, _end}; }

RangeIterator Range::end() {
  return RangeIterator{_step < 0 ? _end - 1 : _end, _step, _end};
}

RangeIterator::RangeIterator(long num, long step, long end)
    : _num(num), _step(step), _end(end) {}

RangeIterator &RangeIterator::operator++() {
  _num += _step;
  return *this;
}

RangeIterator RangeIterator::operator++(int) {
  RangeIterator ret = *this;
  ++(*this);
  return ret;
}

bool RangeIterator::operator==(const RangeIterator &other) const {
  return _step > 0 ? _num >= other._num : _num <= other._num;
}

bool RangeIterator::operator!=(const RangeIterator &other) const {
  return !(*this == other);
}

long RangeIterator::operator*() const { return _num; }
