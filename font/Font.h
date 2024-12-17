#ifndef TOS_FONT_H
#define TOS_FONT_H

#include "../stdlib/Types.h"
#include "../memory/BufferMemory.h"
#include "../utils/EndianUtils.h"
#include "../utils/Utils.h"

#if __aarch64__
    #include "../stdlib/sve/SVE_I32.h"
#else
    #include "../stdlib/simd/SIMD_I32.h"
#endif

#if _WIN32
    #include "../platform/win32/FileUtils.cpp"
#else
    #include "../platform/linux/FileUtils.cpp"
#endif

struct GlyphMetrics {
    f32 width;     // Width of the glyph
    f32 height;    // Height of the glyph
    f32 offset_x;  // Horizontal offset from baseline
    f32 offset_y;  // Vertical offset from baseline
    f32 advance_x; // Horizontal advance after drawing the glyph
};

struct GlyphTextureCoords {
    f32 x1;
    f32 y1;
    f32 x2;
    f32 y2;
};

#define GLYPH_SIZE 40
struct Glyph {
    uint32 codepoint;
    GlyphMetrics metrics;
    GlyphTextureCoords coords;
};

struct Font {
    uint32 glyph_count;
    char texture_name[32];
    f32 size;              // Default font size at which the font renders best
    f32 line_height;       // How tall is a single line (mostly important for multiple lines)

    // WARNING: Glyphs MUST be sorted ascending based on codepoint
    Glyph* glyphs;

};

inline
void font_init(Font* font, byte* data, int count)
{
    font->glyphs = (Glyph *) data;
    font->glyph_count = count;
}

inline
Glyph* font_glyph_find(Font* font, uint32 codepoint)
{
    for (uint32 i = 0; i < font->glyph_count; ++i) {
        if (font->glyphs[i].codepoint == codepoint) {
            return &font->glyphs[i];
        }
    }

    return NULL;
}

void font_from_file_txt(
    Font* font,
    const char* path,
    RingMemory* ring
)
{
    FileBody file;
    file_read(path, &file, ring);
    ASSERT_SIMPLE(file.size);

    char* pos = (char *) file.content;

    bool start = true;
    char block_name[32];

    int32 glyph_index = 0;

    int32 image_width = 0;
    int32 image_height = 0;

    char* texture_pos = font->texture_name;

    while (*pos != '\0') {
        if (start) {
            // Parsing general data
            int32 i = 0;
            while (*pos != '\0' && *pos != ' ' && *pos != ':' && *pos != '\n' && i < 31) {
                block_name[i] = *pos;
                ++pos;
                ++i;
            }

            block_name[i] = '\0';

            // Go to value
            while (*pos == ' ' || *pos == '\t' || *pos == ':') {
                ++pos;
            }

            if (strcmp(block_name, "texture") == 0) {
                while (*pos != '\n') {
                    *texture_pos++ = *pos++;
                }

                *texture_pos++ = '\0';
            } else if (strcmp(block_name, "font_size") == 0) {
                font->size = strtof(pos, &pos);
            } else if (strcmp(block_name, "line_height") == 0) {
                font->line_height = strtof(pos, &pos);
            } else if (strcmp(block_name, "image_width") == 0) {
                image_width = strtoul(pos, &pos, 10);
            } else if (strcmp(block_name, "image_height") == 0) {
                image_height = strtoul(pos, &pos, 10);
            } else if (strcmp(block_name, "glyph_count") == 0) {
                // glyph_count has to be the last general element
                font->glyph_count = strtoul(pos, &pos, 10);
                start = false;
            }

            // Go to next line
            while (*pos != '\0' && *pos++ != '\n') {};
        } else {
            // Parsing glyphs
            // In the text file we don't have to define width and height of the character, we calculate that here
            font->glyphs[glyph_index] = {
                strtoul(pos, &pos, 10),
                {0.0f, 0.0f, strtof(++pos, &pos), strtof(++pos, &pos), strtof(++pos, &pos)},
                {strtof(++pos, &pos), strtof(++pos, &pos), strtof(++pos, &pos), strtof(++pos, &pos)}
            };

            font->glyphs[glyph_index].metrics.width = font->glyphs[glyph_index].coords.x2 - font->glyphs[glyph_index].coords.x1;
            font->glyphs[glyph_index].metrics.height = font->glyphs[glyph_index].coords.y2 - font->glyphs[glyph_index].coords.y1;

            font->glyphs[glyph_index].coords.x1 /= image_width;
            font->glyphs[glyph_index].coords.x2 /= image_width;

            font->glyphs[glyph_index].coords.y1 /= image_height;
            font->glyphs[glyph_index].coords.y2 /= image_height;

            ++glyph_index;

            // Go to next line
            while (*pos != '\n' && *pos != '\0') { ++pos; };
            ++pos;
        }
    }
}

