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
#include "../utils/StringUtils.h"
#include "../memory/RingMemory.h"

#if _WIN32
    #include "../platform/win32/UtilsWin32.h"
#else
    #include "../platform/linux/UtilsLinux.h"
#endif

#include "Audio.h"

struct Audio {
    // bits per sample
    // usually 48000 or 44100
    uint32 sample_rate;

    // bytes per bloc
    // channel count * bit
    // usually 2 * 16 = 4
    uint32 sample_size;

    // audio channels
    // usually 2
    uint32 channels;

    // usually 16 = 2
    uint32 bloc_size;

    // sample_rate * sample_size
    uint32 byte_per_sec;

    uint32 size;
    byte* data; // owner of data
};

#endif
