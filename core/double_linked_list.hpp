#pragma once

#include "alloc.hpp"
#include <cstddef>

template <typename T> struct DNode {
  T val;
  DNode *previous;
  DNode *next;
};

template <typename T, AllocatorFor<DNode<T>> Allocator> class DList {
public:
  DList() = default;
  void Insert(T &item);
  void Remove(T &item);
  T Get(size_t index);
  T &GetRef(size_t index);

private:
  NO_UNIQUE_ADDRESS Allocator allocator_;
  DNode<T> *start_ = nullptr;
  size_t length_ = 0;
  size_t capacity_ = 0;
};

template <typename T, AllocatorFor<DNode<T>> A> void DList<T, A>::Insert(T &item) {
  DNode<T> *ptr = start_;
  if (length_ == 0) {
    if (start_ == nullptr) {
      start_ = allocator_.Allocate(1);
      capacity_++;
    }

  }

  length_++;
}
