#pragma once

#include "alloc.hpp"
#include "iterator.hpp"

#include <cassert>
#include <cstddef>
#include <cstring>
#include <type_traits>
#include <utility>

template <typename T> class Array {
public:
  Array() = default;
  explicit Array(size_t capacity);
  explicit Array(size_t capacity, const T &item);

  ~Array();

  Array(Array &item) = delete;
  Array &operator=(Array &item) = delete;
  Array(Array &&item) = delete;
  Array &operator=(Array &&item) = delete;

  void Reserve(size_t capacity);
  void Resize(size_t size);
  T *Ptr() { return ptr_; }
  [[nodiscard]] size_t Length() const { return length_; }
  [[nodiscard]] size_t Capacity() const { return capacity_; }

  T Get(size_t index) const;
  T &GetRef(size_t index);
  T &operator[](size_t index);
  const T &operator[](size_t index) const;

  void Push(const T &item);
  void Push(T &&item);
  void Clear();

  Iterator<T> Begin() const;
  Iterator<T> End() const;

private:
  T *ptr_ = nullptr;
  size_t capacity_ = 0;
  size_t length_ = 0;

  void ReAllocate(size_t capacity);
}; // class Array

template <typename T>
Array<T>::Array(size_t capacity)
    : ptr_(KS_ALLOC(T, capacity)), capacity_(capacity) {}

template <typename T>
Array<T>::Array(size_t capacity, const T &item)
    : ptr_(KS_ALLOC(T, capacity)), capacity_(capacity), length_(capacity) {
  for (size_t i = 0; i < length_; i++)
    new (ptr_ + i) T(item);
}

template <typename T> Array<T>::~Array() { Clear(); }

template <typename T> void Array<T>::Reserve(size_t capacity) {
  assert(capacity > 0);
  if (capacity > capacity_)
    ReAllocate(capacity);
}

template <typename T> void Array<T>::Resize(size_t size) {
  if (size < length_) {
    for (size_t i = size; i < length_; i++)
      ptr_[i].~T();
    length_ = size;
    return;
  }

  if (size > capacity_)
    ReAllocate(size);
  for (size_t i = length_; i < size; i++)
    new (ptr_ + i) T();
  
  length_ = size;
}

template <typename T> T Array<T>::Get(size_t index) const {
  assert(index < length_);
  return ptr_[index];
}

template <typename T> T &Array<T>::GetRef(size_t index) {
  assert(index < length_);
  return ptr_[index];
}

template <typename T> T &Array<T>::operator[](size_t index) {
  return GetRef(index);
}

template <typename T> const T &Array<T>::operator[](size_t index) const {
  return GetRef(index);
}

template <typename T> void Array<T>::Push(const T &item) {
  if (length_ + 1 > capacity_)
    ReAllocate((capacity_ * 2) + 1);
  new (ptr_ + length_) T(item);
  length_++;
}

template <typename T> void Array<T>::Push(T &&item) {
  if (length_ + 1 > capacity_)
    ReAllocate((capacity_ * 2) + 1);
  new (ptr_ + length_) T(std::move(item));
  length_++;
}

template <typename T> void Array<T>::Clear() {
  for (size_t i = 0; i < length_; i++)
    ptr_[i].~T();
  KS_FREE(ptr_);
  ptr_ = nullptr;
  length_ = 0;
  capacity_ = 0;
}

template <typename T> Iterator<T> Array<T>::Begin() const {
  return Iterator<T>(ptr_, 0, length_);
}

template <typename T> Iterator<T> Array<T>::End() const {
  return Iterator<T>(ptr_, length_, length_);
}

template <typename T> void Array<T>::ReAllocate(size_t capacity) {
  assert(capacity > length_);
  capacity_ = capacity;
  T *new_ptr = KS_ALLOC(T, capacity_);
  if constexpr (std::is_trivially_copyable_v<T>) {
    memcpy(new_ptr, ptr_, length_ * sizeof(T));
  } else {
    for (size_t i = 0; i < length_; i++) {
      new (new_ptr + i) T(std::move(ptr_[i]));
      ptr_[i].~T();
    }
  }

  KS_FREE(ptr_);
  ptr_ = new_ptr;
}
