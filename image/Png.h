/**
 * Jingga
 *
 * @copyright Jingga
 * @license   OMS License 2.0
 * @version   1.0.0
 * @link      https://jingga.app
 *
 * png: https://www.w3.org/TR/2003/REC-PNG-20031110/
 * zlib: https://www.ietf.org/rfc/rfc1950.txt
 * deflate: https://www.ietf.org/rfc/rfc1951.txt
 */
#ifndef TOS_IMAGE_PNG_H
#define TOS_IMAGE_PNG_H

#include <string.h>
#include "../stdlib/Types.h"
#include "../utils/Utils.h"
#include "../utils/EndianUtils.h"
#include "Image.h"

// Packed header size
#define PNG_HEADER_SIZE 8

struct PngHeader {
    byte signature[8];
};

struct PngChunk {
    uint32 length;
    uint32 type;
    uint32 crc;
};

struct PngIHDR {
    uint32 length;
    uint32 type;
    uint32 width;
    uint32 height;
    byte bit_depth;
    byte colory_type;
    byte compression;
    byte filter;
    byte interlace;
    uint32 crc;
};

struct PngIDATHeader {
    byte zlib_method_flag;
    byte add_flag;
};

struct Png {
    PngHeader header;
    PngIHDR ihdr;

    // Encoded pixel data
    byte* pixels; // WARNING: This is not the owner of the data. The owner is the FileBody

    uint32 size;
    byte* data; // WARNING: This is not the owner of the data. The owner is the FileBody
};

struct PngHuffmanEntry {
    uint16 symbol;
    uint16 bits_used;
};

struct PngHuffman {
    uint32 max_code_length; // in bits
    uint32 count;
    PngHuffmanEntry entries[32768]; // 2^15
};

static const byte PNG_SIGNATURE[] = {0x89, 0x50, 0x4E, 0x47, 0x0D, 0x0A, 0x1A, 0x0A};
static const uint32 HUFFMAN_BIT_COUNTS[][2] = {{143, 8}, {255, 9}, {279, 7}, {287, 8}, {319, 5}};
static const uint32 HUFFMAN_CODE_LENGTH_ALPHA[] = {
    16, 17, 18, 0, 8, 7, 9, 6, 10, 5, 11, 4, 12, 3, 13, 2, 14, 1, 15
};
static const PngHuffmanEntry PNG_LENGTH_EXTRA[] = {
    {3, 0}, {4, 0}, {5, 0}, {6, 0}, {7, 0}, {8, 0}, {9, 0}, {10, 0}, {11, 1},
    {13, 1}, {15, 1}, {17, 1}, {19, 2}, {23, 2}, {27, 2}, {31, 2}, {35, 3},
    {43, 3}, {51, 3}, {59, 3}, {67, 4}, {83, 4}, {99, 4}, {115, 4}, {131, 5},
    {163, 5}, {195, 5}, {227, 5}, {258, 0}
};

static const PngHuffmanEntry PNG_DIST_EXTRA[] = {
    {1, 0}, {2, 0}, {3, 0}, {4, 0}, {5, 1}, {7, 1}, {9, 2}, {13, 2}, {17, 3},
    {25, 3}, {33, 4}, {49, 4}, {65, 5}, {97, 5}, {129, 6}, {193, 6}, {257, 7},
    {385, 7}, {513, 8}, {769, 8}, {1025, 9}, {1537, 9}, {2049, 10}, {3073, 10},
    {4097, 11}, {6145, 11}, {8193, 12}, {12289, 12}, {16385, 13}, {24577, 13}
};

