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
#include "../utils/MathUtils.h"
#include "../memory/ChunkMemory.h"
#include "../math/matrix/MatrixFloat32.h"

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
};

struct AudioMixer {
    ChunkMemory audio_instances;
    bool is_active;

    uint64 effect;

    AudioSetting settings;
    AudioLocationSetting camera;

    #if DIRECT_SOUND
        DirectSoundSetting api_setting;
    #elif XAUDIO2
        XAudio2Setting api_setting;
    #endif

    int16* buffer_temp;

    // @todo add mutex for locking and create threaded functions
    // do we need a condition or semaphore?
};

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
    for (int32 i = 0; i < mixer->audio_instances.count; ++i) {
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
    for (int32 i = 0; i < mixer->audio_instances.count; ++i) {
        AudioInstance* instance = (AudioInstance *) chunk_get_element(&mixer->audio_instances, i);
        if (instance->id == id) {
            instance->id = 0;
            chunk_free_element(&mixer->audio_instances, i);

            // No return, since we want to remove all instances
        }
    }
}

void apply_echo(int16* buffer, uint16 buffer_size, f32 delay, f32 feedback, int32 sample_rate) {
    int32 delay_samples = (int32) (delay * sample_rate);
    for (int32 i = delay_samples; i < buffer_size; ++i) {
        buffer[i] += (int16) (buffer[i - delay_samples] * feedback);
    }
}

void apply_reverb(int16* buffer, uint16 buffer_size, f32 intensity) {
    intensity *= 0.5f;
    for (int32 i = 1; i < buffer_size; ++i) {
        buffer[i] += (int16) (buffer[i - 1] * intensity); // Simple reverb with decay
    }
}

void apply_cave(int16* buffer, uint16 buffer_size, int32 sample_rate) {
    f32 echo_delay = 0.1f; // Echo delay in seconds
    f32 feedback = 0.3f;  // Echo feedback level
    apply_echo(buffer, buffer_size, echo_delay, feedback, sample_rate);
    apply_reverb(buffer, buffer_size, 0.4f); // Add mild reverb
}

void apply_underwater(int16* buffer, uint16 buffer_size) {
    for (int32 i = 0; i < buffer_size; ++i) {
        buffer[i] = (int16) sinf(buffer[i] * 0.5f); // Dampen + distortion
    }
}

void apply_flanger(int16* buffer, uint16 buffer_size, f32 rate, f32 depth, int32 sample_rate) {
    int32 delay_samples = (int32) (depth * sample_rate);
    f32 temp = OMS_TWO_PI * rate / sample_rate;

    for (int32 i = 0; i < buffer_size; ++i) {
        int32 delay = (int32) (delay_samples * (0.5f + 0.5f * sinf(i * temp)));
        if (i >= delay) {
            buffer[i] += (int16) (buffer[i - delay] * 0.5f);
        }
    }
}

void apply_tremolo(int16* buffer, uint16 buffer_size, f32 rate, f32 depth, int32 sample_rate) {
    f32 temp = OMS_TWO_PI * rate / sample_rate;
    f32 temp2 = (1.0f - depth) + depth;

    for (int32 i = 0; i < buffer_size; ++i) {
        f32 mod = temp2 * (0.5f + 0.5f * sinf(i * temp));
        buffer[i] = (int16) (buffer[i] * mod);
    }
}

void apply_distortion(int16* buffer, uint16 buffer_size, f32 gain) {
    for (int32 i = 0; i < buffer_size; ++i) {
        buffer[i] = (int16) tanh(buffer[i] * gain);
    }
}

void apply_chorus(int16* buffer, uint16 buffer_size, f32 rate, f32 depth, int32 sample_rate) {
    f32 temp = OMS_TWO_PI * rate / sample_rate;

    int32 max_delay = (int32) (depth * sample_rate);
    for (int32 i = 0; i < buffer_size; ++i) {
        int32 delay = (int32) (max_delay * (0.5f + 0.5f * sinf(i * temp)));
        if (i >= delay) {
            buffer[i] += (int16) (buffer[i - delay] * 0.5f);
        }
    }
}

void apply_pitch_shift(int16* buffer, uint16 buffer_size, f32 pitch_factor) {
    for (int32 i = 0; i < buffer_size; ++i) {
        buffer[i] = (int16) (buffer[i] * pitch_factor);
    }
}

