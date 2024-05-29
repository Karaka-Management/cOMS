/**
 * Karaka
 *
 * @package   Stdlib
 * @copyright Dennis Eichhorn
 * @license   OMS License 1.0
 * @version   1.0.0
 * @link      https://jingga.app
 */
#ifndef SOUND_XAUDIO2_H
#define SOUND_XAUDIO2_H

#include <xaudio2.h>
#include <windows.h>
#include <math.h>

#include "../Stdlib/Types.h"
#include "../Utils/MathUtils.h"
#include "../UI/Window.h"

#define SOUND_API_XAUDIO2 1

namespace Sound {
    struct XAudio2Setting {
        uint32 sample_rate;
        uint32 sample_size;
        uint32 sample_index;
        int16 volume;
        uint32 buffer_size;

        bool playing = false;
        byte type = SOUND_API_XAUDIO2;

        // Api specific data from here on
        IXAudio2* xaudio2;
        IXAudio2SourceVoice* source_voice;
        IXAudio2MasteringVoice* mastering_voice;

        XAUDIO2_BUFFER buffer[2];
    };

    // BEGIN: Dynamically load XAudio2
    typedef HRESULT WINAPI xaudio2_create(IXAudio2**, UINT32, XAUDIO2_PROCESSOR);
    HRESULT WINAPI XAudio2CreateStub(IXAudio2**, UINT32, XAUDIO2_PROCESSOR) {
        return 0;
    }
    // END: Dynamically load XAudio2

    void xaudio2_load(XAudio2Setting* setting) {
        HMODULE lib = LoadLibraryExA((LPCSTR) "xaudio2_9.dll", NULL, LOAD_LIBRARY_SEARCH_SYSTEM32);
        if (!lib) {
             // @todo Log
            lib = LoadLibraryExA((LPCSTR) "xaudio2_8.dll", NULL, LOAD_LIBRARY_SEARCH_SYSTEM32);
        }

        if (!lib) {
            // @todo Log
            return;
        }

        xaudio2_create* XAudio2Create = (xaudio2_create *) GetProcAddress(lib, "XAudio2Create");
        if (!XAudio2Create || !SUCCEEDED(XAudio2Create(&setting->xaudio2, 0, XAUDIO2_DEFAULT_PROCESSOR))) {
            // @todo Log
            return;
        }

        if (!SUCCEEDED(setting->xaudio2->CreateMasteringVoice(&setting->mastering_voice))) {
            // @todo Log
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

        if (!SUCCEEDED(setting->xaudio2->CreateSourceVoice(&setting->source_voice, &wf))) {
            // @todo Log
            return;
        }

        setting->buffer_size = setting->sample_rate * wf.nBlockAlign;

        // @question Consider to move to the heap?
        setting->buffer[0].Flags = 0;
        setting->buffer[0].AudioBytes = setting->buffer_size;
        setting->buffer[0].pAudioData = (BYTE *) malloc(setting->buffer_size * sizeof(BYTE));
        setting->buffer[0].PlayBegin = 0;
        setting->buffer[0].PlayLength = 0;
        setting->buffer[0].LoopBegin = 0;
        setting->buffer[0].LoopLength = 0;
        setting->buffer[0].LoopCount = 0;
        setting->buffer[0].pContext = NULL;

        setting->buffer[1].Flags = 0;
        setting->buffer[1].AudioBytes = setting->buffer_size;
        setting->buffer[1].pAudioData = (BYTE *) malloc(setting->buffer_size * sizeof(BYTE));
        setting->buffer[1].PlayBegin = 0;
        setting->buffer[1].PlayLength = 0;
        setting->buffer[1].LoopBegin = 0;
        setting->buffer[1].LoopLength = 0;
        setting->buffer[1].LoopCount = 0;
        setting->buffer[1].pContext = NULL;

        setting->sample_index = 0;
    }

    void xaudio2_play(XAudio2Setting* setting) {
        if (!setting->source_voice) {
            // @todo Log

            return;
        }

        setting->source_voice->Start(0, XAUDIO2_COMMIT_NOW);
        setting->playing = true;
    }

    void xaudio2_free(XAudio2Setting* setting)
    {
        if (setting->buffer[0].pAudioData) {
            free((void *) setting->buffer[0].pAudioData);
        }

        if (setting->buffer[1].pAudioData) {
            free((void *) setting->buffer[1].pAudioData);
        }

        if (setting->source_voice) {
            setting->source_voice->DestroyVoice();
        }

        if (setting->mastering_voice) {
            setting->mastering_voice->DestroyVoice();
        }

        if (setting->xaudio2) {
            setting->xaudio2->Release();
        }
    }

    bool xaudio2_should_update(XAudio2Setting* setting)
    {
        if (!setting->source_voice) {
            // @todo Log

            return false;
        }

        XAUDIO2_VOICE_STATE state;
        setting->source_voice->GetState(&state);
        if (state.BuffersQueued > 1) {
            return false;
        }

        return true;
    }

    int16* xaudio2_return_buffer(XAudio2Setting* setting)
    {
        return (int16 *) setting->buffer[setting->sample_index].pAudioData;
    }

    int16* xaudio2_fill_buffer(XAudio2Setting* setting, int16* buffer)
    {
        int16* sample_out = (int16 *) setting->buffer[setting->sample_index].pAudioData;
    }

    void xaudio2_play_buffer(XAudio2Setting* setting) {
        if (!setting->source_voice) {
            // @todo Log

            return;
        }

        setting->sample_index = (setting->sample_index + 1) % 2;

        if (!SUCCEEDED(setting->source_voice->SubmitSourceBuffer(&setting->buffer[setting->sample_index]))) {
            // @todo Log
            return;
        }
    }
}

#endif
