#ifndef TOS_UI_LANGUAGE_H
#define TOS_UI_LANGUAGE_H

#include "../stdlib/Types.h"
#include "../memory/RingMemory.h"

#if _WIN32
    #include "../platform/win32/FileUtils.cpp"
#else
    #include "../platform/linux/FileUtils.cpp"
#endif

struct Language {
    // WARNING: the actual start of data is data -= sizeof(count); see file loading below
    byte* data;

    int32 count;
    char** lang;
};

void language_from_file_txt(
    Language* language,
    byte* data
) {
    // count elements
    language->count = 1;
    int64 len = 0;

    while (data[len] != '\0') {
        if (data[len] == '\n' && data[len + 1] == '\n') {
            ++language->count;
            data[len] = '\0';
            ++len;
        }

        ++len;
    }

    language->lang = (char **) language->data;
    memcpy(language->data + language->count * sizeof(char *), data, len);

    // First element = 0
    char** pos = language->lang;
    *pos++ = (char *) data;

    for (int32 i = 1; i < len - 1; ++i) {
        if (data[i] == '\0') {
            // We have to move by 2 since every text element is separated by 2 \n
            // 1 \n is a valid char for a single text element
            // @performance This also means that we have one additional byte for
            // every text element even in the binary version.
            *pos++ = (char *) &data[i + 2];
        }
    }
}

// File layout - binary
// offsets for start of strings
// actual string data
void language_from_file(
    Language* language
) {
    byte* pos = language->data;

    // Count
    language->count = SWAP_ENDIAN_LITTLE(*((int32 *) pos));
    pos += sizeof(language->count);

    language->lang = (char **) pos;
    char** pos_lang = language->lang;

    byte* start = pos;

    // Load pointers/offsets
    for (int32 i = 0; i < language->count; ++i) {
        *pos_lang++ = (char *) (start + SWAP_ENDIAN_LITTLE(*((uint64 *) pos)));
        pos += sizeof(uint64);
    }

    // We don't have to load the actual strings, they are already in ->data due to the file reading
}

void language_to_file(
    RingMemory* ring,
    const char* path,
    Language* language
) {
    FileBody file;

    // Temporary file size for buffer
    // @todo This is a bad placeholder, The problem is we don't know how much we actually need without stepping through the elements
    //      I also don't want to add a size variable to the theme as it is useless in all other cases
    file.size = MEGABYTE * 32;

    file.content = ring_get_memory(ring, file.size, 64);
    byte* pos = file.content;

    // Count
    *((int32 *) pos) = SWAP_ENDIAN_LITTLE(language->count);
    pos += sizeof(language->count);

    byte* start = pos;

    // Save pointers
    for (int32 i = 0; i < language->count; ++i) {
       *((uint64 *) pos) = SWAP_ENDIAN_LITTLE(pos - start);
        pos += sizeof(uint64);
    }

    int64 len_total = 0;

    // Save actual strings
    int64 len;
    for (int32 i = 0; i < language->count; ++i) {
        len = strlen(language->lang[i]);
        len_total += len;
        memcpy((char *) pos, language->lang[i], len + 1);
        pos += len;
    }

    file.size = pos - file.content;
    file_write(path, &file);
}

#endif