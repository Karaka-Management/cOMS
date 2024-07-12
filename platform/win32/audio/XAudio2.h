/**
 * Jingga
 *
 * @package   Stdlib
 * @copyright Jingga
 * @license   OMS License 2.0
 * @version   1.0.0
 * @link      https://jingga.app
 */
#ifndef TOS_SOUND_XAUDIO2_H
#define TOS_SOUND_XAUDIO2_H

#include <xaudio2.h>
#include <windows.h>

#include "../../../stdlib/Types.h"
#include "../../../audio/AudioSetting.h"
#include "../../../utils/MathUtils.h"

struct XAudio2Setting {
    IXAudio2* xaudio2;
    IXAudio2SourceVoice* source_voice;
    IXAudio2MasteringVoice* mastering_voice;

    XAUDIO2_BUFFER internal_buffer[2];
};

// BEGIN: Dynamically load XAudio2
typedef HRESULT WINAPI audio_create(IXAudio2**, UINT32, XAUDIO2_PROCESSOR);
HRESULT WINAPI XAudio2CreateStub(IXAudio2**, UINT32, XAUDIO2_PROCESSOR) {
    return 0;
}
// END: Dynamically load XAudio2

void audio_load(HWND hwnd, AudioSetting* setting, XAudio2Setting* api_setting) {
    HMODULE lib = LoadLibraryExA((LPCSTR) "xaudio2_9.dll", NULL, LOAD_LIBRARY_SEARCH_SYSTEM32);
    if (!lib) {
            // @todo Log
        lib = LoadLibraryExA((LPCSTR) "xaudio2_8.dll", NULL, LOAD_LIBRARY_SEARCH_SYSTEM32);
    }

    if (!lib) {
        // @todo Log
        return;
    }

    audio_create* XAudio2Create = (audio_create *) GetProcAddress(lib, "XAudio2Create");
    if (!XAudio2Create || !SUCCEEDED(XAudio2Create(&api_setting->xaudio2, 0, XAUDIO2_DEFAULT_PROCESSOR))) {
        // @todo Log
        return;
    }

    if (!SUCCEEDED(api_setting->xaudio2->CreateMasteringVoice(
        &api_setting->mastering_voice,
        XAUDIO2_DEFAULT_CHANNELS,
        setting->sample_rate,
        0,
        NULL
    ))) {
        // @todo Log
        return;
    }

    WAVEFORMATEX wf = {};
    wf.wFormatTag = WAVE_FORMAT_PCM;
    wf.nChannels = 2;
    wf.wBitsPerSample = (uint16) ((setting->sample_size * 8) / wf.nChannels); // = sample_size per channel
    wf.nBlockAlign = (wf.nChannels * wf.wBitsPerSample) / 8; // = sample_szie
    wf.nSamplesPerSec = setting->sample_rate;
    wf.nAvgBytesPerSec = wf.nSamplesPerSec * wf.nBlockAlign; // = buffer_size
    wf.cbSize = 0;

    if (!SUCCEEDED(api_setting->xaudio2->CreateSourceVoice(&api_setting->source_voice, &wf))) {
        // @todo Log
        return;
    }

    setting->buffer_size = setting->sample_rate * setting->sample_size;
    setting->buffer = (int16 *) calloc(setting->sample_rate, setting->sample_size);

    // @question Consider to move to the heap?
    api_setting->internal_buffer[0].Flags = 0;
    api_setting->internal_buffer[0].AudioBytes = setting->buffer_size;
    api_setting->internal_buffer[0].pAudioData = (byte *) malloc(setting->buffer_size * sizeof(byte));
    api_setting->internal_buffer[0].PlayBegin = 0;
    api_setting->internal_buffer[0].PlayLength = 0;
    api_setting->internal_buffer[0].LoopBegin = 0;
    api_setting->internal_buffer[0].LoopLength = 0;
    api_setting->internal_buffer[0].LoopCount = 0;
    api_setting->internal_buffer[0].pContext = NULL;

    api_setting->internal_buffer[1].Flags = 0;
    api_setting->internal_buffer[1].AudioBytes = setting->buffer_size;
    api_setting->internal_buffer[1].pAudioData = (byte *) malloc(setting->buffer_size * sizeof(byte));
    api_setting->internal_buffer[1].PlayBegin = 0;
    api_setting->internal_buffer[1].PlayLength = 0;
    api_setting->internal_buffer[1].LoopBegin = 0;
    api_setting->internal_buffer[1].LoopLength = 0;
    api_setting->internal_buffer[1].LoopCount = 0;
    api_setting->internal_buffer[1].pContext = NULL;

    setting->sample_index = 0;
}

inline
void audio_play(AudioSetting* setting, XAudio2Setting* api_setting) {
    if (!api_setting->source_voice) {
        // @todo Log

        return;
    }

    api_setting->source_voice->Start(0, XAUDIO2_COMMIT_NOW);
    setting->is_playing = true;
}

inline
void audio_free(AudioSetting* setting, XAudio2Setting* api_setting)
{
    if (api_setting->internal_buffer[0].pAudioData) {
        free((void *) api_setting->internal_buffer[0].pAudioData);
    }

    if (api_setting->internal_buffer[1].pAudioData) {
        free((void *) api_setting->internal_buffer[1].pAudioData);
    }

    if (setting->buffer) {
        free((void *) setting->buffer);
    }

    if (api_setting->source_voice) {
        api_setting->source_voice->DestroyVoice();
    }

    if (api_setting->mastering_voice) {
        api_setting->mastering_voice->DestroyVoice();
    }

    if (api_setting->xaudio2) {
        api_setting->xaudio2->Release();
    }
}

/**
 * Calculates the samples to generate for the buffer
 *
 * For XAudio2 we currently always fill the entire buffer size.
 * For other audio APIs we maybe have to do something else
 */
inline
uint32 audio_buffer_fillable(const AudioSetting* setting, const XAudio2Setting* api_setting)
{
    if (!api_setting->source_voice) {
        // @todo Log

        return 0;
    }

    XAUDIO2_VOICE_STATE state;
    api_setting->source_voice->GetState(&state);
    if (state.BuffersQueued > 1) {
        return 0;
    }

    return setting->buffer_size;
}

inline
void audio_play_buffer(AudioSetting* setting, XAudio2Setting* api_setting, uint32 bytes_to_write) {
    if (!api_setting->source_voice) {
        // @todo Log

        return;
    }

    if (bytes_to_write == 0) {
        return;
    }

    memcpy(
        (void *) api_setting->internal_buffer[setting->sample_index].pAudioData,
        setting->buffer,
        bytes_to_write
    );

    if (!SUCCEEDED(api_setting->source_voice->SubmitSourceBuffer(&api_setting->internal_buffer[setting->sample_index]))) {
        // @todo Log
        return;
    }

    setting->sample_index = (setting->sample_index + 1) % 2;
    setting->sample_buffer_size = 0;
}

#endif
