/**
 * Jingga
 *
 * @copyright Jingga
 * @license   OMS License 2.0
 * @version   1.0.0
 * @link      https://jingga.app
 */
#ifndef TOS_AUDIO_MIXER_H
#define TOS_AUDIO_MIXER_H

#include "../stdlib/Types.h"
#include "Audio.h"
#include "AudioSetting.h"
#include "../utils/Utils.h"
#include "../utils/MathUtils.h"
#include "../memory/ChunkMemory.h"
#include "../math/matrix/MatrixFloat32.h"

#if _WIN32
    #include "../platform/win32/threading/Atomic.h"
#elif __linux__
    #include "../platform/linux/threading/Atomic.h"
#endif

#if DIRECT_SOUND
    #include "../platform/win32/audio/DirectSound.h"
#elif XAUDIO2
    #include "../platform/win32/audio/XAudio2.h"
#endif

enum AudioEffect {
    AUDIO_EFFECT_NONE,
    AUDIO_EFFECT_ECHO = 1,
    AUDIO_EFFECT_REVERB = 2,
    AUDIO_EFFECT_UNDERWATER = 4,
    AUDIO_EFFECT_CAVE = 8,
    AUDIO_EFFECT_LOWPASS = 16,
    AUDIO_EFFECT_HIGHPASS = 32,
    AUDIO_EFFECT_FLANGER = 64,
    AUDIO_EFFECT_TREMOLO = 128,
    AUDIO_EFFECT_DISTORTION = 256,
    AUDIO_EFFECT_CHORUS = 512,
    AUDIO_EFFECT_PITCH_SHIFT = 1024,
    AUDIO_EFFECT_GRANULAR_DELAY = 2048,
    AUDIO_EFFECT_FM = 4096,
    AUDIO_EFFECT_STEREO_PANNING = 8192,
    AUDIO_EFFECT_EASE_IN = 16384,
    AUDIO_EFFECT_EASE_OUT = 32768,
};

struct AudioInstance {
    int64 id;
    AudioLocationSetting origin;

    uint32 audio_size;
    byte* audio_data;

    uint32 sample_index;
};

enum AudioMixerState {
    AUDIO_MIXER_STATE_UNINITIALIZED,
    AUDIO_MIXER_STATE_INACTIVE,
    AUDIO_MIXER_STATE_SHOULD_PLAY,
    AUDIO_MIXER_STATE_ACTIVE,
};

struct AudioMixer {
    ChunkMemory audio_instances;
    AudioMixerState state_old;
    AudioMixerState state_new;

    uint64 effect;

    AudioSetting settings;
    AudioLocationSetting camera;

    #if DIRECT_SOUND
        DirectSoundSetting api_setting;
    #elif XAUDIO2
        XAudio2Setting api_setting;
    #endif

    // @todo Replace HWND with our own typedef for linux
    HWND window;

    int16* buffer_temp;

    // @todo add mutex for locking and create threaded functions
    // do we need a condition or semaphore?
};

bool audio_mixer_is_active(AudioMixer* mixer) {
    if (mixer->state_new == AUDIO_MIXER_STATE_ACTIVE
        && atomic_get((int32 *) &mixer->state_new) == AUDIO_MIXER_STATE_ACTIVE
    ) {
        return true;
    }

    AudioMixerState mixer_state;
    if ((mixer_state = (AudioMixerState) atomic_get((int32 *) &mixer->state_new)) != mixer->state_old) {
        if (mixer_state != AUDIO_MIXER_STATE_UNINITIALIZED) {
            audio_load(
                mixer->window,
                &mixer->settings,
                &mixer->api_setting
            );

            mixer_state = AUDIO_MIXER_STATE_INACTIVE;
        }

        if (mixer_state == AUDIO_MIXER_STATE_ACTIVE) {
            audio_play(&mixer->settings, &mixer->api_setting);
            mixer_state = AUDIO_MIXER_STATE_ACTIVE;
        }
    }

    return (mixer->state_old = mixer_state) == AUDIO_MIXER_STATE_ACTIVE;
}

// @todo expand AudioLocationSetting so that it also includes audio effects, repeat etc.
void audio_mixer_add(AudioMixer* mixer, int64 id, Audio* audio, AudioLocationSetting* origin)
{
    int64 index = chunk_reserve(&mixer->audio_instances, 1);
    if (index < 0) {
        return;
    }

    AudioInstance* instance = (AudioInstance *) chunk_get_element(&mixer->audio_instances, index);
    instance->id = id;
    instance->audio_size = audio->size;
    instance->audio_data = audio->data;

    if (origin) {
        memcpy(&instance->origin, origin, sizeof(AudioLocationSetting));
    }
}

