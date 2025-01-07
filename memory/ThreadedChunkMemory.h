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
#include "../thread/Thread.h"

struct ThreadedChunkMemory {
    byte* memory;

    uint64 size;
    uint32 last_pos;
    uint32 count;
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