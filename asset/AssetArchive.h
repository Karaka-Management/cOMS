/**
 * Jingga
 *
 * @copyright Jingga
 * @license    License 2.0
 * @version   1.0.0
 * @link      https://jingga.app
 */
#ifndef TOS_ASSET_ARCHIVE_H
#define TOS_ASSET_ARCHIVE_H

#include "../stdlib/Types.h"
#include "../utils/StringUtils.h"
#include "../utils/EndianUtils.h"
#include "../utils/Utils.h"
#include "../stdlib/simd/SIMD_I32.h"
#include "../memory/RingMemory.h"
#include "../memory/BufferMemory.h"
#include "AssetManagementSystem.h"

#if _WIN32
    #include <windows.h>
    #include "../platform/win32/FileUtils.cpp"
#elif __linux__
    #include "../platform/win32/FileUtils.cpp"
#endif

struct AssetArchiveElement {
    int32 type;

    int32 start;
    int32 length;

    int32 dependency_start; // actual index for asset_dependencies
    int32 dependency_count;
};

struct AssetArchiveHeader {
    int32 version;

    uint32 asset_count;
    uint32 asset_dependency_count;

    AssetArchiveElement* asset_element; // is not the owner of the data
    int32* asset_dependencies; // is not the owner of the data
};

struct AssetArchive {
    AssetArchiveHeader header;
    byte* data; // owner of the data

    FileHandler fd;
};

// Calculates how large the header memory has to be to hold all its information
int32 asset_archive_header_size(AssetArchive* archive, byte* data)
{
    data += sizeof(archive->header.version);

    int32 asset_count = SWAP_ENDIAN_LITTLE(*((uint32 *) data));
    data += sizeof(archive->header.asset_count);

    int32 asset_dependency_count = SWAP_ENDIAN_LITTLE(*((uint32 *) data));
    data += sizeof(archive->header.asset_dependency_count);

    return sizeof(archive->header.version)
        + sizeof(archive->header.asset_count)
        + sizeof(archive->header.asset_dependency_count)
        + asset_count * sizeof(AssetArchiveElement)
        + asset_dependency_count * sizeof(int32);
}

void asset_archive_header_load(AssetArchiveHeader* header, byte* data, int32 steps = 8)
{
    header->version = SWAP_ENDIAN_LITTLE(*((int32 *) data));
    data += sizeof(header->version);

    header->asset_count = SWAP_ENDIAN_LITTLE(*((uint32 *) data));
    data += sizeof(header->asset_count);

    header->asset_dependency_count = SWAP_ENDIAN_LITTLE(*((uint32 *) data));
    data += sizeof(header->asset_dependency_count);

    memcpy(header->asset_element, data, header->asset_count * sizeof(AssetArchiveElement));
    data += header->asset_count * sizeof(AssetArchiveElement);

    SWAP_ENDIAN_LITTLE_SIMD(
        (int32 *) header->asset_element,
        (int32 *) header->asset_element,
        header->asset_count * sizeof(AssetArchiveElement) / 4, // everything is 4 bytes -> super easy to swap
        steps
    );

    header->asset_dependencies = (int32 *) ((byte *) header->asset_element + header->asset_count * sizeof(AssetArchiveElement));

    memcpy(header->asset_dependencies, data, header->asset_dependency_count * sizeof(int32));
    SWAP_ENDIAN_LITTLE_SIMD(
        (int32 *) header->asset_dependencies,
        (int32 *) header->asset_dependencies,
        header->asset_count * header->asset_dependency_count, // everything is 4 bytes -> super easy to swap
        steps
    );
}

inline
AssetArchiveElement* asset_archive_element_find(const AssetArchive* archive, int32 id)
{
    return &archive->header.asset_element[id];
}

void asset_archive_load(AssetArchive* archive, const char* path, BufferMemory* buf, RingMemory* ring, int32 steps = 8)
{
    // Get file handle
    archive->fd = file_read_async_handle(path);
    if (!archive->fd) {
        return;
    }

    FileBody file;
    file.size = 64;

    // Find header size
    file.content = ring_get_memory(ring, file.size);
    file_read(archive->fd, &file, 0, file.size);
    file.size = asset_archive_header_size(archive, file.content);

    // Reserve memory for the header
    archive->data = buffer_get_memory(
        buf,
        file.size
            - sizeof(archive->header.version)
            - sizeof(archive->header.asset_count)
            - sizeof(archive->header.asset_dependency_count),
        4
    );

    // Read entire header
    file.content = ring_get_memory(ring, file.size);
    file_read(archive->fd, &file, 0, file.size);
    asset_archive_header_load(&archive->header, file.content, steps);
}

// @performance This can probably be done much faster by handling the loading of dependencies faster
void asset_archive_asset_load(const AssetArchive* archive, int32 id, AssetManagementSystem* ams_array, RingMemory* ring)
{
    AssetArchiveElement* element = &archive->header.asset_element[id];
    AssetManagementSystem* ams = element->type > 0
        ? &ams_array[element->type]
        : &ams_array[0];

    uint64 hash = hash_djb2((const char *) &id);

    // @performance I think we could optimize the ams_reserver_asset in a way so we don't have to lock it the entire time
    pthread_mutex_lock(&ams->mutex);
    // @bug this is not how this function works
    if (hashmap_get_entry(&ams->hash_map, (const char *) &id, hash)) {
        pthread_mutex_unlock(&ams->mutex);
    }

    if (element->type == 0) {
        // @bug We can't just do this, this won't work. Check if we might want to change the asset management directly to hash indices or at least int values
        Asset* asset = ams_reserve_asset(ams, (const char *) &id, ams_calculate_chunks(ams, element->length));
        asset->self = (byte *) (asset + 1);

        FileBody file = {};
        file.content = asset->self;

        // We are directly reading into the correct destination
        file_read(archive->fd, &file, element->start, element->length);
    } else {
        // We are reading into temp memory since we have to perform transformations on the data
        FileBodyAsync file = {};
        file_read_async(archive->fd, &file, element->start, element->length, ring);

        // This happens while the file system loads the data
        Asset* asset = ams_reserve_asset(ams, (const char *) &id, ams_calculate_chunks(ams, element->length));
        asset->self = (byte *) (asset + 1);

        byte* data = ring_get_memory(ring, element->length, 64);
        size_t data_size = 0;

        // @todo create platform wrapper
        GetOverlappedResult(archive->fd, &file.ov, NULL, true);
        switch (element->type) {
            case 1: {
            } break;
            default: {
            }
        }

        memcpy(asset->self, data, data_size);
    }
    pthread_mutex_unlock(&ams->mutex);

    // @performance maybe do in worker threads?
    for (int32 i = 0; i < element->dependency_count; ++i) {
        asset_archive_asset_load(archive, id, ams, ring);
    }
}

#endif