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
    #include "../platform/win32/FileUtils.cpp"
#else
    #include "../platform/linux/FileUtils.cpp"
#endif

#include "Audio.h"
#include "AudioSetting.h"
#include "Wav.h"

void audio_from_file(Audio* audio, const char* path, RingMemory* ring)
{
    FileBody file;
    file_read(path, &file, ring);

    ASSERT_SIMPLE(file.size);

    if (str_ends_with(path, ".wav")) {
        wav_from_data(file.content, (uint32) file.size, audio, ring);
    }
}

int32 audio_data_size(const Audio* audio)
{
    return (int32) (audio->size
        + sizeof(audio->sample_rate)
        + sizeof(audio->sample_size)
        + sizeof(audio->channels)
        + sizeof(audio->bloc_size)
        + sizeof(audio->byte_per_sec)
        + sizeof(audio->size)
    );
}

int32 audio_from_data(const byte* data, Audio* audio)
{
    audio->sample_rate = SWAP_ENDIAN_LITTLE(*((uint16 *) data));
    data += sizeof(audio->sample_rate);

    audio->sample_size = *data;
    data += sizeof(audio->sample_size);

    audio->channels = *data;
    data += sizeof(audio->channels);

    audio->bloc_size = *data;
    data += sizeof(audio->bloc_size);

    audio->byte_per_sec = SWAP_ENDIAN_LITTLE(*((uint32 *) data));
    data += sizeof(audio->byte_per_sec);

    audio->size = SWAP_ENDIAN_LITTLE(*((uint32 *) data));
    data += sizeof(audio->size);

    memcpy(audio->data, data, audio->size);
    data += audio->size;

    return audio_data_size(audio);
}

int32 audio_to_data(const Audio* audio, byte* data)
{
    *((uint16 *) data) = SWAP_ENDIAN_LITTLE(audio->sample_rate);
    data += sizeof(audio->sample_rate);

    *data = audio->sample_size;
    data += sizeof(audio->sample_size);

    *data = audio->channels;
    data += sizeof(audio->channels);

    *data = audio->bloc_size;
    data += sizeof(audio->bloc_size);

    *((uint32 *) data) = SWAP_ENDIAN_LITTLE(audio->byte_per_sec);
    data += sizeof(audio->byte_per_sec);

    *((uint32 *) data) = SWAP_ENDIAN_LITTLE(audio->size);
    data += sizeof(audio->size);

    memcpy(data, audio->data, audio->size);
    data += audio->size;

    return audio_data_size(audio);
}

#endif
