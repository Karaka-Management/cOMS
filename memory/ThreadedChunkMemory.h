/**
 * Jingga
 *
 * @copyright Jingga
 * @license   OMS License 2.0
 * @version   1.0.0
 * @link      https://jingga.app
 */
#ifndef TOS_MEMORY_THREADED_CHUNK_MEMORY_H
#define TOS_MEMORY_THREADED_CHUNK_MEMORY_H

#include <string.h>
#include "../stdlib/Types.h"

#if _WIN32
    #include "../platform/win32/threading/Thread.h"
#elif __linux__
    #include "../platform/linux/threading/Thread.h"
#endif

struct ThreadedChunkMemory {
    byte* memory;

    uint64 count;
    uint64 size;
    int64 last_pos;
    uint32 chunk_size;
    int32 alignment;

    // length = count
    // free describes which locations are used and which are free
    uint64* free;

    // Chunk implementation ends here
    // The completeness indicates if the data is completely written to
    uint64* completeness;

    pthread_mutex_t mutex;
    pthread_cond_t cond;
};

#endif