/**
 * Jingga
 *
 * @copyright Jingga
 * @license   OMS License 2.0
 * @version   1.0.0
 * @link      https://jingga.app
 */
#ifndef TOS_PLATFORM_WIN32_ALLOCATION_H
#define TOS_PLATFORM_WIN32_ALLOCATION_H

#include <malloc.h>
#include <windows.h>
#include "../../stdlib/Types.h"

inline
void* aligned_alloc(size_t alignment, size_t size) {
    return _aligned_malloc(size, alignment);
}

inline
void aligned_free(void** ptr) {
    _aligned_free(*ptr);
    *ptr = NULL;
}

inline
void* platform_alloc(size_t size)
{
    return VirtualAlloc(NULL, size, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
}

inline
void* platform_alloc_aligned(size_t size, int32 alignment)
{
    void* ptr = VirtualAlloc(NULL, size + alignment + sizeof(void*), MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);

    void* aligned_ptr = (void*)(((uintptr_t)ptr + alignment + sizeof(void*) - 1) & ~(alignment - 1));
    ((void**) aligned_ptr)[-1] = ptr;

    return aligned_ptr;
}

inline
void platform_free(void** ptr, size_t) {
    VirtualFree(*ptr, 0, MEM_RELEASE);
    *ptr = NULL;
}

inline
void platform_aligned_free(void** aligned_ptr, size_t) {
    void* ptr = ((void**) *aligned_ptr)[-1];
    VirtualFree(ptr, 0, MEM_RELEASE);
    *aligned_ptr = NULL;
}
#endif