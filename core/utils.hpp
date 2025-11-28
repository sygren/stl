#pragma once

#include "macros.hpp"

template<typename T>
NO_DISCARD inline constexpr T &&move(T &v) noexcept {
  return static_cast<T&&>(v);
}
