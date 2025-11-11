#include "core/array.hpp"

#include <string>
#include <iostream>
#include <vector>

int main() {
  int *leak = new int(10);
  std::vector<int> vec;
  Array<std::string> names; 
  names.Push("salut");
  names.Push("Je suis le goat");
  names.Push("de ce jeu");

  Array<int> id(5);
  id.Push(10);
  id.Push(9);

  for (auto it = names.Begin(); it != names.End(); it++)
    std::cout << *it << std::endl;

  return 0;
}
