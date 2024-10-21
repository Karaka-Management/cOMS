#ifndef TOS_UI_LANGUAGE_H
#define TOS_UI_LANGUAGE_H

#include "../stdlib/Types.h"
#include "../memory/RingMemory.h"

#if _WIN32
    #include "../platform/win32/UtilsWin32.h"
#else
    #include "../platform/linux/UtilsLinux.h"
#endif

struct Language {
    // WARNING: the actual start of data is data -= sizeof(count); see file loading below
    byte* data;

    int32 count;
    char** lang;
};

void language_from_file_txt(
    RingMemory* ring,
    const char* path,
    Language* language
) {
    FileBody file;
    file.size = file_size(path);
    file.content = ring_get_memory(ring, file.size);

    file_read(path, &file);

    // count elements
    language->count = 1;
    for (int32 i = 0; i < file.size - 1; ++i) {
        if (file.content[i] == '\n' && file.content[i + 1] == '\n') {
            ++language->count;
            file.content[i] = '\0';
            ++i;
        }
    }

    language->lang = (char **) language->data;
    memcpy(language->data + language->count * sizeof(char *), file.content, file.size);

    // First element = 0
    *language->lang = (char *) file.content;
    ++language->lang;

    for (int32 i = 1; i < file.size - 1; ++i) {
        if (file.content[i] == '\0') {
            // We have to move by 2 since every text element is separated by 2 \n
            // 1 \n is a valid char for a single text element
            // @performance This also means that we have one additional byte for
            // every text element even in the binary version.
            *language->lang = (char *) &file.content[i + 2];
            ++language->lang;
        }
    }
}

// File layout - binary
// offsets for start of strings
// actual string data
void language_from_file(
    const char* path,
    Language* language
) {
    FileBody file;
    file.content = language->data;

    file_read(path, &file);

    byte* pos = language->data;

    // Count
    language->count = SWAP_ENDIAN_LITTLE(*((int32 *) pos));
    pos += sizeof(language->count);

    language->lang = (char **) pos;

    byte* start = pos;

    // Load pointers/offsets
    for (int32 i = 0; i < language->count; ++i) {
        *language->lang = (char *) (start + SWAP_ENDIAN_LITTLE(*((uint64 *) pos)));
        ++language->lang;
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

    // Save actual strings
    for (int32 i = 0; i < language->count; ++i) {
        strcpy((char *) pos, language->lang[i]);
        pos += strlen(language->lang[i]);
    }

    file.size = pos - file.content;
    file_write(path, &file);
}

#endif