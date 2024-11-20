/**
 * Jingga
 *
 * @copyright Jingga
 * @license   OMS License 2.0
 * @version   1.0.0
 * @link      https://jingga.app
 */
#ifndef TOS_PLATFORM_LINUX_ALLOCATION_H
#define TOS_PLATFORM_LINUX_ALLOCATION_H

#include <unistd.h>
#include <sys/mman.h>

inline
void aligned_free(void** ptr) {
    free(*ptr);
    *ptr = NULL;
}

inline
void* platform_alloc(size_t size)
{
    ssize_t page_size = sysconf(_SC_PAGESIZE);

    size = (size + page_size - 1) & ~(page_size - 1);

    return mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
}

inline
void* platform_alloc_aligned(size_t size, int32 alignment)
{
    ssize_t page_size = sysconf(_SC_PAGESIZE);
    if (alignment < page_size) {
        alignment = page_size;
    }

    size = (size + alignment - 1) & ~(alignment - 1);

    void* ptr = mmap(NULL, size + alignment + sizeof(void*), PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

    void* aligned_ptr = (void*)(((uintptr_t)ptr + alignment + sizeof(void*) - 1) & ~(alignment - 1));
    ((void**) aligned_ptr)[-1] = ptr;

    return aligned_ptr;
}

inline
void platform_free(void** ptr, size_t size) {
    munmap(*ptr, size);
    *ptr = NULL;
}

inline
void platform_aligned_free(void** aligned_ptr, size_t size) {
    void* ptr = ((void**) *aligned_ptr)[-1];
    munmap(ptr, size + ((uintptr_t) *aligned_ptr - (uintptr_t)ptr));
    *aligned_ptr = NULL;
}
#endif