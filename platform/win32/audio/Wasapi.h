/**
 * Jingga
 *
 * @copyright Jingga
 * @license   OMS License 2.0
 * @version   1.0.0
 * @link      https://jingga.app
 */
#ifndef TOS_SOUND_WASAPI_H
#define TOS_SOUND_WASAPI_H

#include <windows.h>
#include <mmdeviceapi.h>
#include <audioclient.h>
#include <endpointvolume.h>
#include <commdlg.h>
#include <stdio.h>

#include "../../../stdlib/Types.h"
#include "../../../audio/AudioSetting.h"
#include "../../../utils/MathUtils.h"
#include "../../../log/Log.h"
#include "../../../audio/Audio.cpp"

struct WasapiSetting {
    IAudioClient* audio_handle;
    IAudioRenderClient* render_client;
};

// BEGIN: Dynamically load DirectSound
typedef HRESULT WINAPI CoInitializeEx_t(LPVOID, DWORD);
typedef HRESULT WINAPI CoCreateInstance_t(REFCLSID, LPUNKNOWN, DWORD, REFIID, LPVOID*);

typedef HRESULT WINAPI IMMDeviceEnumerator_GetDefaultAudioEndpoint_t(IMMDeviceEnumerator*, EDataFlow, ERole, IMMDevice**);
typedef HRESULT WINAPI IMMDevice_Activate_t(IMMDevice*, REFIID, DWORD, PROPVARIANT*, void**);

typedef HRESULT WINAPI IAudioClient_GetMixFormat_t(IAudioClient*, WAVEFORMATEX**);
typedef HRESULT WINAPI IAudioClient_Initialize_t(IAudioClient*, AUDCLNT_SHAREMODE, DWORD, REFERENCE_TIME, REFERENCE_TIME, WAVEFORMATEX*, void*);
typedef HRESULT WINAPI IAudioClient_Start_t(IAudioClient*);
typedef HRESULT WINAPI IAudioClient_Stop_t(IAudioClient*);
typedef HRESULT WINAPI IAudioClient_GetService_t(IAudioClient*, REFIID, void**);
// END: Dynamically load DirectSound

