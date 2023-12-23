#include <iostream>

int main() {
  int x = 6;
  for (int i{}; i<10; i++) {
    x += 2;
  }
  std::cout << x << '\n';
}