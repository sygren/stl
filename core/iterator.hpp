#pragma once

#include <cassert>
#include <cstddef>

template <typename T> class Iterator {
public:
  Iterator() = delete;
  Iterator(T *ptr, size_t index, size_t length)
      : ptr_(ptr), index_(index), length_(length) {};

  ~Iterator() = default;

  Iterator(Iterator &it) = default;
  Iterator(Iterator &&it) = default;

  Iterator &operator=(Iterator const &it) = default;
  Iterator &operator=(Iterator &&it) = default;

  Iterator &operator++();
  Iterator operator++(int);
  Iterator &operator--();
  bool operator==(Iterator &other);
  bool operator==(Iterator other);
  bool operator!=(Iterator &other);
  bool operator!=(Iterator other);
  T &operator*() {
    assert(ptr_ != nullptr);
    return *ptr_;
  }

private:
  T *ptr_ = nullptr;
  size_t index_ = 0;
  size_t length_ = 0;
}; // class Iterator

template <typename T> Iterator<T> &Iterator<T>::operator++() {
  if (index_++ < length_)
    ptr_++;
  return *this;
}

template <typename T> Iterator<T> Iterator<T>::operator++(int) {
  auto it = *this;
  if (index_++ < length_)
    ptr_++;
  return it;
}

template <typename T> Iterator<T> &Iterator<T>::operator--() {
  if (index_-- > 0)
    ptr_--;
  return *this;
}

template <typename T> bool Iterator<T>::operator==(Iterator &other) {
  return index_ == other.index_;
}

template <typename T> bool Iterator<T>::operator==(Iterator other) {
  return index_ == other.index_;
}

template <typename T> bool Iterator<T>::operator!=(Iterator &other) {
  return index_ != other.index_;
}

template <typename T> bool Iterator<T>::operator!=(Iterator other) {
  return index_ != other.index_;
}
