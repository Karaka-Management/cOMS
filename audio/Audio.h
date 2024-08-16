/**
 * Jingga
 *
 * @copyright Jingga
 * @license   OMS License 2.0
 * @version   1.0.0
 * @link      https://jingga.app
 */
#ifndef TOS_AUDIO_H
#define TOS_AUDIO_H

#include "../stdlib/Types.h"

struct Audio {
    uint32 sample_rate; // bits_per_sample
    uint32 sample_size; // byte_per_bloc
    uint32 frequency;
    uint32 channels;
    uint32 bloc_size;
    uint32 byte_per_sec;
    uint32 size;
    byte* data; // owner of data
};

#endif