void audio_mixer_add_unique(AudioMixer* mixer, int64 id, Audio* audio, AudioLocationSetting* origin)
{
    for (uint32 i = 0; i < mixer->audio_instances.count; ++i) {
        // @performance We are not really utilizing chunk memory.
        // Maybe a simple array would be better
        // Or we need to use more chunk functions / maybe even create a chunk_iterate() function?
        AudioInstance* instance = (AudioInstance *) chunk_get_element(&mixer->audio_instances, i);
        if (instance->id == id) {
            return;
        }
    }

    audio_mixer_add(mixer, id, audio, origin);
}

void audio_mixer_remove(AudioMixer* mixer, int64 id)
{
    for (uint32 i = 0; i < mixer->audio_instances.count; ++i) {
        AudioInstance* instance = (AudioInstance *) chunk_get_element(&mixer->audio_instances, i);
        if (instance->id == id) {
            instance->id = 0;
            chunk_free_element(&mixer->audio_instances, i);

            // No return, since we want to remove all instances
        }
    }
}

void apply_echo(int16* buffer, uint32 buffer_size, f32 delay, f32 feedback, int32 sample_rate) {
    int32 delay_samples = (int32) (delay * sample_rate);
    for (uint32 i = delay_samples; i < buffer_size; ++i) {
        buffer[i] += (int16) (buffer[i - delay_samples] * feedback);
    }
}

void apply_reverb(int16* buffer, uint32 buffer_size, f32 intensity) {
    intensity *= 0.5f;
    for (uint32 i = 1; i < buffer_size; ++i) {
        buffer[i] += (int16) (buffer[i - 1] * intensity); // Simple reverb with decay
    }
}

void apply_cave(int16* buffer, uint32 buffer_size, int32 sample_rate) {
    f32 echo_delay = 0.1f; // Echo delay in seconds
    f32 feedback = 0.3f;  // Echo feedback level
    apply_echo(buffer, buffer_size, echo_delay, feedback, sample_rate);
    apply_reverb(buffer, buffer_size, 0.4f); // Add mild reverb
}

void apply_underwater(int16* buffer, uint32 buffer_size) {
    for (uint32 i = 0; i < buffer_size; ++i) {
        buffer[i] = (int16) sinf(buffer[i] * 0.5f); // Dampen + distortion
    }
}

void apply_flanger(int16* buffer, uint32 buffer_size, f32 rate, f32 depth, int32 sample_rate) {
    f32 delay_samples = depth * sample_rate;
    f32 temp = OMS_TWO_PI * rate / sample_rate;

    for (uint32 i = 0; i < buffer_size; ++i) {
        int32 delay = (int32) (delay_samples * (0.5f + 0.5f * sinf(i * temp)));
        if (i >= delay) {
            buffer[i] += (int16) (buffer[i - delay] * 0.5f);
        }
    }
}

void apply_tremolo(int16* buffer, uint32 buffer_size, f32 rate, f32 depth, int32 sample_rate) {
    f32 temp = OMS_TWO_PI * rate / sample_rate;
    f32 temp2 = (1.0f - depth) + depth;

    for (uint32 i = 0; i < buffer_size; ++i) {
        f32 mod = temp2 * (0.5f + 0.5f * sinf(i * temp));
        buffer[i] = (int16) (buffer[i] * mod);
    }
}

void apply_distortion(int16* buffer, uint32 buffer_size, f32 gain) {
    for (uint32 i = 0; i < buffer_size; ++i) {
        buffer[i] = (int16) tanh(buffer[i] * gain);
    }
}

void apply_chorus(int16* buffer, uint32 buffer_size, f32 rate, f32 depth, int32 sample_rate) {
    f32 temp = OMS_TWO_PI * rate / sample_rate;

    int32 max_delay = (int32) (depth * sample_rate);
    for (uint32 i = 0; i < buffer_size; ++i) {
        int32 delay = (int32) (max_delay * (0.5f + 0.5f * sinf(i * temp)));
        if (i >= delay) {
            buffer[i] += (int16) (buffer[i - delay] * 0.5f);
        }
    }
}

