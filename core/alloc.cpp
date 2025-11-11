#include "alloc.hpp"

#include <cstdlib>

void *KaosAllocate(size_t size) {
    return malloc(size);
}

void KaosFree(void *item) {
    free(item);
}
