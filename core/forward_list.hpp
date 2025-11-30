#pragma once

#include "alloc.hpp"
#include "macros.hpp"
#include "result.hpp"
#include <cassert>
#include <cstddef>

template <typename T> struct ForwardListNode {
  T val_;
  ForwardListNode *next_;
};

/* This is a simple Linked List */
template <typename T, AllocatorFor<ForwardListNode<T>> Allocator =
                          DefaultAllocator<ForwardListNode<T>>>
class ForwardList {
private:
  using Node = ForwardListNode<T>;

  NO_UNIQUE_ADDRESS Allocator allocator_;
  size_t length_ = 0;
  Node *start_ = nullptr;
  Node *end_ = nullptr;

public:
  ForwardList() = default;
  ~ForwardList() { cleanup(); }

  /* Copy semantics */
  ForwardList(const ForwardList &other) = delete;
  ForwardList &operator=(const ForwardList &other) = delete; // Copy semantics

  /* Move semantics */
  ForwardList(ForwardList &&other) = delete;
  ForwardList &operator=(ForwardList &&other) = delete; // Move semantics

  NO_DISCARD size_t length() const { return length_; }

  NO_DISCARD const T &get(size_t index) const {
    assert(index < length_);
    return get_node(index)->val_;
  }
  NO_DISCARD T &get_ref(size_t index) {
    assert(index < length_);
    return get_node(index)->val_;
  }
  T &operator[](size_t index) { return get_ref(index); }

  result<AllocationError> insert(size_t index, const T &item) {
    assert(index < length_);
    auto *new_node = allocator_.allocate(1);
    if (new_node == nullptr)
      return AllocationError::COULD_NOT_ALLOCATE;

    new (&(new_node->val_)) T(item);
    new_node->next_ = nullptr;

    auto ptr = start_;
    for (size_t i = 0; i < index-1; i++)
      ptr = ptr->next_;

    new_node = ptr->next_->next_;
    ptr->next_ = new_node;

    return result<AllocationError>::ok();
  }

  result<AllocationError> push(const T &item) {
    auto *new_node = allocator_.allocate(1);
    if (new_node == nullptr)
      return AllocationError::COULD_NOT_ALLOCATE;

    new (&(new_node->val_)) T(item);
    new_node->next_ = nullptr;

    if (length_ == 0) {
      start_ = new_node;
      end_ = start_;
    } else {
      end_->next_ = new_node;
      end_ = new_node;
    }

    length_++;
    return result<AllocationError>::ok();
  }

  result<AllocationError> push_front(const T &item) {
    auto *new_node = allocator_.allocate(1);
    if (new_node == nullptr)
      return AllocationError::COULD_NOT_ALLOCATE;

    new (&(new_node->val_)) T(item);
    new_node->next_ = start_;

    start_ = new_node;
    if (length_ == 0)
      end_ = start_;

    length_++;
    return result<AllocationError>::ok();
  }

  void remove(size_t index) {
    assert(index < length_);
    auto ptr = get_node(index - 1);

    auto tmp = ptr->next_;
    ptr->next_ = ptr->next_->next_;

    tmp->val_.~T();
    allocator_.free(tmp);
  }

  void clear() { cleanup(); }

private:
  Node *get_node(size_t index) const {
    auto ptr = start_;
    for (size_t i = 0; i < index; i++)
      ptr = ptr->next_;
    return ptr;
  }

  inline void cleanup() {
    auto node_ptr = start_;
    for (size_t i = 0; i < length_; i++) {
      auto tmp = node_ptr;
      node_ptr = node_ptr->next_;
      tmp->val_.~T();
      allocator_.free(tmp);
    }
    length_ = 0;
    start_ = nullptr;
    end_ = nullptr;
  }
  // inline void copy_from(const ForwardList &other);
  // inline void move_from(ForwardList &&other);
}; // class ForwardList<T,Allocator>