void apply_pitch_shift(int16* buffer, uint32 buffer_size, f32 pitch_factor) {
    for (uint32 i = 0; i < buffer_size; ++i) {
        buffer[i] = (int16) (buffer[i] * pitch_factor);
    }
}

void apply_granular_delay(int16* buffer, uint32 buffer_size, f32 delay, f32 granularity, int32 sample_rate) {
    int32 delay_samples = (int32) (delay * sample_rate);
    int32 limit = (int32) (granularity * sample_rate);

    for (uint32 i = 0; i < buffer_size; ++i) {
        if (i % limit == 0 && i >= delay_samples) {
            buffer[i] += (int16) (buffer[i - delay_samples] * 0.6f);
        }
    }
}

void apply_frequency_modulation(int16* buffer, uint32 buffer_size, f32 mod_freq, f32 mod_depth, int32 sample_rate) {
    f32 temp = OMS_TWO_PI * mod_freq / sample_rate;
    for (uint32 i = 0; i < buffer_size; ++i) {
        buffer[i] = (int16) (buffer[i] * sinf(i * temp) * mod_depth);
    }
}

void apply_stereo_panning(int16* buffer, int32 buffer_size, f32 pan) {
    f32 left_gain = 1.0f - pan;
    f32 right_gain = pan;

    for (uint32 i = 0; i < buffer_size; ++i) {
        buffer[i] = (int16) (buffer[i] * left_gain);
        buffer[i + 1] = (int16) (buffer[i + 1] * right_gain);
    }
}

void apply_highpass(int16* buffer, uint32 buffer_size, f32 cutoff, int32 sample_rate) {
    f32 rc = 1.0f / (OMS_TWO_PI * cutoff);
    f32 dt = 1.0f / sample_rate;
    f32 alpha = rc / (rc + dt);
    f32 previous = buffer[0];
    f32 previous_output = buffer[0];

    for (uint32 i = 1; i < buffer_size; ++i) {
        f32 current = buffer[i];
        buffer[i] = (int16) (alpha * (previous_output + current - previous));
        previous = current;
        previous_output = buffer[i];
    }
}

void apply_lowpass(int16* buffer, uint32 buffer_size, f32 cutoff, int32 sample_rate) {
    f32 rc = 1.0f / (OMS_TWO_PI * cutoff);
    f32 dt = 1.0f / sample_rate;
    f32 alpha = dt / (rc + dt);
    f32 previous = buffer[0];

    for (uint32 i = 1; i < buffer_size; ++i) {
        buffer[i] = (int16) (previous + alpha * (buffer[i] - previous));
        previous = buffer[i];
    }
}