inline
int32 font_data_size(const Font* font)
{
    ASSERT_SIMPLE_CONST(sizeof(Glyph) == GLYPH_SIZE);
    return font->glyph_count * sizeof(Glyph)
        + sizeof(font->glyph_count)
        + sizeof(font->texture_name)
        + sizeof(font->size)
        + sizeof(font->line_height);
}

int32 font_from_data(
    const byte* data,
    Font* font,
    int32 steps = 8
)
{
    const byte* pos = data;

    // Read count
    font->glyph_count = SWAP_ENDIAN_LITTLE(*((uint32 *) pos));
    pos += sizeof(font->glyph_count);

    // Read texture name
    memcpy(font->texture_name, pos, ARRAY_COUNT(font->texture_name) * sizeof(char));
    pos += ARRAY_COUNT(font->texture_name) * sizeof(char);

    // Read font size
    font->size = SWAP_ENDIAN_LITTLE(*((f32 *) pos));
    pos += sizeof(font->size);

    // Read line height
    font->line_height = SWAP_ENDIAN_LITTLE(*((f32 *) pos));
    pos += sizeof(font->line_height);

    memcpy(font->glyphs, pos, font->glyph_count * sizeof(Glyph));

    #if OPENGL
        // @todo Implement y-offset correction
        for (uint32 i = 0; i < font->glyph_count; ++i) {
            float temp = font->glyphs[i].coords.y1;
            font->glyphs[i].coords.y1 = 1.0f - font->glyphs[i].coords.y2;
            font->glyphs[i].coords.y2 = 1.0f - temp;
        }
    #endif

    SWAP_ENDIAN_LITTLE_SIMD(
        (int32 *) font->glyphs,
        (int32 *) font->glyphs,
        font->glyph_count * sizeof(Glyph) / 4, // everything in here is 4 bytes -> super easy to swap
        steps
    );

    return font_data_size(font);
}

int32 font_to_data(
    const Font* font,
    byte* data,
    int32 steps = 8
)
{
    byte* pos = data;

    // Glyph count
    *((uint32 *) pos) = font->glyph_count;
    pos += sizeof(font->glyph_count);

    // Texture name
    memcpy(pos, font->texture_name, ARRAY_COUNT(font->texture_name) * sizeof(char));
    pos += ARRAY_COUNT(font->texture_name) * sizeof(char);

    // Font size
    *((f32 *) pos) = font->size;
    pos += sizeof(font->size);

    // Line height
    *((f32 *) pos) = font->line_height;
    pos += sizeof(font->line_height);

    // The glyphs are naturally tightly packed -> we can just store the memory
    memcpy(pos, font->glyphs, font->glyph_count * sizeof(Glyph));
    pos += font->glyph_count * sizeof(Glyph);

    int32 size = font_data_size(font);

    SWAP_ENDIAN_LITTLE_SIMD(
        (int32 *) file.content,
        (int32 *) file.content,
        size / 4, // everything in here is 4 bytes -> super easy to swap
        steps
    );

    return size;
}

f32 font_line_height(Font* font, f32 size)
{
    return font->line_height * size / font->size;
}

#endif