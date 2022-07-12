#include "LinkedList.hpp"
#include <assert.h>
#include <string>

void test_access() {
  LinkedList<int> list;
  for (int i = 0; i < 100; ++i) {
    list.push_right(i);
  }
  for (int i = 0; i < 100; ++i) {
    assert(list[i] == i);
    list[i] *= 2;
  }
  for (int i = 0; i < 100; ++i) {
    assert(list.pop_left() == i * 2);
  }
  for (int i = 0; i < 100; ++i) {
    list.push_left(i);
  }
  for (int i = 0; i < 100; ++i) {
    assert(list.pop_right() == i);
  }
  assert(list.empty());
  assert(list.size() == 0);
  try {
    list.pop_right();
    assert(false && "pop error not caught");
  } catch (std::runtime_error) {
  }
}

void test_iterator() {
  LinkedList<int> list;
  for (int i = 0; i < 100; ++i) {
    list.push_right(i);
  }
  int curr = 0;
  for (int &i : list) {
    assert(i == curr++);
    i *= i;
  }
  int curr2 = 0;
  for (const int &i : list) {
    int tmp = curr2 * curr2;
    assert(i == tmp);
    curr2++;
  }
}

void test_equality() {
  LinkedList<std::string> list1{"one", "two", "three"};
  LinkedList<std::string> list2{"one", "two", "three"};
  assert(list1 == list2);
  list2.pop_right();
  assert(list1 != list2);
  LinkedList<std::string> list3(list1);
  list3[2] = "four";
  assert(list3 != list1);
}

int main(void) {
  test_access();
  test_iterator();
  test_equality();
  std::cout << "All Tests Passed\n";
}