void audio_mixer_mix(AudioMixer* mixer) {
    uint32 limit = OMS_MIN(
        mixer->settings.sample_buffer_size / mixer->settings.sample_size,
        mixer->settings.buffer_size / mixer->settings.sample_size
    );

    bool has_location = !is_empty((byte *) &mixer->camera.audio_location, sizeof(mixer->camera.audio_location));

    f32 volume_scale = mixer->settings.master_volume * mixer->settings.master_volume;

    for (uint32 i = 0; i < mixer->audio_instances.count; ++i) {
        AudioInstance* sound = (AudioInstance *) chunk_get_element(&mixer->audio_instances, i);
        if (sound->id == 0) {
            continue;
        }

        // Compute the vector from the player to the sound's origin
        v3_f32 to_sound = {};
        f32 total_attenuation = 1.0f;
        bool has_origin = !is_empty((byte *) &sound->origin.audio_location, sizeof(sound->origin.audio_location));

        if (has_location && has_origin) {
            vec3_sub(&to_sound, &sound->origin.audio_location, &mixer->camera.audio_location);

            f32 distance = vec3_length(&to_sound);
            if (distance) {
                f32 distance_attenuation = OMS_MAX(0.0f, 1.0f - (distance / 50.0f));

                vec3_normalize(&to_sound);
                f32 alignment = vec3_dot(&mixer->camera.audio_lookat, &to_sound);
                f32 directional_attenuation = OMS_MAX(0.0f, alignment);

                total_attenuation = distance_attenuation * directional_attenuation;
            }
        }

        uint32 sound_sample_count = sound->audio_size / mixer->settings.sample_size;
        uint32 sound_sample_index = sound->sample_index;
        int16* audio_data = (int16 *) sound->audio_data;

        // Temporary buffer for effects processing
        // @performance If there are situations where only one file exists in the mixer that should be played we could directly write to
        // the output buffer improving the performance. Some of those mixers are: music, cinematic, ui
        // Careful, NOT voice since we will probably manually layer them according to their position?
        for (int32 j = 0; j < limit; ++j) {
            if (sound_sample_index >= sound_sample_count) {
                // @todo if repeat we need to handle part of it here, else quit

                sound_sample_index = 0;

                // @question why are we doing this?
                mixer->settings.sample_index = 0;
            }

            mixer->buffer_temp[j * 2] = (int16) (audio_data[sound_sample_index * 2] * volume_scale * total_attenuation);
            mixer->buffer_temp[j * 2 + 1] = (int16) (audio_data[sound_sample_index * 2 + 1] * volume_scale * total_attenuation);

            ++sound_sample_index;

            // @performance Some adjustments could be made right here the question is if this is faster.
            // Probably depends on how likely the adjustment is to happen.

            // @todo if end of file and no repeat -> remove from list
        }

        // @question We also have to set setting->sample_index = sound_sample_index.
        // But that currently happens in the sound api. Do we want to keep it there or move it here

        // Apply effects based on sound's effect type
        // @performance Depending on how we implement effects we could even pull them out of this loop
        // What I mean is effects could either be sound file dependent (current location correct) or mixer dependent
        if (mixer->effect) {
            if (mixer->effect & AUDIO_EFFECT_ECHO) {
                apply_echo(mixer->buffer_temp, limit, 0.2f, 0.4f, mixer->settings.sample_rate);
            }

            if (mixer->effect & AUDIO_EFFECT_REVERB) {
                apply_reverb(mixer->buffer_temp, limit, 0.3f);
            }

            if (mixer->effect & AUDIO_EFFECT_UNDERWATER) {
                apply_underwater(mixer->buffer_temp, limit);
            }

            if (mixer->effect & AUDIO_EFFECT_CAVE) {
                apply_cave(mixer->buffer_temp, limit, mixer->settings.sample_rate);
            }

            if (mixer->effect & AUDIO_EFFECT_LOWPASS) {
                apply_lowpass(mixer->buffer_temp, limit, 500.0f, mixer->settings.sample_rate); // Cutoff frequency 500
            }

            if (mixer->effect & AUDIO_EFFECT_HIGHPASS) {
                apply_highpass(mixer->buffer_temp, limit, 2000.0f, mixer->settings.sample_rate); // Cutoff frequency 2 kHz
            }

            if (mixer->effect & AUDIO_EFFECT_FLANGER) {
                apply_flanger(mixer->buffer_temp, limit, 0.25f, 0.005f, mixer->settings.sample_rate);
            }

            if (mixer->effect & AUDIO_EFFECT_TREMOLO) {
                apply_tremolo(mixer->buffer_temp, limit, 5.0f, 0.8f, mixer->settings.sample_rate);
            }

            if (mixer->effect & AUDIO_EFFECT_DISTORTION) {
                apply_distortion(mixer->buffer_temp, limit, 10.0f);
            }

            if (mixer->effect & AUDIO_EFFECT_CHORUS) {
                apply_chorus(mixer->buffer_temp, limit, 0.25f, 0.005f, mixer->settings.sample_rate);
            }

            if (mixer->effect & AUDIO_EFFECT_PITCH_SHIFT) {
                apply_pitch_shift(mixer->buffer_temp, limit, 1.2f); // Slight pitch increase
            }

            if (mixer->effect & AUDIO_EFFECT_GRANULAR_DELAY) {
                apply_granular_delay(mixer->buffer_temp, limit, 0.1f, 0.2f, mixer->settings.sample_rate);
            }

            if (mixer->effect & AUDIO_EFFECT_FM) {
                apply_frequency_modulation(mixer->buffer_temp, limit, 2.0f, 0.5f, mixer->settings.sample_rate);
            }

            if (mixer->effect & AUDIO_EFFECT_STEREO_PANNING) {
                apply_stereo_panning(mixer->buffer_temp, limit, 0.5f);
            }
        }

        // @bug the actual output "limit" could be smaller if sound files end earlier and no repeat is defined
        // In that case we would also have to adjust mixer->settings.sample_buffer_size

        // Add the processed sound to the output buffer
        for (uint32 j = 0; j < limit; j++) {
            mixer->settings.buffer[j] += mixer->buffer_temp[j];
        }
    }
}

#endif