/**
 * Jingga
 *
 * @package   Stdlib
 * @copyright Dennis Eichhorn
 * @license   OMS License 1.0
 * @version   1.0.0
 * @link      https://jingga.app
 */
#ifndef STDLIB_COMPILER_H
#define STDLIB_COMPILER_H

#ifdef _MSC_VER
    void* aligned_alloc(size_t alignment, size_t size) {
        return _aligned_malloc(size, alignment);
    }

    void aligned_free(void* ptr) {
        _aligned_free(ptr);
    }
#else
    void aligned_free(void* ptr) {
        free(ptr);
    }
#endif

#endif
