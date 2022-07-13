#include "ArrayList.hpp"
#include <assert.h>
#include <string>

void test_access() {
  ArrayList<int> list;
  for (int i = 0; i < 100; ++i) {
    list.append(i);
  }
  for (int i = 0; i < 100; ++i) {
    assert(list[i] == i);
    list[i] *= 2;
  }
  for (int i = 0; i < 100; ++i) {
    assert(list.popleft() == i * 2);
  }
  for (int i = 0; i < 100; ++i) {
    list.append(i);
  }
  for (int i = 99; i >= 0; --i) {
    assert(list.pop() == i);
  }
  assert(list.empty());
  assert(list.size() == 0);
  try {
    list.pop();
    assert(false && "pop error not caught");
  } catch (std::runtime_error) {
  }
}

void test_iterator() {
  ArrayList<int> list;
  for (int i = 0; i < 100; ++i) {
    list.append(i);
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
  ArrayList<std::string> list1{"one", "two", "three"};
  ArrayList<std::string> list2{"one", "two", "three"};
  assert(list1 == list2);
  list2.pop();
  assert(list1 != list2);
  ArrayList<std::string> list3(list1);
  list3[2] = "four";
  assert(list3 != list1);
}

int main(void) {
  std::cout << "Starting tests...\n";
  test_access();
  std::cout << "test access passed\n";
  test_iterator();
  std::cout << "test iterator passed\n";
  test_equality();
  std::cout << "test equality passed\n";
  std::cout << "All Tests Passed\n";
}