void huffman_png_compute(uint32 symbol_count, uint32* symbol_code_length, PngHuffman* huff)
{
    uint32 code_length_hist[16] = {};
    for (uint32 i = 0; i < symbol_count; ++i) {
        ++code_length_hist[symbol_code_length[i]];
    }

    uint32 next_unused_code[16];
    next_unused_code[0] = 0;
    code_length_hist[0] = 0;

    for (uint32 i = 1; i < 16; ++i) {
        next_unused_code[i] = (next_unused_code[i - 1] + code_length_hist[i - 1]) << 1;
    }

    for (uint32 i = 0; i < symbol_count; ++i) {
        uint32 code_length = symbol_code_length[i];
        if (!code_length) {
            continue;
        }

        uint32 code = next_unused_code[code_length]++;
        uint32 bits = huff->max_code_length - code_length;
        uint32 entries = 1 << bits;

        for (uint32 j = 0; j < entries; ++j) {
            uint32 base_index = (code << bits) | j;
            uint32 index = reverse_bits(base_index, huff->max_code_length);

            PngHuffmanEntry* entry = huff->entries + index;

            entry->bits_used = (uint16) code_length;
            entry->symbol = (uint16) i;
        }
    }
}

PngHuffmanEntry huffman_png_decode(PngHuffman* huff, const byte* data, int pos)
{
    uint32 index = get_bits(data, huff->max_code_length, pos);
    return huff->entries[index];
}

void png_filter_reconstruct(uint32 width, uint32 height, const byte* decompressed, byte* finalized, int steps)
{
    uint32 zero = 0;
    byte* prev_row = NULL;
    byte prev_row_advance = 0;

    for (uint32 y = 0; y < height; ++y) {
        byte filter = *decompressed;
        byte* current_row = ;

        switch (filter) {
            case 0: {
                    memcpy(finalized + y * width, decompressed + y * width, width);
                } break;
            case 1: {
                    // no simd possible, well 4 + 4 probably not worth it

                } break;
            case 2: {
                    // requires manual simd impl. since prev_row_advance can be 0 or 4
                } break;
            case 3: {
                    // no simd possible, well 4 + 4 probably not worth it
                } break;
            case 4: {
                    // no simd possible, well 4 + 4 probably not worth it
                } break;
            default: {

            }
        }

        prev_row = current_row;
        prev_row_advance = 4;
    }
}

void generate_default_png_references(const FileBody* file, Png* png)
{
    png->size = (uint32) file->size;
    png->data = file->content;

    if (png->size < 33) {
        // This shouldn't happen
        return;
    }

    // The first chunk MUST be IHDR -> we handle it here
    memcpy(png, file->content, 29);
    png->ihdr.crc = SWAP_ENDIAN_BIG((uint32 *) (file->content + 30));

    png->ihdr.length = SWAP_ENDIAN_BIG(&png->ihdr.length);
    png->ihdr.type = SWAP_ENDIAN_BIG(&png->ihdr.type);
    png->ihdr.width = SWAP_ENDIAN_BIG(&png->ihdr.width);
    png->ihdr.height = SWAP_ENDIAN_BIG(&png->ihdr.height);
}

