#pragma once

#include "macros.hpp"
#include <type_traits>

template<typename T>
NO_DISCARD inline constexpr T &&move(T &v) noexcept {
  return static_cast<T&&>(v);
}

template<typename T, typename E>
struct is_same {
  static constexpr bool value = false;
};

template<typename T>
struct is_same<T,T> {
  static constexpr bool value = true;
};

template<typename T, typename E>
inline constexpr bool is_same_v = is_same<T,E>::value;

template<bool Condition, typename T, typename F>
struct conditionnal {
  using type = T;
}; 

template<typename T, typename F>
struct conditionnal<false, T, F> {
  using type = F;
}; 

template<bool Condition, typename T, typename F>
using condition_t = conditionnal<Condition, T, F>;

template<typename T>
struct should_return_by_value {
  static constexpr bool value =
    sizeof(T) <= 2 * sizeof(void*) && std::is_trivially_copyable_v<T>;
};

template<typename T>
inline constexpr bool should_return_by_value_v = should_return_by_value<T>::value;
