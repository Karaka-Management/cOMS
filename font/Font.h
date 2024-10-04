#ifndef TOS_FONT_H
#define TOS_FONT_H

#include "../stdlib/Types.h"
#include "../memory/BufferMemory.h"

// @todo Move this somewhere else, it doesn't belong here
enum UIAlignH {
    UI_ALIGN_H_LEFT,
    UI_ALIGN_H_CENTER,
    UI_ALIGN_H_RIGHT,
};

enum UIAlignV {
    UI_ALIGN_V_BOTTOM,
    UI_ALIGN_V_CENTER,
    UI_ALIGN_V_TOP,
};

struct GlyphMetrics {
    f32 width;         // Width of the glyph
    f32 height;        // Height of the glyph
    f32 offset_x;       // Horizontal offset from baseline
    f32 offset_y;       // Vertical offset from baseline
    f32 advance_x;      // Horizontal advance after drawing the glyph
};

// @question Do we even need all this information? x2 and y2 follow from width and height, no?
struct GlyphTextureCoords {
    f32 x1;
    f32 y1;

    f32 x2;
    f32 y2;
};

struct Glyph {
    uint32 codepoint;
    GlyphMetrics metrics;
    GlyphTextureCoords coords;
};

struct Font {
    uint32 glyph_count;
    f32 size; // Default font size
    uint32 line_height;
    Glyph* glyphs;

    // @question Do we want to have a pointer to the glyph Texture
};

inline
void font_init(BufferMemory* buf, Font* font, int count)
{
    font->glyphs = (Glyph *) buffer_get_memory(buf, sizeof(Glyph) * count);
    font->glyph_count = count;
}

inline
Glyph* font_glyph_find(Font* font, uint32 codepoint)
{
    for (int i = 0; i < font->glyph_count; ++i) {
        if (font->glyphs[i].codepoint == codepoint) {
            return &font->glyphs[i];
        }
    }

    return NULL;
}

#endif