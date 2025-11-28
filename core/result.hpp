#pragma once

#include "macros.hpp"
#include "utils.hpp"

#include <cassert>
#include <cstdlib>
#include <iostream>
#include <source_location>

template <typename T, typename E> struct result {
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
    if (this == &r) return *this;
    cleanup();
    copy_from(r);
    return *this;
  }
  // Copy semantics

  /* Move semantics */
  result(result &&r) { move_from(move(r)); }
  result &operator=(result &&r) {
    if (this == &r) return *this;
    cleanup();
    move_from(move(r));
    return *this;
  } // Move semantics

  NO_DISCARD bool has_value() const noexcept { return has_val_; }

  NO_DISCARD const T &val() const noexcept {
    assert(has_val_);
    return u_.val_;
  };

  NO_DISCARD T &val_ref() noexcept {
    assert(has_val_);
    return u_.val_;
  };

  NO_DISCARD const E &err() const noexcept {
    assert(!has_val_);
    return u_.err_;
  };

  NO_DISCARD E &err_ref() noexcept {
    assert(!has_val_);
    return u_.err_;
  };

#ifdef NDEBUG
  T abort_on_error() const {
    if (!has_val_) {
      std::cout << "Error: " << "\n";
      abort();
    }
  }
#else
  T abort_on_error(std::source_location loc = std::source_location::current()) const {
    if (!has_val_) {
      std::cerr << loc.file_name() << ":" << loc.line() << " - ";
      std::cout << "Error: " << "\n";
      abort();
    }
  }
#endif

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
