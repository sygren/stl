#include <iostream>

#include "core/array.hpp"
#include "core/arraylist.hpp"
#include "core/enum_debug.hpp"
#include "core/forwardlist.hpp"
#include "core/optional.hpp"
#include "core/result.hpp"

#include <cassert>

using namespace std;

enum class Error {
  Ok ,
  NotOk,
  Im_very_happy,
};

int main() {
  cout << "Start of the program" << endl;
  ArrayList<int> arr;
  cout << "checkpoint-2" << endl;

  Error err1 = Error::Ok;
  Error err2 = Error::NotOk;
  Error err3 = Error::Im_very_happy;

  cout << enum_name(err1) << endl;
  cout << enum_name(err2) << endl; 
  cout << enum_name(err3) << endl;

  arr.push(10);
  arr.push(10);
  arr.push(10);
  arr.push(10);
  cout << "checkpoint-1" << endl;

  for (auto it = arr.begin(); it != arr.end(); it++)
    assert(*it == 10);

  cout << "checkpoint0" << endl;
  Array<int, 10> fixed_arr;
  cout << "checkpoint1" << endl;
  assert(fixed_arr.length() == 0);
  assert(fixed_arr.capacity() == 10);

  cout << "checkpoint2" << endl;
  fixed_arr.push(10);
  fixed_arr.push(10);
  fixed_arr.push(10);
  fixed_arr.push(10);
  cout << "checkpoint3" << endl;

  assert(fixed_arr.length() == 4);

  for (size_t i = 0; i < fixed_arr.length(); i++)
    assert(fixed_arr[i] == 10 && fixed_arr.get(i) == 10);

  auto opt1 = optional<int>::wrap(9);
  assert(opt1.has_value());
  assert(opt1.val() == 9);

  auto opt2 = optional<int>::none();
  assert(!opt2.has_value());

  auto res1 = result<int, int>::val(9);
  assert(res1.has_value());
  assert(res1.val() == 9);

  auto res2 = result<int, int>::err(8);
  assert(!res2.has_value());
  assert(res2.err() == 8);

  ForwardList<int> list;
  assert(list.length() == 0);
  list.push(10).abort_on_error();
  list.push(10).abort_on_error();
  list.push_front(2).abort_on_error();
  assert(list.length() == 3);
  assert(list[0] == 2 && list.get(0) == 2);
  assert(list[1] == 10 && list.get(1) == 10);
  assert(list[2] == 10 && list.get(2) == 10);

  std::cout << "Everything worked flawlessly, good job\n";
  return 0;
}
