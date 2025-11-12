#pragma once

#include "alloc.hpp"
#include "iterator.hpp"

#include <cassert>
#include <cstddef>
#include <cstring>
#include <type_traits>
#include <utility>

template <typename T, AllocatorFor<T> Allocator = DefaultAllocator<T>>
class Array {
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
  NO_UNIQUE_ADDRESS Allocator allocator_;
  T *ptr_ = nullptr;
  size_t capacity_ = 0;
  size_t length_ = 0;

  void ReAllocate(size_t capacity);
}; // class Array

template <typename T, AllocatorFor<T> Allocator>
Array<T, Allocator>::Array(size_t capacity)
    : allocator_(), ptr_(allocator_.Allocate(capacity)), capacity_(capacity) {}

template <typename T, AllocatorFor<T> Allocator>
Array<T, Allocator>::Array(size_t capacity, const T &item)
    : allocator_(), ptr_(allocator_.Allocate(capacity)), capacity_(capacity),
      length_(capacity) {
  for (size_t i = 0; i < length_; i++)
    new (ptr_ + i) T(item);
}

template <typename T, AllocatorFor<T> Allocator> Array<T, Allocator>::~Array() {
  Clear();
}

template <typename T, AllocatorFor<T> Allocator>
void Array<T, Allocator>::Reserve(size_t capacity) {
  assert(capacity > 0);
  if (capacity > capacity_)
    ReAllocate(capacity);
}

template <typename T, AllocatorFor<T> Allocator>
void Array<T, Allocator>::Resize(size_t size) {
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

template <typename T, AllocatorFor<T> Allocator>
T Array<T, Allocator>::Get(size_t index) const {
  assert(index < length_);
  return ptr_[index];
}

template <typename T, AllocatorFor<T> Allocator>
T &Array<T, Allocator>::GetRef(size_t index) {
  assert(index < length_);
  return ptr_[index];
}

template <typename T, AllocatorFor<T> Allocator>
T &Array<T, Allocator>::operator[](size_t index) {
  return GetRef(index);
}

template <typename T, AllocatorFor<T> Allocator>
const T &Array<T, Allocator>::operator[](size_t index) const {
  return GetRef(index);
}

template <typename T, AllocatorFor<T> Allocator>
void Array<T, Allocator>::Push(const T &item) {
  if (length_ + 1 > capacity_)
    ReAllocate((capacity_ * 2) + 1);
  new (ptr_ + length_) T(item);
  length_++;
}

template <typename T, AllocatorFor<T> Allocator>
void Array<T, Allocator>::Push(T &&item) {
  if (length_ + 1 > capacity_)
    ReAllocate((capacity_ * 2) + 1);
  new (ptr_ + length_) T(std::move(item));
  length_++;
}

template <typename T, AllocatorFor<T> Allocator>
void Array<T, Allocator>::Clear() {
  for (size_t i = 0; i < length_; i++)
    ptr_[i].~T();
  allocator_.Free(ptr_);
  ptr_ = nullptr;
  length_ = 0;
  capacity_ = 0;
}

template <typename T, AllocatorFor<T> Allocator>
Iterator<T> Array<T, Allocator>::Begin() const {
  return Iterator<T>(ptr_, 0, length_);
}

template <typename T, AllocatorFor<T> Allocator>
Iterator<T> Array<T, Allocator>::End() const {
  return Iterator<T>(ptr_, length_, length_);
}

template <typename T, AllocatorFor<T> Allocator>
void Array<T, Allocator>::ReAllocate(size_t capacity) {
  assert(capacity > length_);
  capacity_ = capacity;
  T *new_ptr = allocator_.Allocate(capacity_);
  if constexpr (std::is_trivially_copyable_v<T>) {
    memcpy(new_ptr, ptr_, length_ * sizeof(T));
  } else {
    for (size_t i = 0; i < length_; i++) {
      new (new_ptr + i) T(std::move(ptr_[i]));
      ptr_[i].~T();
    }
  }

  allocator_.Free(ptr_);
  ptr_ = new_ptr;
}
