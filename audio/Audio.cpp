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
#include "Wav.h"

void audio_from_file(RingMemory* ring, const char* path, Audio* audio)
{
    FileBody file;
    file_read(path, &file, ring);

    if (str_ends_with(path, ".wav")) {
        wav_audio_generate(&file, audio);
    }
}

#endif
