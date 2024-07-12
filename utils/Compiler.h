/**
 * Jingga
 *
 * @copyright Jingga
 * @license   OMS License 2.0
 * @version   1.0.0
 * @link      https://jingga.app
 */
#ifndef TOS_UTILS_COMPILER_H
#define TOS_UTILS_COMPILER_H

#ifdef _MSC_VER
    inline
    void* aligned_alloc(size_t alignment, size_t size) {
        return _aligned_malloc(size, alignment);
    }

    inline
    void aligned_free(void* ptr) {
        _aligned_free(ptr);
    }
#else
    #include <stdlib.h>

    inline
    void aligned_free(void* ptr) {
        free(ptr);
    }
#endif

#endif
