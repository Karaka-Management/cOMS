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
    // bits per sample
    // usually 48000 or 44100
    uint32 sample_rate;

    // bytes per bloc
    // channel count * bit
    // usually 2 * 16 = 4
    uint32 sample_size;

    // position in the audio data
    // WARNING: not the byte position, but the index based on the sample size
    uint32 sample_index;

    uint32 latency;

    // how often has the audio_play been called (required for xaudio)
    uint32 sample_output;

    // 0 - 100
    int16 volume;

    // max buffer content/size
    uint32 buffer_size;

    // Actual samples inside the buffer
    // The buffer could be larger than the data to output
    uint32 sample_buffer_size;
    int16* buffer;

    bool is_playing = false;
    byte type = SOUND_API_DIRECT_SOUND;

    // @todo add more settings e.g. repeat etc
};

#endif
