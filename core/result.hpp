#pragma once

#include "enum_debug.hpp"
#include "macros.hpp"
#include "optional.hpp"
#include "utils.hpp"

#include <cassert>
#include <iostream>
#include <source_location>

#define PROPAGATE_RESULT(expr)                                                 \
  do {                                                                         \
    auto macro__err__ = (expr);                                                \
    if (macro__err__.has_err()) {                                              \
      return macro__err__;                                                     \
    }                                                                          \
  } while (0)

template <typename T, typename E = void> struct result {
private:
  union U {
    NO_UNIQUE_ADDRESS T val_;
    NO_UNIQUE_ADDRESS E err_;

    U() {}
    ~U() {}
  };
  NO_UNIQUE_ADDRESS U u_;
  bool has_val_;

  result() = default;

  using Get_RT = conditional_t<should_return_by_value_v<T>, T, T &>;
  using Get_RT_Const = conditional_t<should_return_by_value_v<T>, T, const T &>;

  using Err_Get_RT = conditional_t<should_return_by_value_v<E>, E, E &>;
  using Err_Get_RT_Const =
      conditional_t<should_return_by_value_v<E>, E, const E &>;

public:
  struct error {
    E err_;
    explicit error(const E &e) : err_(e) {}
  };

  explicit result(const T &val) : has_val_(true) { new (&u_.val_) T(val); }
  explicit result(const error &err) : has_val_(false) {
    new (&u_.err_) E(err.err_);
  }

  static result val(const T &val) {
    result r;
    new (&r.u_.val_) T(val);
    r.has_val_ = true;
    return r;
  };

  static result err(const E &err) {
    result r;
    new (&r.u_.err_) E(err);
    r.has_val_ = false;
    return r;
  };

  ~result() { cleanup(); }

  /* Copy semantics */
  result(const result &r) { copy_from(r); }
  result &operator=(const result &r) {
    if (this == &r)
      return *this;
    cleanup();
    copy_from(r);
    return *this;
  }
  // Copy semantics

  /* Move semantics */
  result(result &&r) { move_from(move(r)); }
  result &operator=(result &&r) {
    if (this == &r)
      return *this;
    cleanup();
    move_from(move(r));
    return *this;
  } // Move semantics

  NO_DISCARD bool has_value() const noexcept { return has_val_; }
  NO_DISCARD bool has_err() const noexcept { return !has_val_; }

  NO_DISCARD Get_RT_Const val() const noexcept {
    assert(has_val_);
    return u_.val_;
  };

  NO_DISCARD Get_RT val() noexcept {
    assert(has_val_);
    return u_.val_;
  };

  NO_DISCARD Err_Get_RT_Const err() const noexcept {
    assert(!has_val_);
    return u_.err_;
  };

  NO_DISCARD Err_Get_RT err() noexcept {
    assert(!has_val_);
    return u_.err_;
  };

private:
  inline void cleanup() {
    if (has_val_)
      u_.val_.~T();
    else
      u_.err_.~E();
  }

  inline void copy_from(const result &r) {
    if (r.has_value())
      new (&u_.val_) T(r.val());
    else
      new (&u_.err_) E(r.err());

    has_val_ = r.has_val_;
  }

  inline void move_from(result &&r) {
    if (r.has_value())
      new (&u_.val_) T(move(r.u_.val_));
    else
      new (&u_.err_) E(move(r.u_.err_));

    has_val_ = r.has_val_;
  }
}; // class result<T,E>

template <typename T> struct result<T, void> {
private:
  optional<T> err_;

  using Get_RT = conditional_t<should_return_by_value_v<T>, T, T &>;
  using Get_RT_Const = conditional_t<should_return_by_value_v<T>, T, const T &>;

public:
  constexpr result() : err_(optional<T>::none()) {}
  result(const T &err) : err_(err) {}

  NO_DISCARD static constexpr result ok() { return result(); }
  NO_DISCARD static constexpr result err(const T &err) { return result(err); }

  NO_DISCARD bool has_err() { return err_.has_value(); }
  NO_DISCARD Get_RT_Const err() const {
    assert(err_.has_value());
    return err_.val();
  }
  NO_DISCARD Get_RT err() {
    assert(err_.has_value());
    return err_.val();
  }

#ifdef NDEBUG
  void abort_on_error() const {
    if (err_.has_value()) {
      std::cout << "Error: abort_on_error() " << "\n";
      abort();
    }
  }
#else
  void abort_on_error(
      std::source_location loc = std::source_location::current()) const {
    if (err_.has_value()) {
      std::cerr << loc.file_name() << ":" << loc.line() << "\n";
      if constexpr (DebuggableEnum<T>) {
        std::cout << "Error : " << enum_name(err_.val()) << "\n";
      }
      abort();
    }
  }
#endif
}; // class result<T>
