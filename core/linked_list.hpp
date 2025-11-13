#pragma once

#include "alloc.hpp"
#include <cassert>
#include <cstddef>
template <typename T> struct Node {
  T val;
  Node *next;
};

template <typename T,
          AllocatorFor<Node<T>> Allocator = DefaultAllocator<Node<T>>>
class LinkedList {
public:
  LinkedList() = default;
  ~LinkedList();

  void PushFront(T &item);
  void PushBack(T &item);
  void Insert(size_t index, T &item);
  void Remove(size_t index);
  T Get(size_t index) const;
  T &GetRef(size_t index);

private:
  NO_UNIQUE_ADDRESS Allocator allocator_;
  Node<T> *start_ = nullptr;
  size_t length_ = 0;
};

template <typename T, AllocatorFor<Node<T>> A>
void LinkedList<T, A>::PushFront(T &item) {
  Node<T> *new_node = allocator_.Allocate(1);
  assert(new_node != nullptr);
  new (new_node + offsetof(Node<T>, val)) T(item);
  new_node->next = start_;
  start_ = new_node;
  length_++;
};

template <typename T, AllocatorFor<Node<T>> A>
void LinkedList<T, A>::PushBack(T &item) {
  Node<T> *new_node = allocator_.Allocate(1);
  assert(new_node != nullptr);
  new (new_node + offsetof(Node<T>, val)) T(item);
  new_node->next = nullptr;

  if (length_ > 0) {
    Node<T> *ptr = start_;
    while (ptr->next != nullptr)
      ptr = ptr->next;
    ptr->next = new_node;
  } else
    start_ = new_node;

  length_++;
}

template <typename T, AllocatorFor<Node<T>> A>
void LinkedList<T, A>::Insert(size_t index, T &item) {
  assert(index < length_);
  Node<T> *new_node = allocator_.Allocate(1);
  assert(new_node != nullptr);
  new (new_node + offsetof(Node<T>, val)) T(item);
  new_node->next = nullptr;

  if (length_ > 0) {
    Node<T> *ptr = start_;
    for (size_t i = 1; i < index; i++)
      ptr = ptr->next;
    new_node->next = ptr->next->next;
    ptr->next = new_node;
  } else
    start_ = new_node;

  length_++;
}

template <typename T, AllocatorFor<Node<T>> A>
void LinkedList<T, A>::Remove(size_t index) {
  assert(index < length_);

  Node<T> **ptr = &start_;

  for (size_t i = 0; i < length_; i++)
    ptr = &((*ptr)->next);

  Node<T> *tmp = *ptr;
  *ptr = (*ptr)->next;

  tmp->val.~T();
  allocator_.Free(tmp);

  length_--;
}

template <typename T, AllocatorFor<Node<T>> A> LinkedList<T, A>::~LinkedList() {
  Node<T> *ptr = start_;
  while (ptr != nullptr) {
    auto tmp = ptr;
    ptr = ptr->next;

    tmp->val.~T();
    allocator_.Free(tmp);
  }
}
