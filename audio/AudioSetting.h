/**
 * Jingga
 *
 * @copyright Jingga
 * @license   OMS License 2.0
 * @version   1.0.0
 * @link      https://jingga.app
 */
#ifndef TOS_AUDIO_SETTING_H
#define TOS_AUDIO_SETTING_H

#include "../stdlib/Types.h"

#define SOUND_API_DIRECT_SOUND 0
#define SOUND_API_XAUDIO2 1

struct AudioSetting {
    uint32 sample_rate;
    uint32 sample_size;
    uint32 sample_index;
    uint32 latency;

    int16 volume;

    uint32 buffer_size;

    // Actual samples inside the buffer
    // The buffer could be larger than the data to output
    uint32 sample_buffer_size;
    int16* buffer;

    bool is_playing = false;
    byte type = SOUND_API_DIRECT_SOUND;
};

#endif
