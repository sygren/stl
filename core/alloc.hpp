#pragma once

#include <cstddef>

#define KS_ALLOC(T, size) (T *)KaosAllocate(sizeof(T) * size)
#define KS_FREE(item) KaosFree(item)

extern void *KaosAllocate(size_t size);
extern void  KaosFree(void *item);
