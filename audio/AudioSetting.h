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
#include "../math/matrix/MatrixFloat32.h"

#define SOUND_API_DIRECT_SOUND 0
#define SOUND_API_XAUDIO2 1

struct AudioSetting {
    f32 master_volume;

    // bits per sample
    // usually 48000 or 44100
    uint16 sample_rate;

    // bytes per bloc
    // channel count * bit
    // usually 2 * 16 = 4
    byte sample_size;

    // how often has the audio_play been called (required for xaudio)
    byte sample_output;

    // max buffer content/size
    uint32 buffer_size;

    // Actual samples inside the buffer
    // The buffer could be larger than the data to output
    uint32 sample_buffer_size;
    int16* buffer;

    byte type = SOUND_API_DIRECT_SOUND;
    byte latency;

    // @todo add more settings e.g. repeat etc
};

struct AudioLocationSetting {
    v3_f32 audio_location;
    v3_f32 audio_lookat;
    v3_f32 audio_moveto;
    f32 audio_velocity;
};

inline
void update_audio_location_settings(AudioLocationSetting* settings)
{
    vec3_normalize(&settings->audio_lookat);
}

#endif
