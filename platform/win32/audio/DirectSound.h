/**
 * Jingga
 *
 * @copyright Jingga
 * @license   OMS License 2.0
 * @version   1.0.0
 * @link      https://jingga.app
 */
#ifndef TOS_SOUND_DIRECT_SOUND_H
#define TOS_SOUND_DIRECT_SOUND_H

#include <dsound.h>
#include <windows.h>

#include "../../../stdlib/Types.h"
#include "../../../audio/AudioSetting.h"
#include "../../../utils/MathUtils.h"
#include "../../../log/Log.h"

struct DirectSoundSetting {
    LPDIRECTSOUND8 audio_handle;
    LPDIRECTSOUNDBUFFER primary_buffer;
    LPDIRECTSOUNDBUFFER secondary_buffer;
};

// BEGIN: Dynamically load DirectSound
typedef HRESULT WINAPI audio_create(LPCGUID, LPDIRECTSOUND8*, LPUNKNOWN);
HRESULT WINAPI DirectSoundCreate8Stub(LPCGUID, LPDIRECTSOUND8*, LPUNKNOWN) {
    return 0;
}
// END: Dynamically load DirectSound

void audio_load(HWND hwnd, AudioSetting* setting, DirectSoundSetting* api_setting) {
    HMODULE lib = LoadLibraryExA((LPCSTR) "dsound.dll", NULL, LOAD_LIBRARY_SEARCH_SYSTEM32);
    if (!lib) {
        LOG(log_memory, "DirectSound: Couldn't load dsound.dll\n", log_fp, true, true);

        return;
    }

    audio_create* DirectSoundCreate8 = (audio_create *) GetProcAddress(lib, "DirectSoundCreate8");

    if (!DirectSoundCreate8 || !SUCCEEDED(DirectSoundCreate8(0, &api_setting->audio_handle, 0))) {
        LOG(log_memory, "DirectSound: DirectSoundCreate8 failed\n", log_fp, true, true);

        return;
    }

    if(!SUCCEEDED(api_setting->audio_handle->SetCooperativeLevel(hwnd, DSSCL_PRIORITY))) {
        LOG(log_memory, "DirectSound: SetCooperativeLevel failed.\n", log_fp, true, true);

        return;
    }

    WAVEFORMATEX wf = {};
    wf.wFormatTag = WAVE_FORMAT_PCM;
    wf.nChannels = 2;
    wf.wBitsPerSample = 16;
    wf.nBlockAlign = (wf.nChannels * wf.wBitsPerSample) / 8;
    wf.nSamplesPerSec = setting->sample_rate;
    wf.nAvgBytesPerSec = wf.nSamplesPerSec * wf.nBlockAlign;
    wf.cbSize = 0;

    // Create primary buffer
    DSBUFFERDESC buffer_desc;
    ZeroMemory(&buffer_desc, sizeof(DSBUFFERDESC));

    buffer_desc.dwSize = sizeof(DSBUFFERDESC);
    buffer_desc.dwFlags = DSBCAPS_PRIMARYBUFFER;

    if(!SUCCEEDED(api_setting->audio_handle->CreateSoundBuffer(&buffer_desc, &api_setting->primary_buffer, 0))) {
        LOG(log_memory, "DirectSound: CreateSoundBuffer1 failed.\n", log_fp, true, true);

        return;
    }

    if (!SUCCEEDED(api_setting->primary_buffer->SetFormat(&wf))) {
        LOG(log_memory, "DirectSound: SetFormat failed.\n", log_fp, true, true);

        return;
    }

    // Create secondary buffer
    DSBUFFERDESC buffer_desc2;
    ZeroMemory(&buffer_desc2, sizeof(DSBUFFERDESC));

    buffer_desc2.dwSize = sizeof(DSBUFFERDESC);
    // @todo check alterntaive flags
    buffer_desc2.dwFlags = DSBCAPS_CTRLFREQUENCY | DSBCAPS_CTRLFX | DSBCAPS_CTRLVOLUME | DSBCAPS_GETCURRENTPOSITION2;
    buffer_desc2.dwBufferBytes = setting->buffer_size;
    buffer_desc2.lpwfxFormat = &wf;

    if(!SUCCEEDED(api_setting->audio_handle->CreateSoundBuffer(&buffer_desc2, &api_setting->secondary_buffer, 0))) {
        LOG(log_memory, "DirectSound: CreateSoundBuffer2 failed.\n", log_fp, true, true);

        return;
    }
}

inline
void audio_play(AudioSetting* setting, DirectSoundSetting* api_setting)
{
    if (!api_setting->secondary_buffer) {
        return;
    }

    api_setting->secondary_buffer->Play(0, 0, DSBPLAY_LOOPING);
    setting->is_playing = true;
}

inline
void audio_free(AudioSetting*, DirectSoundSetting* api_setting)
{
    if (api_setting->audio_handle) {
        api_setting->audio_handle->Release();
    }

    if (api_setting->primary_buffer) {
        api_setting->primary_buffer->Release();
    }

    if (api_setting->secondary_buffer) {
        api_setting->secondary_buffer->Release();
    }
}

/**
 * Calculates the samples in bytes to generate for the buffer
 */
inline
uint32 audio_buffer_fillable(const AudioSetting* setting, const DirectSoundSetting* api_setting)
{
    DWORD player_cursor;
    DWORD write_cursor;
    if (!SUCCEEDED(api_setting->secondary_buffer->GetCurrentPosition(&player_cursor, &write_cursor))) {
        LOG(log_memory, "DirectSound: GetCurrentPosition failed.\n", log_fp, true, true);

        return 0;
    }

    DWORD bytes_to_lock = (setting->sample_index * setting->sample_size) % setting->buffer_size;
    DWORD bytes_to_write = 0;

    DWORD target_cursor = (player_cursor + (setting->latency * setting->sample_size)) % setting->buffer_size;

    if (bytes_to_lock == player_cursor) {
        bytes_to_write = setting->is_playing ? 0 : setting->buffer_size;
    } else if (bytes_to_lock > target_cursor) {
        bytes_to_write = setting->buffer_size - bytes_to_lock;
        bytes_to_write += target_cursor;
    } else {
        bytes_to_write = target_cursor - bytes_to_lock;
    }

    return bytes_to_write;
}

inline
void audio_play_buffer(AudioSetting* setting, DirectSoundSetting* api_setting)
{
    if (setting->sample_buffer_size == 0) {
        return;
    }

    if (!setting->is_playing) {
        audio_play(setting, api_setting);
    }

    void *region1;
    DWORD region1_size;

    void *region2;
    DWORD region2_size;

    DWORD bytes_to_lock = (setting->sample_index * setting->sample_size) % setting->buffer_size;

    api_setting->secondary_buffer->Lock(
        bytes_to_lock, setting->sample_buffer_size,
        &region1, &region1_size,
        &region2, &region2_size,
        0
    );

    memcpy(
        (void *) region1,
        (void *) setting->buffer,
        region1_size
    );

    if (region2_size > 0) {
        memcpy(
            (void *) region2,
            (void *) (setting->buffer + region1_size),
            region2_size
        );
    }

    api_setting->secondary_buffer->Unlock(region1, region1_size, region2, region2_size);

    setting->sample_index += setting->sample_buffer_size / setting->sample_size;
    setting->sample_buffer_size = 0;
}

#endif