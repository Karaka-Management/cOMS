/**
 * Jingga
 *
 * @copyright Jingga
 * @license   OMS License 2.0
 * @version   1.0.0
 * @link      https://jingga.app
 */
#ifndef TOS_AUDIO_C
#define TOS_AUDIO_C

#include "../utils/StringUtils.h"
#include "../memory/RingMemory.h"

#if _WIN32
    #include "../platform/win32/UtilsWin32.h"
#else
    #include "../platform/linux/UtilsLinux.h"
#endif

#include "Audio.h"
#include "AudioSetting.h"
#include "Wav.h"

void audio_from_file(RingMemory* ring, const char* path, Audio* audio)
{
    FileBody file;
    file_read(path, &file, ring);

    if (str_ends_with(path, ".wav")) {
        wav_audio_generate(&file, audio);
    }
}

inline
void audio_fill_buffer(AudioSetting* setting, uint32 to_fill, Audio* sound, int16* buffer1, int32 buffer1_size, int16* buffer2 = NULL, int32 buffer2_size = 0)
{
    uint32 limit = to_fill / setting->sample_size;
    buffer1_size /= setting->sample_size;
    buffer2_size /= setting->sample_size;
    uint32 sample_count = sound->size / sound->sample_size;
    f32 volume_scale = setting->volume * setting->volume;
    const int16* data = (int16*) sound->data;
    uint32 sample_index = setting->sample_index;

    uint32 i;
    for (i = 0; i < limit && i < buffer1_size; ++i) {
        if (sample_index >= sample_count) {
            sample_index = 0;

            // @question why are we doing this?
            setting->sample_index = 0;
        }

        buffer1[i * 2] = (int16) (data[sample_index * 2] * volume_scale);
        buffer1[i * 2 + 1] = (int16) (data[sample_index * 2 + 1] * volume_scale);

        ++sample_index;
    }

    for (; i < limit && buffer2_size; ++i) {
        if (sample_index >= sample_count) {
            sample_index = 0;

            // @question why are we doing this?
            setting->sample_index = 0;
        }

        buffer2[i * 2] = (int16) (data[sample_index * 2] * volume_scale);
        buffer2[i * 2 + 1] = (int16) (data[sample_index * 2 + 1] * volume_scale);

        ++sample_index;
    }

    ++setting->sample_output;
    setting->sample_buffer_size = to_fill;
}

// @bug This is slower
/*
inline
void fill_audio_buffer(AudioSetting* audio_handle, uint32 to_fill, Audio* sound, int32 steps = 8)
{
    uint32 limit = to_fill / audio_handle->sample_size;
    uint32 sample_count = sound->size / sound->sample_size;
    uint32 sample_index = audio_handle->sample_index;

    // Copy all data until end
    int32 offset;
    memcpy(
        audio_handle->buffer,
        sound->data + audio_handle->sample_index * audio_handle->sample_size,
        (offset = OMS_MIN(sample_count - sample_index, limit)) * audio_handle->sample_size
    );

    // If we are wrapping (play sound on loop)
    // @todo we still need a setting if we actually want to do this
    if (offset != limit) {
        audio_handle->sample_index = 0;
        memcpy(
            audio_handle->buffer + offset * audio_handle->sample_size,
            sound->data,
            (limit - offset) * audio_handle->sample_size
        );
    }

    simd_mult(
        (int16 *) audio_handle->buffer,
        (f32) audio_handle->volume / 100.0f,
        (int16 *) audio_handle->buffer, limit * 2,
        steps
    );

    ++audio_handle->sample_output;
    audio_handle->sample_buffer_size = to_fill;
}
*/

#endif
