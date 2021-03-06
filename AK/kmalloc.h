#pragma once

#ifndef KERNEL
#include <new>
#endif

#if defined(SERENITY) && defined(KERNEL)
#define AK_MAKE_ETERNAL \
public: \
    void* operator new(size_t size) { return kmalloc_eternal(size); } \
private:
#else
#define AK_MAKE_ETERNAL
#endif

#ifdef KERNEL
#include <Kernel/kmalloc.h>
#else
#include <LibC/stdlib.h>

extern "C" {

[[gnu::malloc, gnu::returns_nonnull]] void* kmalloc(size_t size);
[[gnu::malloc, gnu::returns_nonnull]] void* kmalloc_eternal(size_t);
[[gnu::returns_nonnull]] void* krealloc(void* ptr, size_t size);
void kfree(void* ptr);

}

#ifdef KERNEL
inline void* operator new(size_t, void* p) { return p; }
inline void* operator new[](size_t, void* p) { return p; }
#else

inline void* operator new(size_t size)
{
    return kmalloc(size);
}

inline void operator delete(void* ptr)
{
    return kfree(ptr);
}

inline void* operator new[](size_t size)
{
    return kmalloc(size);
}

inline void operator delete[](void* ptr)
{
    return kfree(ptr);
}

#endif

#endif