bool image_png_generate(const FileBody* src_data, Image* image, int steps = 8)
{
    // @performance We are generating the struct and then filling the data.
    //      There is some asignment/copy overhead
    Png src = {};
    generate_default_png_references(src_data, &src);

    // @todo We probably need the following buffers
    //  1. file buffer (already here)
    //  2. block buffer
    //  3. temp pixel buffer (larger)
    //  4. final pixel buffer (already here)

    if (src.ihdr.bit_depth != 8
        || src.ihdr.colory_type != 6
        || src.ihdr.compression != 0
        || src.ihdr.filter != 0
        || src.ihdr.interlace != 0
    ) {
        // We don't support this type of png
        return false;
    }

    PngChunk chunk;
    PngIDATHeader idat_header;

    bool is_first_idat = true;

    uint32 out_pos = 0;

    // @question the following is a lot of data, should this be moved to heap?
    uint32 literal_length_dist_table[512];

    PngHuffman literal_length_huffman;
    literal_length_huffman.max_code_length = 15;
    literal_length_huffman.count = 1 << literal_length_huffman.max_code_length;

    PngHuffman distance_huffman;
    distance_huffman.max_code_length = 15;
    distance_huffman.count = 1 << distance_huffman.max_code_length;

    PngHuffman dictionary_huffman;
    dictionary_huffman.max_code_length = 7;
    dictionary_huffman.count = 1 << dictionary_huffman.max_code_length;

    // i is the current byte to read
    int i = 33;

    // r is the re-shift value in case we need to go back
    int r = 0;

    // b is the current bit to read
    int b = 0;

    while(i < src.size) {
        chunk.length = SWAP_ENDIAN_BIG((uint32 *) (src_data->content + i));
        chunk.type = SWAP_ENDIAN_BIG((uint32 *) (src_data->content + i + 4));

        // For our png reader, we only care about IDAT
        //  @question consider PLTE, tRNS, gAMA, iCCP
        if (chunk.type == 'IEND') {
            break;
        } else if (chunk.type != 'IDAT') {
            // IDAT chunks are continuous and we don't care for anything else
            if (!is_first_idat) {
                break;
            }

            i += chunk.length + 12;
            continue;
        }

        if (is_first_idat) {
            idat_header.zlib_method_flag = *(src_data->content + i + 8);
            idat_header.add_flag = *(src_data->content + i + 9);

            byte CM = idat_header.zlib_method_flag & 0xF;
            byte FDICT = (idat_header.add_flag >> 5) & 0x1;

            is_first_idat = false;

            if (CM != 8 || FDICT != 0) {
                return false;
            }

            i += 10;
        }

        // @bug The algorithm below works on "blocks".
        //      Could it be possible that a block is spread accross 2 IDAT chunks?
        //      If so this would be bad and break the code below
        //      We could solve this by just having another counting variable and jump to the next block

        // start: src_data->content + i + 8
        // end: src_data->content + i + 8 + length - 1

        // DEFLATE Algorithm
        // @bug the following 3 lines are wrong, they don't have to start at a bit 0/1
        //      A block doesn't have to start at an byte boundary
        byte BFINAL = get_bits(src_data->content + i, 1, b);
        i += (b > 7 - 1);
        b = (b + 1) & 7;

        byte BTYPE = get_bits(src_data->content + i, 2, b);
        i += (b > 7 - 2);
        b = (b + 2) & 7;

        if (BTYPE == 0) {
            // starts at byte boundary -> position = +1 of previous byte
            if (b == 0) {
                i -= 1;
            }

            uint16 len = *((uint16 *) (src_data->content + i + 1));
            uint16 nlen = *((uint16 *) (src_data->content + i + 3));

            memcpy(image->pixels + out_pos, src_data->content + i + 5, len);
            out_pos += len;

            i += 5 + len;
            b = 0;
        } else {
            // @question is this even required or are we overwriting anyways?
            memset(&literal_length_dist_table, 0, 512 * 4);
            memset(&literal_length_huffman.entries, 0, sizeof(PngHuffmanEntry) * 15);
            memset(&distance_huffman.entries, 0, sizeof(PngHuffmanEntry) * 15);
            memset(&dictionary_huffman.entries, 0, sizeof(PngHuffmanEntry) * 7);

            uint32 huffman_literal = 0;
            uint32 huffman_dist = 0;

            if (BTYPE == 2) {
                // Compressed with dynamic Huffman code
                huffman_literal = get_bits(src_data->content + i, 5, b);
                i += (b > 7 - 5);
                b = (b + 5) & 7;

                huffman_dist = get_bits(src_data->content + i, 5, b);
                i += (b > 7 - 5);
                b = (b + 5) & 7;

                uint32 huffman_code_length = get_bits(src_data->content + i, 4, b);
                i += (b > 7 - 4);
                b = (b + 4) & 7;

                huffman_literal += 257;
                huffman_dist += 1;
                huffman_code_length += 4;

                uint32 huffman_code_length_table[19] = {};

                for (uint32 j = 0; j < huffman_code_length; ++j) {
                    huffman_code_length_table[HUFFMAN_CODE_LENGTH_ALPHA[j]] = get_bits(src_data->content + i, 3, b);
                    i += (b > 7 - 3);
                    b = (b + 3) & 7;
                }

                huffman_png_compute(19, huffman_code_length_table, &dictionary_huffman);

                uint32 literal_length_count = 0;
                uint32 length_count = huffman_literal + huffman_dist;

                while (literal_length_count < length_count) {
                    // @todo implement
                    uint32 rep_count = 1;
                    uint32 rep_val = 0;

                    PngHuffmanEntry dict = huffman_png_decode(&dictionary_huffman, src_data->content + i, b);
                    i += (b + dict.bits_used) / 8;
                    b = (b + dict.bits_used) & 7;

                    uint32 encoded_length = dict.bits_used;

                    if (encoded_length <= 15) {
                        rep_val = encoded_length;
                    } else if (encoded_length == 16) {
                        rep_count = 3 + get_bits(src_data->content + i, 2, b);
                        i += (b > 7 - 2);
                        b = (b + 2) & 7;

                        rep_val = literal_length_dist_table[literal_length_count - 1];
                    } else if (encoded_length == 17) {
                        rep_count = 3 + get_bits(src_data->content + i, 3, b);
                        i += (b > 7 - 3);
                        b = (b + 3) & 7;
                    } else if (encoded_length == 18) {
                        rep_count = 11 + get_bits(src_data->content + i, 7, b);
                        i += (b > 7 - 7);
                        b = (b + 7) & 7;
                    }

                    memset(literal_length_dist_table + literal_length_count, rep_val, rep_count);
                }
            } else if (BTYPE == 1) {
                // Compressed with fixed Huffman code
                huffman_literal = 288;
                huffman_dist = 32;

                uint32 bit_index = 0;
                for(uint32 range_index = 0; range_index < 5; ++range_index) {
                    uint32 bit_count = HUFFMAN_BIT_COUNTS[range_index][1];
                    uint32 last = HUFFMAN_BIT_COUNTS[range_index][0];

                    while(bit_index <= last) {
                        literal_length_dist_table[bit_index++] = bit_count;
                    }
                }
            }

            huffman_png_compute(huffman_literal, literal_length_dist_table, &literal_length_huffman);
            huffman_png_compute(huffman_dist, literal_length_dist_table + huffman_literal, &distance_huffman);

            while (true) {
                PngHuffmanEntry literal = huffman_png_decode(&literal_length_huffman, src_data->content + i, b);
                i += (b + literal.bits_used) / 8;
                b = (b + literal.bits_used) & 7;

                uint32 literal_length = literal.bits_used;

                if (literal_length == 256) {
                    break;
                }

                if (literal_length <= 255) {
                    *(image->pixels + out_pos) = (byte) (literal_length & 0xFF);
                    ++out_pos;
                } else {
                    uint32 length_tab_index = literal_length - 257;
                    PngHuffmanEntry length_tab = PNG_LENGTH_EXTRA[length_tab_index];
                    uint32 length = length_tab.symbol;

                    if (length_tab.bits_used) {
                        uint32 extra_bits = get_bits(src_data->content + i, length_tab.bits_used, b);
                        i += (b + length_tab.bits_used) / 8;
                        b = (b + length_tab.bits_used) & 7;

                        length += extra_bits;
                    }

                    PngHuffmanEntry tab = huffman_png_decode(&distance_huffman, src_data->content + i, b);
                    i += (b + tab.bits_used) / 8;
                    b = (b + tab.bits_used) & 7;

                    uint32 dist_tab_index = tab.bits_used;

                    PngHuffmanEntry dist_tab = PNG_DIST_EXTRA[dist_tab_index];
                    uint32 dist = dist_tab.symbol;

                    if (dist_tab.bits_used) {
                        uint32 extra_bits = get_bits(src_data->content + i, dist_tab.bits_used, b);
                        i += (b + dist_tab.bits_used) / 8;
                        b = (b + dist_tab.bits_used) & 7;

                        dist += extra_bits;
                    }

                    memcpy(image->pixels + out_pos, image->pixels + out_pos - dist, length);
                }
            }
        }

        if (BFINAL == 0) {
            break;
        }
    }

    image->width = src.ihdr.width;
    image->height = src.ihdr.height;

    // @todo fix pixels parameter
    png_filter_reconstruct(image->width, image->height, image->pixels, image->pixels, steps);

    return true;
}

#endif