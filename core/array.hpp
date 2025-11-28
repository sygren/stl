#pragma once

#include "macros.hpp"
#include "utils.hpp"

#include <cassert>
#include <cstddef>

template <typename T, unsigned int N> class Array {
private:
  T data_[N];
  size_t length_ = 0;

public:
  NO_DISCARD static constexpr inline unsigned int capacity() { return N; }

  Array() = default;

  explicit Array(size_t count) {
    assert(count < capacity());
    for (size_t i = 0; i < count; i++)
      new (data_ + i) T();
    length_ = count;
  }

  explicit Array(const T &item, size_t count) {
    assert(count < capacity());
    for (size_t i = 0; i < count; i++)
      new (data_ + i) T(item);
    length_ = count;
  }

  explicit Array(const T &item) {
    for (size_t i = 0; i < capacity(); i++)
      new (data_ + i) T(item);
    length_ = capacity();
  }

  /* Copy semantics */
  Array(const Array &other) { copy_from(other); }
  Array &operator=(const Array &other) {
    if (this == &other)
      return *this;
    cleanup();
    copy_from(other);
  } // Copy semantics

  /* Move semantics */
  Array(Array &&other) { move_from(move(other)); }
  Array &operator=(Array &&other) {
    if (this == &other)
      return *this;
    cleanup();
    move_from(move(other));
  } // Move semantics

  static Array zero_initialize() {
    Array arr;
    for (size_t i = 0; i < N; i++)
      new (arr.data_ + i) T();
    arr.length_ = capacity();
    return arr;
  };

  static Array zero_initialize_only(size_t count) {
    Array arr;
    assert(count < capacity());
    for (size_t i = 0; i < count; i++)
      new (arr.data_ + i) T();
    arr.length_ = count;
    return arr;
  };

  static Array initialize(const T &item) {
    Array arr;
    for (size_t i = 0; i < N; i++)
      new (arr.data_ + i) T(item);
    arr.length_ = capacity();
    return arr;
  };

  static Array initialize_only(const T &item, size_t count) {
    Array arr;
    assert(count < capacity());
    for (size_t i = 0; i < count; i++)
      new (arr.data_ + i) T(item);
    arr.length_ = count;
    return arr;
  };

  NO_DISCARD size_t length() const { return length_; }
  NO_DISCARD T *ptr() { return static_cast<T *>(data_); }

  NO_DISCARD const T &get(size_t index) const {
    assert(index < length_ && index < capacity());
    return data_[index];
  }

  NO_DISCARD T &get_ref(size_t index) {
    assert(index < length_ && index < capacity());
    return data_[index];
  }

  T &operator[](size_t index) {
    assert(index < length_ && index < capacity());
    return data_[index];
  }

  void push(const T &item) {
    assert(length_ + 1 <= capacity());
    new (data_ + length_) T(item);
    length_++;
  }

  void emplace(T &&item) {
    assert(length_ + 1 <= capacity());
    new (data_ + length_) T(move(item));
    length_++;
  }

  void clear() { cleanup(); }

private:
  inline void cleanup() {
    for (size_t i = 0; i < length_; i++)
      data_[i].~T();
    length_ = 0;
  };
  inline void copy_from(const Array &other) {
    for (size_t i = 0; i < other.length_; i++)
      new (data_ + i) T(other.get(i));
    length_ = other.length_;
  }
  inline void move_from(Array &&other) {
    for (size_t i = 0; i < other.length_; i++)
      new (data_ + i) T(move(other.get(i)));
    length_ = other.length_;
  }
}; // class Array<T, N>
