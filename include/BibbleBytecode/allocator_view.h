// Copyright 2026 Jannik Laugmand Bülow

#ifndef BIBBLEBYTECODE_ALLOCATOR_VIEW_H
#define BIBBLEBYTECODE_ALLOCATOR_VIEW_H 1

#include <cstddef>

namespace bibblebytecode {
    struct AllocatorView {
        void* (*allocateFn)(void* ctx, size_t size);
        void (*deallocateFn)(void* ctx, void* ptr);
        void* ctx;

        void* allocate(size_t size) const { return allocateFn(ctx, size); }
        void deallocate(void* ptr) const { deallocateFn(ctx, ptr); }
    };
}

#endif //BIBBLEBYTECODE_ALLOCATOR_VIEW_H