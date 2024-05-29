/**
 * Karaka
 *
 * @package   Stdlib
 * @copyright Dennis Eichhorn
 * @license   OMS License 1.0
 * @version   1.0.0
 * @link      https://jingga.app
 */
#ifndef SOUND_DIRECT_SOUND_H
#define SOUND_DIRECT_SOUND_H

#include <dsound.h>
#include <windows.h>
#include <math.h>

#include "../Stdlib/Types.h"
#include "../Utils/MathUtils.h"
#include "../UI/Window.h"

#define SOUND_API_DIRECT_SOUND 0

namespace Sound {
    struct DirectSoundSetting {
        uint32 sample_rate;
        uint32 sample_size;
        uint32 sample_index;
        int16 volume;
        uint32 buffer_size;
        int16* buffer;

        bool playing = false;
        byte type = SOUND_API_DIRECT_SOUND;

        // Api specific data from here on
        LPDIRECTSOUND8 direct_sound;
        LPDIRECTSOUNDBUFFER primary_buffer;
        LPDIRECTSOUNDBUFFER secondary_buffer;
    };

    // BEGIN: Dynamically load DirectSound
    typedef HRESULT WINAPI direct_sound_create(LPCGUID, LPDIRECTSOUND8*, LPUNKNOWN);
    HRESULT WINAPI DirectSoundCreate8Stub(LPCGUID, LPDIRECTSOUND8*, LPUNKNOWN) {
        return 0;
    }
    // END: Dynamically load DirectSound

    void direct_sound_load(UI::Window* w, DirectSoundSetting* setting) {
        HMODULE lib = LoadLibraryExA((LPCSTR) "dsound.dll", NULL, LOAD_LIBRARY_SEARCH_SYSTEM32);
        if (!lib) {
            // @todo Log
            return;
        }

        direct_sound_create* DirectSoundCreate8 = (direct_sound_create *) GetProcAddress(lib, "DirectSoundCreate8");

        if (!DirectSoundCreate8 || !SUCCEEDED(DirectSoundCreate8(0, &setting->direct_sound, 0))) {
            // @todo Log
            return;
        }

        if(!SUCCEEDED(setting->direct_sound->SetCooperativeLevel(w->hwnd, DSSCL_PRIORITY))) {
            // @todo Log
        }

        WAVEFORMATEX wf = {};
        wf.wFormatTag = WAVE_FORMAT_PCM;
        wf.nChannels = 2;
        wf.wBitsPerSample = 16;
        wf.nBlockAlign = (wf.nChannels * wf.wBitsPerSample) / 8;
        wf.nSamplesPerSec = setting->sample_rate;
        wf.nAvgBytesPerSec = wf.nSamplesPerSec * wf.nBlockAlign;
        wf.cbSize;

        // Create primary buffer
        DSBUFFERDESC bufferDesc;
        ZeroMemory(&bufferDesc, sizeof(DSBUFFERDESC));

        bufferDesc.dwSize = sizeof(DSBUFFERDESC);
        bufferDesc.dwFlags = DSBCAPS_PRIMARYBUFFER;

        if(!SUCCEEDED(setting->direct_sound->CreateSoundBuffer(&bufferDesc, &setting->primary_buffer, 0))) {
            // @todo Log
        }

        if (!SUCCEEDED(setting->primary_buffer->SetFormat(&wf))) {
            // @todo Log
        }

        setting->buffer_size = setting->sample_rate * setting->sample_size;

        // Create secondary buffer
        DSBUFFERDESC bufferDesc2;
        ZeroMemory(&bufferDesc2, sizeof(DSBUFFERDESC));

        bufferDesc2.dwSize = sizeof(DSBUFFERDESC);
        bufferDesc2.dwFlags = 0;
        bufferDesc2.dwBufferBytes = setting->buffer_size;
        bufferDesc2.lpwfxFormat = &wf;

        if(!SUCCEEDED(setting->direct_sound->CreateSoundBuffer(&bufferDesc2, &setting->secondary_buffer, 0))) {
            // @todo Log
        }
    }

    void direct_sound_play(DirectSoundSetting* setting)
    {
        setting->secondary_buffer->Play(0, 0, DSBPLAY_LOOPING);
        setting->playing = true;
    }

    void direct_sound_free(DirectSoundSetting* setting)
    {
        if (setting->direct_sound) {
            setting->direct_sound->Release();
        }

        if (setting->primary_buffer) {
            setting->primary_buffer->Release();
        }

        if (setting->secondary_buffer) {
            setting->secondary_buffer->Release();
        }
    }

    bool direct_sound_should_update(DirectSoundSetting* setting)
    {
        return true;
    }

    int16 *direct_sound_return_buffer(DirectSoundSetting* setting)
    {

    }

    void direct_sound_play_buffer(DirectSoundSetting* setting)
    {
        DWORD player_cursor;
        DWORD write_cursor;
        if (!SUCCEEDED(setting->secondary_buffer->GetCurrentPosition(&player_cursor, &write_cursor))) {
            // @todo Log
            return;
        }

        void *region1;
        DWORD region1_size;

        void *region2;
        DWORD region2_size;

        DWORD bytes_to_lock = (setting->sample_index * setting->sample_size) % setting->buffer_size;
        DWORD bytes_to_write = 0;

        if (bytes_to_lock == player_cursor) {
            bytes_to_write = setting->playing ? 0 : setting->buffer_size;
        } else if (bytes_to_lock > player_cursor) {
            bytes_to_write = setting->buffer_size - bytes_to_lock;
            bytes_to_write += player_cursor;
        } else {
            bytes_to_write = player_cursor - bytes_to_lock;
        }

        setting->secondary_buffer->Lock(
            bytes_to_lock, bytes_to_write,
            &region1, &region1_size,
            &region2, &region2_size,
            0
        );

        int16* sample_out = (int16 *) region1;
        DWORD region1_sample_count = region1_size / setting->sample_size;

        for (DWORD idx = 0; idx < region1_sample_count; ++idx) {
            f32 t = ((f32) 1.0f / (f32) (48000 / 256)) * 2.0f * OMS_PI;
            f32 sine_value = sinf(t);
            int16 sample_value = (int16) (sine_value * setting->volume);

            *sample_out++ = sample_value;
            *sample_out++ = sample_value;
        }

        sample_out = (int16 *) region2;
        DWORD region2_sample_count = region2_size / setting->sample_size;

        for (DWORD idx = 0; idx < region2_sample_count; ++idx) {
            f32 t = ((f32) 1.0f / (f32) (48000 / 256)) * 2.0f * OMS_PI;
            f32 sine_value = sinf(t);
            int16 sample_value = (int16) (sine_value * setting->volume);

            *sample_out++ = sample_value;
            *sample_out++ = sample_value;
        }

        setting->secondary_buffer->Unlock(region1, region1_size, region2, region2_size);
    }
}

#endif