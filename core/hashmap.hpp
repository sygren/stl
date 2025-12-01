#pragma once

#include "array_list.hpp"
#include "optional.hpp"

#include <concepts>
#include <cstdint>

template <typename Hash, typename T>
concept HashFor = requires(T item) {
  { Hash::hash(item) } -> same_as<size_t>;
};

template <typename K, typename V, HashFor<K> Hash> class SwissTable {
private:
  template <typename T1, typename T2> struct Group {
    T1 key_;
    T2 val_;
  };

  static constexpr uint8_t EMPTY = 0b10000000;
  static constexpr uint8_t DELETED = 0b11111110;

  ArrayList<uint8_t> metadata_;
  ArrayList<Group<K, V>> table_;

public:
  optional<V> find(const K &key) {
    size_t h = Hash::hash(key);
    size_t h1 = h >> 7;
    size_t h2 = h & 0x7F;

    for (size_t i = h2; i < metadata_.length(); i++) {
      uint8_t ctrl = metadata_[i];
      if (ctrl == EMPTY) return optional<V>::none();
      if (ctrl == DELETED) continue;

      if (h2 == (metadata_[i] & 0x7F)) 
        return table_[h1];
    }

    return optional<V>::none();
  }

private:
};
