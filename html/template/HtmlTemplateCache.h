/**
 * Jingga
 *
 * @copyright Jingga
 * @license   OMS License 2.0
 * @version   1.0.0
 * @link      https://jingga.app
 */
#ifndef COMS_HTML_TEMPLATE_CACHE_H
#define COMS_HTML_TEMPLATE_CACHE_H

#include "../../stdlib/Types.h"
#include "../../stdlib/PerfectHashMap.h"
#include "../../memory/RingMemory.h"
#include "../../system/FileUtils.cpp"
#include "../../html/template/HtmlTemplateInterpreter.h"

struct HtmlTemplateCache {
    // Contains the offsets into the cache
    PerfectHashMap hm;

    // the layout of the cache is as follows:
    //      * Perfect hash map memory which contains the offsets into this cache where the root AST node per template can be found (hash_entries)
    //      * Per template memory:
    //          * minified template string (64 byte aligned)
    //          * AST, with it's own values or alternatively a pointer into the template string depending on the data (32 byte aligned for EVERY AST node)
    byte* cache;

    // Total cache size
    uint32 cache_size;

    // Current position
    uint32 cache_pos;
};

static
void html_template_find(const char* path, va_list args) {
    char** paths = va_arg(args, char**);
    uint32* path_count = va_arg(args, uint32*);
    uint32* max_path_count = va_arg(args, uint32*);
    uint32* total_file_size = va_arg(args, uint32*);
    RingMemory* ring = va_arg(args, RingMemory*);

    if (path_count == max_path_count) {
        uint32 old_max_path_count = *max_path_count;

        *max_path_count += 1000;
        char* new_paths = (char *) ring_get_memory(ring, (*max_path_count) * 256 * sizeof(char), 8, true);
        memcpy(new_paths, *paths, old_max_path_count * 256 * sizeof(char));
        *paths = new_paths;
    }

    *total_file_size += file_size(path);
    str_copy_short(paths[*path_count], path, 256);
    ++(*path_count);
}

void html_template_cache_init(HtmlTemplateCache* cache, const char* basedir, BufferMemory* buf, RingMemory* ring) {
    uint32 max_path_count = 1000;
    uint32 path_count = 0;
    char* paths = (char *) ring_get_memory(ring, max_path_count * 256 * sizeof(char), 8, true);
    uint32 total_file_size = 0;

    iterate_directory(basedir, ".tpl.html", html_template_find, &paths, &path_count, &max_path_count, &total_file_size, ring);
    cache->cache_size = (uint64) (total_file_size * 1.2);
    cache->cache = (byte *) buffer_get_memory(buf, cache->cache_size, 64, true);

    perfect_hashmap_create(&cache->hm, path_count, sizeof(PerfectHashEntryInt32), buf);
    perfect_hashmap_prepare(&cache->hm, (const char*) paths, path_count, 256, 10000, ring);

    LOG_1("Created HtmlTemplateCache with %n B for %n templates with %n B in uncompressed file size", {{LOG_DATA_INT64, &cache->cache_size}, {LOG_DATA_INT32, &path_count}, {LOG_DATA_INT32, &total_file_size}});
}

bool html_template_in_control_structure(const char* str, const char** controls, int32 control_length) {
    for (int32 i = 0; i < control_length; ++i) {
        if (str_compare(controls[i], str) == 0) {
            return true;
        }
    }

    return false;
}

void html_template_cache_load(HtmlTemplateCache* cache, const char* key, const char* str) {
    char* minified = (char *) ROUND_TO_NEAREST((uintptr_t) cache->cache + (uintptr_t) cache->cache_pos, 64);
    char* minified_start = minified;

    static const char* CONTROL_STRUCTURE_START[] = {
        "'", "\"", "<code", "<pre", "<input", "<textarea",
    };

    static const char* CONTROL_STRUCTURE_END[] = {
        "'", "\"", "</code>", "</pre>", "/>", "</textarea>",
    };

    // Remove empty content to reduce template size
    // We could handle this inside the lexer but the lexer itself often uses pointers into the template
    // The lexer would also have to create additional tokens in that case whenever it "splits" the template due to empty text
    // All-in-all let's consider this a pre-pass that we might want to move to the lexer in the future but I don't think so
    int32 in_control_structure = 0;
    while (*str) {
        if (!in_control_structure && str_is_eol(*str)) {
            str_skip_eol(&str);
        } else if (!in_control_structure && str_is_empty(*str)) {
            // @performance This keeps <tag> </tag> whitespaces, which we don't want and could optimize away
            str_skip_empty(&str);
            --str;
        }

        if (!in_control_structure
            && html_template_in_control_structure(str, CONTROL_STRUCTURE_START, ARRAY_COUNT(CONTROL_STRUCTURE_START))
        ) {
            ++in_control_structure;
        } else if (in_control_structure
            && html_template_in_control_structure(str, CONTROL_STRUCTURE_END, ARRAY_COUNT(CONTROL_STRUCTURE_END))
        ) {
            --in_control_structure;
        }

        *minified++ = *str++;
    }

    cache->cache_pos += ((uintptr_t) minified - (uintptr_t) minified_start);

    HtmlTemplateToken current_token = html_template_token_next((const char**) &minified_start, HTML_TEMPLATE_CONTEXT_FLAG_HTML);

    HtmlTemplateContextStack context_stack = {};
    HtmlTemplateContext initial_context = {};
    pushContext(&context_stack, initial_context);

    // @todo Instead of doing this, we want to use the cache.memory
    // For this to work we need to pass the current memory position however into this function
    byte* memory_start = cache->cache + cache->cache_pos;
    byte* memory = memory_start;
    HtmlTemplateASTNode* ast = html_template_statement_parse(
        (const char**) &minified_start,
        &current_token,
        &context_stack,
        current_token.type == TOKEN_CODE_START
            ? HTML_TEMPLATE_CONTEXT_FLAG_TEMPLATE
            : HTML_TEMPLATE_CONTEXT_FLAG_HTML,
        &memory
    );

    cache->cache_pos += ((uintptr_t) memory - (uintptr_t) memory_start);

    ASSERT_SIMPLE(((uintptr_t) ast) % 64 == 0);
    perfect_hashmap_insert(&cache->hm, key, (int32) ((uintptr_t) ast - (uintptr_t) cache->cache));
}

static
void html_template_cache_iter(const char* path, va_list args) {
    HtmlTemplateCache* cache = va_arg(args, HtmlTemplateCache*);
    RingMemory* ring = va_arg(args, RingMemory*);

    char full_path[MAX_PATH];
    relative_to_absolute(path, full_path);

    FileBody file = {};
    file_read(full_path, &file, ring);

    html_template_cache_load(cache, path, (const char *) file.content);
}

void html_template_cache_load_all(HtmlTemplateCache* cache, const char* basedir, RingMemory* ring) {
    iterate_directory(basedir, ".tpl.html", html_template_cache_iter, cache, ring);
    LOG_1("Loaded all html templates with %n in cache size", {{LOG_DATA_INT32, &cache->cache_pos}});
}

HtmlTemplateASTNode* html_template_cache_get(HtmlTemplateCache* cache, const char* key)
{
    PerfectHashEntryInt32* entry = (PerfectHashEntryInt32 *) perfect_hashmap_get_entry(&cache->hm, key);
    if (!entry) {
        return NULL;
    }

    return (HtmlTemplateASTNode *) (cache->cache + entry->value);
}

#endif