void audio_load(HWND hwnd, AudioSetting* setting, WasapiSetting* api_setting) {
    HMODULE ole32 = LoadLibraryExA((LPCSTR) "ole32.dll", NULL, LOAD_LIBRARY_SEARCH_SYSTEM32);
    if (!ole32) {
        LOG("Wasapi: Couldn't load ole32.dll\n", true, true);

        return;
    }

    CoInitializeEx_t* co_initialize_ex = (CoInitializeEx_t *) GetProcAddress(ole32, "CoInitializeEx");
    CoCreateInstance_t* co_create_instance = (CoCreateInstance_t *) GetProcAddress(ole32, "CoCreateInstance");

    if (!co_initialize_ex || !co_create_instance) {
        LOG("Wasapi: ole32 function binding failed\n", true, true);

        return;
    }

    HMODULE mmdevapi = LoadLibraryExA((LPCSTR) "mmdevapi.dll", NULL, LOAD_LIBRARY_SEARCH_SYSTEM32);
    if (!mmdevapi) {
        LOG("Wasapi: Couldn't load mmdevapi.dll\n", true, true);

        return;
    }

    IMMDeviceEnumerator_GetDefaultAudioEndpoint_t* IMMDeviceEnumerator_GetDefaultAudioEndpoint = (IMMDeviceEnumerator_GetDefaultAudioEndpoint_t *) GetProcAddress(mmdevapi, "IMMDeviceEnumerator_GetDefaultAudioEndpoint");
    IMMDevice_Activate_t* IMMDevice_Activate = (IMMDevice_Activate_t *) GetProcAddress(mmdevapi, "IMMDevice_Activate");

    if (!IMMDeviceEnumerator_GetDefaultAudioEndpoint || !IMMDevice_Activate) {
        LOG("Wasapi: mmdevapi function binding failed\n", true, true);

        return;
    }

    HMODULE audioclient = LoadLibraryExA((LPCSTR) "audioclient.dll", NULL, LOAD_LIBRARY_SEARCH_SYSTEM32);
    if (!audioclient) {
        LOG("Wasapi: Couldn't load audioclient.dll\n", true, true);

        return;
    }

    IAudioClient_GetMixFormat_t* pIAudioClient_GetMixFormat = (IAudioClient_GetMixFormat_t *) GetProcAddress(audioclient, "IAudioClient_GetMixFormat");
    IAudioClient_Initialize_t* pIAudioClient_Initialize = (IAudioClient_Initialize_t *) GetProcAddress(audioclient, "IAudioClient_Initialize");
    IAudioClient_Start_t* pIAudioClient_Start = (IAudioClient_Start_t *) GetProcAddress(audioclient, "IAudioClient_Start");
    IAudioClient_Stop_t* pIAudioClient_Stop = (IAudioClient_Stop_t *) GetProcAddress(audioclient, "IAudioClient_Stop");
    IAudioClient_GetService_t* pIAudioClient_GetService = (IAudioClient_GetService_t *) GetProcAddress(audioclient, "IAudioClient_GetService");

    if (!pIAudioClient_GetMixFormat || !pIAudioClient_Initialize || !pIAudioClient_Start || !pIAudioClient_Stop || !pIAudioClient_GetService) {
        LOG("Wasapi: audioclient function binding failed\n", true, true);

        return;
    }

    HRESULT hr = co_initialize_ex(NULL, COINIT_MULTITHREADED);
    if (FAILED(hr)) {
        LOG("Wasapi: Wasapi initialize failed\n", true, true);

        return;
    }

    IMMDeviceEnumerator* enumerator;
    IMMDevice* device;

    hr  = co_create_instance(CLSID_MMDeviceEnumerator, NULL, CLSCTX_ALL, IID_IMMDeviceEnumerator, (void **) &enumerator);
    if (FAILED(hr)) {
        LOG("Wasapi: Wasapi CreateInstance failed\n", true, true);

        return;
    }

    hr  = IMMDeviceEnumerator_GetDefaultAudioEndpoint(enumerator, eRender, eConsole, &device);
    if (FAILED(hr)) {
        LOG("Wasapi: Wasapi DefaultAudioEndpoint failed\n", true, true);

        enumerator->Release();

        return;
    }

    hr = IMMDevice_Activate(device, IID_IAudioClient, CLSCTX_ALL, NULL, (void **) &api_setting->audio_handle);
    if (FAILED(hr)) {
        LOG("Wasapi: Wasapi DeviceActivate failed\n", true, true);

        device->Release();
        enumerator->Release();

        return;
    }

    device->Release();
    enumerator->Release();

    // Initializing the audio client
    WAVEFORMATEX *pwfx = NULL;
    api_setting->audio_handle->GetMixFormat(&pwfx);
    api_setting->audio_handle->Initialize(AUDCLNT_SHAREMODE_SHARED, AUDCLNT_STREAMFLAGS_EVENTCALLBACK, 0, 0, pwfx, NULL);
    api_setting->audio_handle->GetService(IID_IAudioRenderClient, (void **) &api_setting->render_client);
}

inline
void audio_play(AudioSetting* setting, WasapiSetting* api_setting) {
    if (!api_setting->audio_handle) {
        return;
    }

    api_setting->audio_handle->Start();
}

inline
void audio_stop(AudioSetting* setting, WasapiSetting* api_setting) {
    if (!api_setting->audio_handle) {
        return;
    }

    api_setting->audio_handle->Stop();
}

inline
void audio_free(AudioSetting* setting, WasapiSetting* api_setting)
{
    if (!api_setting->render_client) {
        api_setting->render_client->Release();
    }

    if (!api_setting->audio_handle) {
        api_setting->audio_handle->Release();
    }
}

inline
uint32 audio_buffer_fillable(const AudioSetting* setting, const WasapiSetting* api_setting)
{
    if (!api_setting->audio_handle) {
        return 0;
    }

    uint32 buffer_frame_count;
    api_setting->audio_handle->GetBufferSize(&buffer_frame_count);

    uint32 frames_padding;
    api_setting->audio_handle->GetCurrentPadding(&frames_padding);

    return buffer_frame_count - frames_padding;
}

inline
void audio_play_buffer(AudioSetting* setting, WasapiSetting* api_setting) {
    if (!api_setting->audio_handle || setting->sample_buffer_size == 0) {
        return;
    }

    // @question Do we have to change it from sample_buffer_size to sample count?
    byte* buffer;
    api_setting->render_client->GetBuffer(setting->sample_buffer_size, (byte **) &buffer);

    memcpy(buffer, setting->buffer, setting->sample_buffer_size);
}

#endif