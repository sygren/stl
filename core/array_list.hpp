#pragma once

#include "alloc.hpp"
#include "iterator.hpp"
#include "macros.hpp"
#include "result.hpp"
#include "utils.hpp"

#include <cassert>
#include <cstddef>
#include <cstring>

template <typename T, AllocatorFor<T> Allocator = DefaultAllocator<T>>
class ArrayList {
public:
  ArrayList() = default;
  ~ArrayList();

  static result<ArrayList, AllocationError> with_capacity(size_t capacity) {
    ArrayList array;
    array.ptr_ = array.allocator_.allocate(capacity);
    if (array.ptr_ == nullptr)
      return result<ArrayList, AllocationError>::err(
          AllocationError::COULD_NOT_ALLOCATE);

    return result<ArrayList, AllocationError>::val(array);
  };

  static result<ArrayList, AllocationError>
  with_capacity_initialize(size_t capacity, const T &item) {
    ArrayList array;
    array.ptr_ = array.allocator_.allocate(capacity);
    if (array.ptr_ == nullptr)
      return result<ArrayList, AllocationError>::err(
          AllocationError::COULD_NOT_ALLOCATE);

    for (size_t i = 0; i < array.length_; i++)
      new (array.ptr_ + i) T(item);

    array.capacity_ = capacity;
    return result<ArrayList, AllocationError>::val(array);
  };

  ArrayList(ArrayList &item) = delete;
  ArrayList &operator=(ArrayList &item) = delete;
  ArrayList(ArrayList &&item) = delete;
  ArrayList &operator=(ArrayList &&item) = delete;

  result<AllocationError> reserve(size_t capacity);
  result<AllocationError> resize(size_t size);
  T *ptr() { return ptr_; }
  NO_DISCARD size_t length() const { return length_; }
  NO_DISCARD size_t capacity() const { return capacity_; }

  NO_DISCARD T get(size_t index) const;
  NO_DISCARD T &get_ref(size_t index);
  NO_DISCARD const T &get_ref(size_t index) const;
  T &operator[](size_t index);
  const T &operator[](size_t index) const;

  result<AllocationError> push(const T &item);
  result<AllocationError> emplace(T &&item);
  void clear();

  Iterator<T> begin();
  Iterator<T> end();

private:
  NO_UNIQUE_ADDRESS Allocator allocator_;
  T *ptr_ = nullptr;
  size_t capacity_ = 0;
  size_t length_ = 0;

  result<AllocationError> reallocate(size_t capacity);
}; // class ArrayList<T, Allocator>

template <typename T, AllocatorFor<T> Allocator>
ArrayList<T, Allocator>::~ArrayList() {
  clear();
}

template <typename T, AllocatorFor<T> Allocator>
result<AllocationError> ArrayList<T, Allocator>::reserve(size_t capacity) {
  assert(capacity > 0);
  if (capacity > capacity_)
    PROPAGATE_RESULT(reallocate(capacity));
  return result<AllocationError>::ok();
}

template <typename T, AllocatorFor<T> Allocator>
result<AllocationError> ArrayList<T, Allocator>::resize(size_t size) {
  if (size < length_) {
    for (size_t i = size; i < length_; i++)
      ptr_[i].~T();
    length_ = size;
    return result<AllocationError>::ok();
  }

  if (size > capacity_)
    PROPAGATE_RESULT(reallocate(size));
  for (size_t i = length_; i < size; i++)
    new (ptr_ + i) T();

  length_ = size;
}

template <typename T, AllocatorFor<T> Allocator>
T ArrayList<T, Allocator>::get(size_t index) const {
  assert(index < length_);
  return ptr_[index];
}

template <typename T, AllocatorFor<T> Allocator>
T &ArrayList<T, Allocator>::get_ref(size_t index) {
  assert(index < length_);
  return ptr_[index];
}

template <typename T, AllocatorFor<T> Allocator>
const T &ArrayList<T, Allocator>::get_ref(size_t index) const {
  assert(index < length_);
  return static_cast<const T &>(ptr_[index]);
}

template <typename T, AllocatorFor<T> Allocator>
T &ArrayList<T, Allocator>::operator[](size_t index) {
  return get_ref(index);
}

template <typename T, AllocatorFor<T> Allocator>
const T &ArrayList<T, Allocator>::operator[](size_t index) const {
  return get_ref(index);
}

template <typename T, AllocatorFor<T> Allocator>
result<AllocationError> ArrayList<T, Allocator>::push(const T &item) {
  if (length_ + 1 > capacity_)
    PROPAGATE_RESULT(reallocate((capacity_ * 2) + 1));
  new (ptr_ + length_) T(item);
  length_++;
  return result<AllocationError>::ok();
}

template <typename T, AllocatorFor<T> Allocator>
result<AllocationError> ArrayList<T, Allocator>::emplace(T &&item) {
  if (length_ + 1 > capacity_)
    PROPAGATE_RESULT(reallocate((capacity_ * 2) + 1));
  new (ptr_ + length_) T(move(item));
  length_++;
  return result<AllocationError>::ok();
}

template <typename T, AllocatorFor<T> Allocator>
void ArrayList<T, Allocator>::clear() {
  for (size_t i = 0; i < length_; i++)
    ptr_[i].~T();
  allocator_.free(ptr_);
  ptr_ = nullptr;
  length_ = 0;
  capacity_ = 0;
}

template <typename T, AllocatorFor<T> Allocator>
Iterator<T> ArrayList<T, Allocator>::begin() {
  return Iterator<T>(ptr_, 0, length_);
}

template <typename T, AllocatorFor<T> Allocator>
Iterator<T> ArrayList<T, Allocator>::end() {
  return Iterator<T>(ptr_, length_, length_);
}

template <typename T, AllocatorFor<T> Allocator>
result<AllocationError> ArrayList<T, Allocator>::reallocate(size_t capacity) {
  assert(capacity > length_);
  capacity_ = capacity;
  T *new_ptr = allocator_.allocate(capacity_);
  if (new_ptr == nullptr)
    return AllocationError::COULD_NOT_ALLOCATE;
  if constexpr (std::is_trivially_copyable_v<T>) {
    memcpy(new_ptr, ptr_, length_ * sizeof(T));
  } else {
    for (size_t i = 0; i < length_; i++) {
      new (new_ptr + i) T(move(ptr_[i]));
      ptr_[i].~T();
    }
  }

  allocator_.free(ptr_);
  ptr_ = new_ptr;
  return result<AllocationError>::ok();
}
