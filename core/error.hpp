#pragma once

#include "enum_debug.hpp"
#include "optional.hpp"

#include <iostream>
#include <source_location>
#include <type_traits>

#define PROPAGATE_ERROR(expr)                                                  \
  do {                                                                         \
    auto macro__err__ = (expr);                                                \
    if (macro__err__.has_err()) {                                              \
      return macro__err__;                                                     \
    }                                                                          \
  } while (0)

template <typename T>
concept EnumType = std::is_enum_v<T>;

template <EnumType E> class error {
private:
  optional<E> err_;

public:
  error() : err_(optional<E>::none()) {}
  error(E err) : err_(err) {}

  constexpr static error none() { return error(); }

  bool has_err() const { return err_.has_value(); }
  E val() const { return err_.val(); }
#ifdef NDEBUG
  void abort_on_error() const {
    if (err_.has_value()) {
      std::cout << "Error: " << enum_name(err_.val()) << "\n";
      abort();
    }
  }
#else
  void abort_on_error(source_location loc = source_location::current()) const {
    if (err_.has_value()) {
      std::cerr << loc.file_name() << ":" << loc.line() << " - ";
      std::cout << "Error: " << enum_name(err_.val()) << "\n";
      abort();
    }
  }
#endif
};
