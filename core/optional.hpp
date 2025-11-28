#pragma once

#include "macros.hpp"
#include "utils.hpp"
#include <cassert>

template <typename T> struct optional {
private:
  union U {
    T val_;

    U() {}
    ~U() {}
  };
  NO_UNIQUE_ADDRESS U u_;
  bool has_val_;

  optional() : has_val_(false) {}

public:
  struct none {};
  explicit optional(const T &v) {
    new (&u_.val_) T(v);
    has_val_ = true;
  }
  explicit optional(T &&v) {
    new (&u_.val_) T(move(v));
    has_val_ = true;
  }
  explicit optional(const none &_n) : has_val_(false) {}

  NO_DISCARD static optional wrap(const T &v) {
    optional o;
    new (&o.u_.val_) T(v);
    o.has_val_ = true;
    return o;
  };

  NO_DISCARD constexpr inline static optional none() {
    optional o;
    o.has_val_ = false;
    return o;
  }

  ~optional() { cleanup(); }

  /* Copy semantics */
  optional(const optional &o) { copy_from(o); }
  optional &operator=(const optional &o) {
    if (this == &o) return *this;
    cleanup();
    copy_from(o);
    return *this;
  } // Copy semantics

  /* Move semantics */
  optional(optional &&o) { move_from(move(o)); }
  optional &operator=(optional &&o) {
    if (this == &o) return *this;
    cleanup();
    move_from(move(o));
    return *this;
  } // Copy semantics

  NO_DISCARD bool has_value() const noexcept { return has_val_; }
  NO_DISCARD const T &val() const noexcept {
    assert(has_val_);
    return u_.val_;
  }
  NO_DISCARD T &val_ref() noexcept {
    assert(has_val_);
    return u_.val_;
  }

private:
  void cleanup() {
    if (has_val_)
      u_.val_.~T();
  }
  void copy_from(const optional &o) {
    if (o.has_val_)
      new (&u_.val_) T(o.u_.val_);
    has_val_ = o.has_val_;
  }
  void move_from(optional &&o) {
    if (o.has_val_)
      new (&u_.val_) T(move(o.u_.val_));
    has_val_ = o.has_val_;
  }
}; // class optional<T>
