#pragma once

#include "alloc.hpp"
#include "iterator.hpp"

template <typename K, typename V,
          AllocatorFor<K> AllocatorK = DefaultAllocator<K>,
          AllocatorFor<V> AllocatorV = DefaultAllocator<V>>
class HashMap {
public:
  HashMap();

  void Insert(K &key, V &item);
  void Delete(K &key);
  V Search(K &key);
  V &SearchRef(K &key);
  bool Contains(K &key);

private:
  struct Pair {
    K key_;
    V item_;
  };

public:
  Iterator<struct Pair> Begin() const;
  Iterator<struct Pair> End() const;

private:
  NO_UNIQUE_ADDRESS AllocatorK key_allocator_;
  NO_UNIQUE_ADDRESS AllocatorV value_allocator_;
};
