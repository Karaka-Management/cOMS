/**
 * Jingga
 *
 * @copyright Jingga
 * @license   OMS License 2.0
 * @version   1.0.0
 * @link      https://jingga.app
 */
#ifndef TOS_SOUND_XAUDIO2_H
#define TOS_SOUND_XAUDIO2_H

#include <xaudio2.h>
#include <windows.h>
#include <objbase.h>

#include "../../../stdlib/Types.h"
#include "../../../audio/AudioSetting.h"
#include "../../../log/Log.h"
#include "../../../audio/Audio.cpp"

struct XAudio2Setting {
    IXAudio2* audio_handle;
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
    CoInitialize(NULL);
    HMODULE lib = LoadLibraryExA((LPCSTR) "xaudio2_9.dll", NULL, LOAD_LIBRARY_SEARCH_SYSTEM32);
    if (!lib) {
        LOG("Xaudio2: Couldn't load xaudio2_9.dll\n", true, true);

        lib = LoadLibraryExA((LPCSTR) "xaudio2_8.dll", NULL, LOAD_LIBRARY_SEARCH_SYSTEM32);
    }

    if (!lib) {
        LOG("Xaudio2: Couldn't load xaudio2_8.dll\n", true, true);

        return;
    }

    audio_create* XAudio2Create = (audio_create *) GetProcAddress(lib, "XAudio2Create");
    if (!XAudio2Create || !SUCCEEDED(XAudio2Create(&api_setting->audio_handle, 0, XAUDIO2_DEFAULT_PROCESSOR))) {
        LOG("Xaudio2: XAudio2Create failed\n", true, true);

        return;
    }

    HRESULT hr;
    if (!SUCCEEDED(hr = api_setting->audio_handle->CreateMasteringVoice(
        &api_setting->mastering_voice,
        XAUDIO2_DEFAULT_CHANNELS,
        setting->sample_rate,
        0,
        NULL))
    ) {
        LOG("Xaudio2: CreateMasteringVoice failed\n", true, true);

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

    if (!SUCCEEDED(api_setting->audio_handle->CreateSourceVoice(&api_setting->source_voice, &wf))) {
        LOG("Xaudio2: CreateSourceVoice failed\n", true, true);

        return;
    }

    // @todo consider to remove mallocs/callocs
    setting->buffer_size = setting->sample_rate * setting->sample_size;
    setting->buffer = (int16 *) calloc(setting->sample_rate, setting->sample_size);

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
        return;
    }

    api_setting->source_voice->Start(0, XAUDIO2_COMMIT_NOW);
    setting->is_playing = true;

    if (setting->sample_index > 1) {
        setting->sample_index = 0;
    }
}

inline
void audio_stop(AudioSetting* setting, XAudio2Setting* api_setting) {
    if (!api_setting->source_voice) {
        return;
    }

    api_setting->source_voice->Stop(0, XAUDIO2_COMMIT_NOW);
    setting->is_playing = false;
}

inline
void audio_free(AudioSetting* setting, XAudio2Setting* api_setting)
{
    if (api_setting->source_voice) {
        api_setting->source_voice->DestroyVoice();
    }

    if (api_setting->mastering_voice) {
        api_setting->mastering_voice->DestroyVoice();
    }

    if (api_setting->audio_handle) {
        api_setting->audio_handle->Release();
    }

    if (api_setting->internal_buffer[0].pAudioData) {
        free((void *) api_setting->internal_buffer[0].pAudioData);
    }

    if (api_setting->internal_buffer[1].pAudioData) {
        free((void *) api_setting->internal_buffer[1].pAudioData);
    }

    if (setting->buffer) {
        free((void *) setting->buffer);
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
void audio_play_buffer(AudioSetting* setting, XAudio2Setting* api_setting) {
    if (!api_setting->source_voice || setting->sample_buffer_size == 0) {
        return;
    }

    if (!setting->is_playing) {
        audio_play(setting, api_setting);
    }

    uint32 idx = setting->sample_output % 2;

    // @performance why are we copying again, we already created our buffer, now we have to copy it again?!
    //  Ideally we should have already copied it into the correct final one, no?
    //  We should probably provide a audio_buffer_fill function, that does this -> we could remove one whole memcopy
    memcpy(
        (void *) api_setting->internal_buffer[idx].pAudioData,
        setting->buffer,
        setting->sample_buffer_size
    );

    if (!SUCCEEDED(api_setting->source_voice->SubmitSourceBuffer(&api_setting->internal_buffer[idx]))) {
        LOG("Xaudio2: SubmitSourceBuffer failed\n", true, true);

        return;
    }

    setting->sample_index += setting->sample_buffer_size / setting->sample_size;
    setting->sample_buffer_size = 0;
}

// Basically the same as audio_play_buffer but by using this we can avoid one copy
// The only reason we have audio_play_buffer is that there might be situations where this is not possible
inline
void audio_fill_play_buffer(AudioSetting* setting, uint32 to_fill, Audio* sound, XAudio2Setting* api_setting)
{
    setting->sample_buffer_size = to_fill;

    if (!api_setting->source_voice || setting->sample_buffer_size == 0) {
        return;
    }

    if (!setting->is_playing) {
        audio_play(setting, api_setting);
    }

    uint32 idx = setting->sample_output % 2;

    audio_fill_buffer(setting, to_fill, sound, (int16 *) api_setting->internal_buffer[idx].pAudioData, to_fill);

    if (!SUCCEEDED(api_setting->source_voice->SubmitSourceBuffer(&api_setting->internal_buffer[idx]))) {
        LOG("Xaudio2: SubmitSourceBuffer failed\n", true, true);

        return;
    }

    setting->sample_index += setting->sample_buffer_size / setting->sample_size;
    setting->sample_buffer_size = 0;
}

#endif
