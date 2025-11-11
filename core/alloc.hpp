#pragma once

#include <cstddef>
#include <cstdlib>
#include <concepts>
#include <type_traits>

/* This is a concept that checks if a certain type A, is an allocator for the type T.
 * It checks if the type A contains two methods:
 *  T *Allocate(size_t)
 *  void Free(T*)
 *  It also check if the type A is default constructible, meaning it has a default constructor without any arguments.
 * See bellow DefaultAllocator which verify this concept and is really just a wrapper around malloc/free.
 */
template<typename A, typename T>
concept AllocatorFor = requires (A alloc, size_t n, T *ptr) {
  { alloc.Allocate(n) } -> std::same_as<T*>;
  { alloc.Free(ptr) } -> std::same_as<void>;

  requires std::is_default_constructible_v<A>;
};

template <typename T> class DefaultAllocator {
public:
  DefaultAllocator()  = default;
  ~DefaultAllocator() = default;

  DefaultAllocator(DefaultAllocator<T> &allocator) = delete;
  DefaultAllocator &operator=(DefaultAllocator<T> &allocator) = delete;

  DefaultAllocator(DefaultAllocator<T> &&allocator) = delete;
  DefaultAllocator &operator=(DefaultAllocator<T> &&allocator) = delete;

  T *Allocate(size_t count);
  void Free(T *item);
};

template <typename T> T *DefaultAllocator<T>::Allocate(size_t count) {
  return static_cast<T*>(malloc(sizeof(T) * count));
}

template <typename T> void DefaultAllocator<T>::Free(T *item) {
  free(item);
}
