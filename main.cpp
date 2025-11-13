#include "core/array.hpp"

#include <string>
#include <iostream>

int main() {
  Array<std::string> names; 
  names.Push("salut");
  names.Push("Je suis le goat");
  names.Push("de ce jeu");

  Array<int> id(5);
  id.Push(10);
  id.Push(9);

  for (auto it = names.Begin(); it != names.End(); it++)
    std::cout << *it << "\n";

  return 0;
}
