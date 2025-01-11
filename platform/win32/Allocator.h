/**
 * Jingga
 *
 * @copyright Jingga
 * @license   OMS License 2.0
 * @version   1.0.0
 * @link      https://jingga.app
 */
#ifndef TOS_PLATFORM_WIN32_ALLOCATOR_H
#define TOS_PLATFORM_WIN32_ALLOCATOR_H

#include <malloc.h>
#include <windows.h>
#include "../../stdlib/Types.h"
#include "../../utils/TestUtils.h"

// @todo Currently alignment only effects the starting position, but it should also effect the ending/size
// @todo Consider to rename file to Allocator.h

inline
void* platform_alloc(size_t size)
{
    return VirtualAlloc(NULL, size, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
}

inline
void* platform_alloc_aligned(size_t size, int32 alignment)
{
    size = ROUND_TO_NEAREST(size, alignment);

    void* ptr = VirtualAlloc(NULL, size + alignment + sizeof(void*), MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
    ASSERT_SIMPLE(ptr);

    // We want an aligned memory area but mmap doesn't really support that.
    // That's why we have to manually offset our memory area.
    // However, when freeing the pointer later on we need the actual start of the memory area, not the manually offset one.
    void* aligned_ptr = (void *) (((uintptr_t) ptr + alignment + sizeof(void*) - 1) & ~(alignment - 1));
    ((void**) aligned_ptr)[-1] = ptr;

    return aligned_ptr;
}

inline
void platform_free(void** ptr) {
    VirtualFree(*ptr, 0, MEM_RELEASE);
    *ptr = NULL;
}

inline
void platform_aligned_free(void** aligned_ptr) {
    void* ptr = ((void**) *aligned_ptr)[-1];
    VirtualFree(ptr, 0, MEM_RELEASE);
    *aligned_ptr = NULL;
}

inline
void* platform_shared_alloc(HANDLE* fd, const char* name, size_t size)
{
    *fd = CreateFileMappingA(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, (DWORD) size, name);
    ASSERT_SIMPLE(*fd);

    void* shm_ptr = MapViewOfFile(*fd, FILE_MAP_ALL_ACCESS, 0, 0, size);
    ASSERT_SIMPLE(shm_ptr);

    return shm_ptr;
}

inline
void* platform_shared_open(HANDLE* fd, const char* name, size_t size)
{
    *fd = OpenFileMappingA(FILE_MAP_READ | FILE_MAP_WRITE, FALSE, name);
    ASSERT_SIMPLE(*fd);

    void* shm_ptr = MapViewOfFile(*fd, FILE_MAP_READ | FILE_MAP_WRITE, 0, 0, (DWORD) size);
    ASSERT_SIMPLE(shm_ptr);

    return shm_ptr;
}

inline
void platform_shared_free(HANDLE fd, const char*, void** ptr)
{
    UnmapViewOfFile(*ptr);
    CloseHandle(fd);
    *ptr = NULL;
}

inline
void platform_shared_close(HANDLE fd)
{
    CloseHandle(fd);
}

#endif