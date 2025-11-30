#pragma once 

#include "array.hpp"

#include <string_view>
using namespace std;

template<typename T>
concept DebuggableEnum = requires (T t) {
  { enum_name(t) };
};

template<typename Enum, Enum Variant>
constexpr auto enum_variant_impl() {
  string_view name = __PRETTY_FUNCTION__;
  auto begin = name.find_last_of("=") + 2;
  auto end = name.find_last_of("]");
  return name.substr(begin, end-begin);
};

template<typename Enum, unsigned int Index, unsigned int Range>
constexpr void fill_table(Array<string_view,Range> &table) {
  if constexpr (Index < Range) {
    table.push(enum_variant_impl<Enum, static_cast<Enum>(Index)>());
    fill_table<Enum, Index + 1, Range>(table);
  }
}

template<typename Enum, unsigned int Range = 100>
string_view enum_name(Enum variant) {
  constexpr auto make_table = []() {
    Array<string_view, Range> table;
    fill_table<Enum, 0, Range>(table);
    return table;
  };
  static auto table = make_table();

  int index = static_cast<int>(variant);
  if (index < 0 || index >= Range) {
    return "UNKNOWN";
  }

  return table[index];
}
