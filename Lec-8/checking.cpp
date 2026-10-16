// perfect forwarding lets doi

#include "utils.h"
#include <iostream>
struct X {
  void print() const & { std::cout << "L value\n"; }
  void print() const && { std::cout << "R value\n"; }
};

template <typename T> void func(T &&obj) { utils::Forward<T>(obj).print(); }
int main() {
  const X a;
  func(a);
}
