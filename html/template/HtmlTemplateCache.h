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
#include "../../stdlib/PerfectHashMap.h"
#include "../../system/FileUtils.cpp"
#include "../../html/template/HtmlTemplateInterpreter.h"

bool html_template_in_control_structure(const char* str, const char** controls, int32 control_length) {
    for (int32 i = 0; i < control_length; ++i) {
        if (str_compare(controls[i], str) == 0) {
            return true;
        }
    }

    return false;
}

// @performance This combines load and build, that should be two separate functions
// Data layout:
//      1. minified text file
//      2. AST
void html_template_cache_load(PerfectHashMapRef* cache, const char* key, const char* str, int32 alignment = 64) {
    char* minified = (char *) ROUND_TO_NEAREST((uintptr_t) cache->data + (uintptr_t) cache->data_pos, alignment);
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
        // @performance What about optional tags such as </li>, </br>, </td>, </tr>, </option>, ...
        // @performance Remove comments /* */ and //
        if (!in_control_structure && str_is_eol(*str)) {
            str_skip_eol(&str);
            //continue; // @question Why does this fail?
        } else if (!in_control_structure && str_is_empty(*str)) {
            str_skip_empty(&str);
            // @performance This keeps <tag> </tag> whitespaces, which we don't want and could optimize away
            // We would have to check the previous char and the next char to be != > and != <
            // the problem however is that we would have to specially handle the first char and last char in str
            --str;
        }

        if (!(*str)) {
            break;
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

    cache->data_pos += ((uintptr_t) minified - (uintptr_t) minified_start);

    // Now add AST to cache
    HtmlTemplateToken current_token = html_template_token_next((const char**) &minified_start, HTML_TEMPLATE_CONTEXT_FLAG_HTML);

    HtmlTemplateContextStack context_stack = {};
    HtmlTemplateContext initial_context = {};
    pushContext(&context_stack, initial_context);

    // @todo Instead of doing this, we want to use the cache.memory
    // For this to work we need to pass the current memory position however into this function
    byte* memory_start = cache->data + cache->data_pos;
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

    cache->data_pos += ((uintptr_t) memory - (uintptr_t) memory_start);

    ASSERT_SIMPLE(ast);
    ASSERT_SIMPLE(((uintptr_t) ast) % alignment == 0);
    // We only store the AST index in the hash map
    perfect_hashmap_insert(&cache->hm, key, (int32) ((uintptr_t) ast - (uintptr_t) cache->data));
}

void html_template_cache_iter(const char* path, va_list args) {
    PerfectHashMapRef* cache = va_arg(args, PerfectHashMapRef*);
    RingMemory* ring = va_arg(args, RingMemory*);

    char full_path[MAX_PATH];
    relative_to_absolute(path, full_path);

    FileBody file = {};
    file_read(full_path, &file, ring);

    html_template_cache_load(cache, path, (const char *) file.content);
}

void raw_file_cache_iter(const char* path, va_list args) {
    PerfectHashMapRef* cache = va_arg(args, PerfectHashMapRef*);
    RingMemory* ring = va_arg(args, RingMemory*);

    char full_path[MAX_PATH];
    relative_to_absolute(path, full_path);

    FileBody file = {};
    file_read(full_path, &file, ring);

    perfect_hashmap_insert(cache, path, file.content, file.size);
}

HtmlTemplateASTNode* html_template_cache_get(const PerfectHashMapRef* cache, const char* key)
{
    return (HtmlTemplateASTNode *) perfect_hashmap_get_value(cache, key);
}

#endif