void apply_granular_delay(int16* buffer, uint16 buffer_size, f32 delay, f32 granularity, int32 sample_rate) {
    int32 delay_samples = (int32) (delay * sample_rate);
    int32 limit = (int32) (granularity * sample_rate);

    for (int32 i = 0; i < buffer_size; ++i) {
        if (i % limit == 0 && i >= delay_samples) {
            buffer[i] += (int16) (buffer[i - delay_samples] * 0.6f);
        }
    }
}

void apply_frequency_modulation(int16* buffer, uint16 buffer_size, f32 mod_freq, f32 mod_depth, int32 sample_rate) {
    f32 temp = OMS_TWO_PI * mod_freq / sample_rate;
    for (int32 i = 0; i < buffer_size; ++i) {
        buffer[i] = (int16) (buffer[i] * sinf(i * temp) * mod_depth);
    }
}

void apply_stereo_panning(int16* buffer, int32 buffer_size, f32 pan) {
    f32 left_gain = 1.0f - pan;
    f32 right_gain = pan;

    for (int32 i = 0; i < buffer_size; ++i) {
        buffer[i] = (int16) (buffer[i] * left_gain);
        buffer[i + 1] = (int16) (buffer[i + 1] * right_gain);
    }
}

void apply_highpass(int16* buffer, uint16 buffer_size, f32 cutoff, int32 sample_rate) {
    f32 rc = 1.0f / (OMS_TWO_PI * cutoff);
    f32 dt = 1.0f / sample_rate;
    f32 alpha = rc / (rc + dt);
    f32 previous = buffer[0];
    f32 previous_output = buffer[0];

    for (int32 i = 1; i < buffer_size; ++i) {
        f32 current = buffer[i];
        buffer[i] = (int16) (alpha * (previous_output + current - previous));
        previous = current;
        previous_output = buffer[i];
    }
}


void apply_lowpass(int16* buffer, uint16 buffer_size, f32 cutoff, int32 sample_rate) {
    f32 rc = 1.0f / (OMS_TWO_PI * cutoff);
    f32 dt = 1.0f / sample_rate;
    f32 alpha = dt / (rc + dt);
    f32 previous = buffer[0];

    for (int32 i = 1; i < buffer_size; ++i) {
        buffer[i] = (int16) (previous + alpha * (buffer[i] - previous));
        previous = buffer[i];
    }
}

void audio_mixer_mix(AudioMixer *mixer) {
    uint16 limit = (uint16) (mixer->settings.sample_buffer_size / mixer->settings.sample_size);

    for (int32 i = 0; i < mixer->audio_instances.count; ++i) {
        AudioInstance* sound = (AudioInstance *) chunk_get_element(&mixer->audio_instances, i);
        if (sound->id == 0) {
            continue;
        }

        // Compute the vector from the player to the sound's origin
        v3_f32 to_sound;
        vec3_sub(&to_sound, &sound->origin.audio_location, &mixer->camera.audio_location);
        f32 distance = vec3_length(&to_sound);
        f32 distance_attenuation = OMS_MAX(0.0f, 1.0f - (distance / 50.0f));
        vec3_normalize(&to_sound);
        f32 alignment = vec3_dot(&mixer->camera.audio_lookat, &to_sound);
        f32 directional_attenuation = OMS_MAX(0.0f, alignment);
        f32 total_attenuation = distance_attenuation * directional_attenuation;

        // Temporary buffer for effects processing
        // @performance If there are situations where only one file exists in the mixer that should be played we could directly write to
        // the output buffer improving the performance. Some of those mixers are: music, cinematic, ui
        // Careful, NOT voice since we will probably manually layer them according to their position?
        for (int32 j = 0; j < limit; ++j) {
            // @todo if repeat handle here

            mixer->buffer_temp[j] = (int16) (sound->audio_data[j * 2] * mixer->settings.master_volume * total_attenuation);
            mixer->buffer_temp[j + 1] = (int16) (sound->audio_data[j * 2 + 2] * mixer->settings.master_volume * total_attenuation);

            // @performance Some adjustments could be made right here the question is if this is faster.
            // Probably depends on how likely the adjustment is to happen.
        }

        // Apply effects based on sound's effect type
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

        // Add the processed sound to the output buffer
        for (int32 j = 0; j < limit; j++) {
            mixer->settings.buffer[j] += mixer->buffer_temp[j];
        }
    }
}

#endif