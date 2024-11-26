/**
 * Jingga
 *
 * @copyright Jingga
 * @license   OMS License 2.0
 * @version   1.0.0
 * @link      https://jingga.app
 */
#ifndef TOS_PLATFORM_LINUX_ALLOCATOR_H
#define TOS_PLATFORM_LINUX_ALLOCATOR_H

#include <stdio.h>
#include <unistd.h>
#include <sys/mman.h>
#include <fcntl.h>
#include "../../stdlib/Types.h"
#include "../../utils/TestUtils.h"

// @todo Currently alignment only effects the starting position, but it should also effect the ending/size
// @todo Consider to rename file to Allocator.h

// @question Since we store at least the size of the memory in the beginning,
// does this have a negative impact on caching?
// Our Memory doesn't start at the cache line beginning but at least offset by sizeof(size_t)

inline
void* platform_alloc(size_t size)
{
    ssize_t page_size = sysconf(_SC_PAGESIZE);
    size = (size + sizeof(size_t) + page_size - 1) & ~(page_size - 1);

    void* ptr = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    ASSERT_SIMPLE(ptr != MAP_FAILED);

    *((size_t *) ptr) = size;

    return (void *) ((uintptr_t) ptr + sizeof(size_t));
}

inline
void* platform_alloc_aligned(size_t size, int32 alignment)
{
    ssize_t page_size = sysconf(_SC_PAGESIZE);
    if (alignment < page_size) {
        alignment = page_size;
    }

    size += alignment - 1 + sizeof(void *) + sizeof(size_t);

    void* ptr = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    ASSERT_SIMPLE(ptr != MAP_FAILED);

    // We want an aligned memory area but mmap doesn't really support that.
    // That's why we have to manually offset our memory area.
    // However, when freeing the pointer later on we need the actual start of the memory area, not the manually offset one.
    // We do the same with the size, which is required when freeing
    uintptr_t raw_address = (uintptr_t) ptr + sizeof(void *) + sizeof(size_t);
    void* aligned_ptr = (void *) ((raw_address + alignment - 1) & ~(alignment - 1));

    *((void **) ((uintptr_t) aligned_ptr - sizeof(void *) - sizeof(size_t))) = ptr;
    *((size_t *) ((uintptr_t) aligned_ptr - sizeof(size_t))) = size;

    return aligned_ptr;
}

inline
void platform_free(void** ptr) {
    void* actual_ptr = (void *) ((uintptr_t) *ptr - sizeof(size_t));
    munmap(actual_ptr, *((size_t *) actual_ptr));
    *ptr = NULL;
}

inline
void platform_aligned_free(void** aligned_ptr) {
    void* ptr = (void *) ((uintptr_t) *aligned_ptr - sizeof(void *) - sizeof(size_t));
    munmap(ptr, *((size_t *) ((uintptr_t) ptr + sizeof(void *))));
    *aligned_ptr = NULL;
}

inline
void* platform_shared_alloc(int32* fd, const char* name, size_t size)
{
    *fd = shm_open(name, O_CREAT | O_RDWR, 0666);
    ASSERT_SIMPLE(*fd != -1);

    ssize_t page_size = sysconf(_SC_PAGESIZE);
    size = (size + sizeof(size_t) + page_size - 1) & ~(page_size - 1);

    ftruncate(*fd, size);

    void* shm_ptr = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, *fd, 0);
    ASSERT_SIMPLE(shm_ptr);

    *((size_t *) shm_ptr) = size;

    return (void *) ((uintptr_t) shm_ptr + sizeof(size_t));
}

inline
void* platform_shared_open(int32* fd, const char* name, size_t size)
{
    *fd = shm_open(name, O_RDWR, 0666);
    ASSERT_SIMPLE(*fd != -1);

    ssize_t page_size = sysconf(_SC_PAGESIZE);
    size = (size + sizeof(size_t) + page_size - 1) & ~(page_size - 1);

    void* shm_ptr = mmap(NULL, size, PROT_READ, MAP_SHARED, *fd, 0);
    ASSERT_SIMPLE(shm_ptr);

    *((size_t *) shm_ptr) = size;

    return (void *) ((uintptr_t) shm_ptr + sizeof(size_t));
}

inline
void platform_shared_free(int32 fd, const char* name, void** ptr)
{
    munmap((void *) ((uintptr_t) *ptr - sizeof(size_t)), *((size_t *) ((uintptr_t) *ptr - sizeof(size_t))));
    *ptr = NULL;

    shm_unlink(name);
    close(fd);
}

inline
void platform_shared_close(int32 fd)
{
    close(fd);
}

#endif