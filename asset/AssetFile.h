/**
 * Jingga
 *
 * @copyright Jingga
 * @license    License 2.0
 * @version   1.0.0
 * @link      https://jingga.app
 */
#ifndef TOS_ASSET_FILE_H
#define TOS_ASSET_FILE_H

#include "../stdlib/Types.h"

enum AssetType {
    ASSET_TYPE_RAW_IMG,
    ASSET_TYPE_PNG,

    ASSET_TYPE_RAW_SOUND,
    ASSET_TYPE_WAV,

    ASSET_TYPE_OBJBIN,
    ASSET_TYPE_MATBIN,
    ASSET_TYPE_ANIBIN,

    ASSET_TYPE_RAW_TEXTURE, // may include bump map, normal map, ...
};

struct AssetFileTableOfContent {
    uint32 asset_id;
    AssetType asset_type;
    uint64 asset_offset;
};

struct AssetFileHeader {
    uint32 version;
    uint32 asset_count;
    AssetFileTableOfContent* table_of_content;
};

struct AssetFile {
    AssetFileHeader header;
    byte* data;
};

void asset_file_load_header(void* fp, AssetFileHeader* header) {}

void asset_file_write_header() {}

// add asset at end (automatically) or in pos
void asset_file_add_asset(void* fp, AssetFile* asset_file, byte* asset, int64 pos = -1) {}

// remove asset by id or pos
void asset_file_remove_asset(void* fp, AssetFile* asset_file, int64 id, int64 pos = -1) {}

// load the toc info of an asset by id or pos
AssetFileTableOfContent* asset_file_load_asset_toc(AssetFileHeader* header, int64 id, int64 pos = -1) {}

#endif