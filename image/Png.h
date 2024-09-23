/**
 * Jingga
 *
 * @copyright Jingga
 * @license   OMS License 2.0
 * @version   1.0.0
 * @link      https://jingga.app
 *
 * png: https://www.w3.org/TR/2003/REC-PNG-20031110/
 * png: https://www.w3.org/TR/PNG-Chunks.html
 * zlib: https://www.ietf.org/rfc/rfc1950.txt
 * deflate: https://www.ietf.org/rfc/rfc1951.txt
 */
#ifndef TOS_IMAGE_PNG_H
#define TOS_IMAGE_PNG_H

#include <string.h>
#include "../stdlib/Types.h"
#include "../utils/BitUtils.h"
#include "../utils/EndianUtils.h"
#include "Image.h"

// Packed header size
#define PNG_HEADER_SIZE 8

struct PngHeader {
    uint8 signature[8];
};

/*
The following table describes the chunk layout.
Please note that we do NOT support most of this

Critical chunks (order is defined):

    Name  Multiple  Ordering constraints
    IHDR    No      Must be first
    PLTE    No      Before IDAT (optional)
    IDAT    Yes     Multiple IDATs must be consecutive
    IEND    No      Must be last

Ancillary chunks (order is not defined):

    Name  Multiple  Ordering constraints
    cHRM    No      Before PLTE and IDAT
    gAMA    No      Before PLTE and IDAT
    iCCP    No      Before PLTE and IDAT
    sBIT    No      Before PLTE and IDAT
    sRGB    No      Before PLTE and IDAT
    bKGD    No      After PLTE, before IDAT
    hIST    No      After PLTE, before IDAT
    tRNS    No      After PLTE, before IDAT
    pHYs    No      Before IDAT
    sPLT    Yes     Before IDAT
    tIME    No      None
    iTXt    Yes     None
    tEXt    Yes     None
    zTXt    Yes     None
*/
#define PNG_CHUNK_SIZE_MIN 12

struct PngChunk {
    uint32 length;
    uint32 type;
    // +data here, can be 0
    uint32 crc;
};

// Special chunk
#define PNG_IHDR_SIZE 25
struct PngIHDR {
    uint32 length;
    uint32 type;
    uint32 width;
    uint32 height;
    uint8 bit_depth;
    uint8 colory_type;
    uint8 compression;
    uint8 filter;
    uint8 interlace;
    uint32 crc;
};

struct PngIDATHeader {
    uint8 zlib_method_flag;
    uint8 add_flag;
};

struct Png {
    PngHeader header;
    PngIHDR ihdr;

    // Encoded pixel data
    uint8* pixels; // WARNING: This is not the owner of the data. The owner is the FileBody

    uint32 size;
    uint8* data; // WARNING: This is not the owner of the data. The owner is the FileBody
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

static const uint8 PNG_SIGNATURE[] = {0x89, 0x50, 0x4E, 0x47, 0x0D, 0x0A, 0x1A, 0x0A};
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

void huffman_png_compute(uint32 symbol_count, const uint32* __restrict symbol_code_length, PngHuffman* huff)
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
            uint32 index = bits_reverse(base_index, huff->max_code_length);

            PngHuffmanEntry* entry = huff->entries + index;

            entry->bits_used = (uint16) code_length;
            entry->symbol = (uint16) i;
        }
    }
}

inline
uint16 huffman_png_decode(PngHuffman* __restrict huff, BitWalk* __restrict stream)
{
    // huff->max_code_length has a length of a maximum of 15 -> span a maximum of 3 bytes
    uint32 index = SWAP_ENDIAN_BIG(BITS_GET_32(BYTES_MERGE_4(stream->pos), stream->bit_pos, huff->max_code_length));

    bits_walk(stream, huff->entries[index].bits_used);

    return huff->entries[index].symbol;
}

inline
uint8 png_filter_1_and_2(const uint8* __restrict x, const uint8* __restrict a, uint32 channel)
{
    return x[channel] + a[channel];
}

inline
uint8 png_filter_3(const uint8* x, const uint8* a, const uint8* b, uint32 channel)
{
    return x[channel] + (uint8) (((uint32) a[channel] + (uint32) b[channel]) / 2);
}

inline
uint8 png_filter_4(const uint8* x, const uint8* a_full, const uint8* b_full, const uint8* c_full, uint32 channel)
{
    int32 a = (int32) a_full[channel];
    int32 b = (int32) b_full[channel];
    int32 c = (int32) c_full[channel];
    int32 p = a + b - c;
    int32 pa = p >= a ? p - a : a - p;
    int32 pb = p >= b ? p - b : b - p;
    int32 pc = p >= c ? p - c : c - p;

    int32 paeth;
    if (pa < pb && pa <= pc) {
        paeth = a;
    } else if (pb <= pc) {
        paeth = b;
    } else {
        paeth = c;
    }

    return x[channel] + (uint8) paeth;
}

void png_filter_reconstruct(uint32 width, uint32 height, const uint8* decompressed, uint8* finalized, int steps = 8)
{
    uint64 zero = 0;
    uint8* prev_row = (uint8 *) &zero;
    uint8 prev_row_advance = 0;

    const uint8* src = decompressed;
    uint8* dest = finalized;

    for (uint32 y = 0; y < height; ++y) {
        uint8 filter = *decompressed;
        uint8* current_row = dest;

        switch (filter) {
            case 0: {
                    memcpy(dest, src, width * sizeof(uint32));
                    dest += 4 * width;
                    src += 4 * width;
                } break;
            case 1: {
                    uint32 a_pixel = 0;
                    for (uint32 x = 0; x < width; ++x) {
                        // png_filter_1_and_2
                        dest[0] = src[0] + ((uint8 *) &a_pixel)[0];
                        dest[1] = src[1] + ((uint8 *) &a_pixel)[1];
                        dest[2] = src[2] + ((uint8 *) &a_pixel)[2];
                        dest[3] = src[3] + ((uint8 *) &a_pixel)[3];

                        a_pixel = *(uint32 *) dest;

                        dest += 4;
                        src += 4;
                    }
                } break;
            case 2: {
                    // @performance this is simd optimizable
                    // requires manual simd impl. since prev_row_advance can be 0 or 4
                    uint8* b_pixel = prev_row;
                    for (uint32 x = 0; x < width; ++x) {
                        // png_filter_1_and_2
                        dest[0] = src[0] + b_pixel[0];
                        dest[1] = src[1] + b_pixel[1];
                        dest[2] = src[2] + b_pixel[2];
                        dest[3] = src[3] + b_pixel[3];

                        b_pixel += prev_row_advance;

                        dest += 4;
                        src += 4;
                    }
                } break;
            case 3: {
                    uint32 a_pixel = 0;
                    uint8* b_pixel = prev_row;
                    for (uint32 x = 0; x < width; ++x) {
                        // png_filter_3
                        dest[0] = src[0] + (uint8) (((uint32) ((uint8 *) &a_pixel)[0] + (uint32) b_pixel[0]) / 2);
                        dest[1] = src[1] + (uint8) (((uint32) ((uint8 *) &a_pixel)[1] + (uint32) b_pixel[1]) / 2);
                        dest[2] = src[2] + (uint8) (((uint32) ((uint8 *) &a_pixel)[2] + (uint32) b_pixel[2]) / 2);
                        dest[3] = src[3] + (uint8) (((uint32) ((uint8 *) &a_pixel)[3] + (uint32) b_pixel[3]) / 2);

                        a_pixel = *(uint32 *) dest;
                        b_pixel += prev_row_advance;

                        dest += 4;
                        src += 4;
                    }
                } break;
            case 4: {
                    uint32 a_pixel = 0;
                    uint32 c_pixel = 0;
                    uint8* b_pixel = prev_row;
                    for (uint32 x = 0; x < width; ++x) {
                        // png_filter_4
                        dest[0] = png_filter_4(src, (uint8 *) &a_pixel, b_pixel, (uint8 *) &c_pixel, 0);
                        dest[1] = png_filter_4(src, (uint8 *) &a_pixel, b_pixel, (uint8 *) &c_pixel, 1);
                        dest[2] = png_filter_4(src, (uint8 *) &a_pixel, b_pixel, (uint8 *) &c_pixel, 2);
                        dest[3] = png_filter_4(src, (uint8 *) &a_pixel, b_pixel, (uint8 *) &c_pixel, 3);

                        a_pixel = *(uint32 *) dest;
                        c_pixel = *(uint32 *) b_pixel;
                        b_pixel += prev_row_advance;

                        dest += 4;
                        src += 4;
                    }
                } break;
            default: {
                ASSERT_SIMPLE(false);
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

    if (png->size < PNG_IHDR_SIZE + PNG_HEADER_SIZE) {
        // This shouldn't happen
        ASSERT_SIMPLE(false);
        return;
    }

    // The first chunk MUST be IHDR -> we handle it here
    ASSERT_SIMPLE_CONST(PNG_HEADER_SIZE + PNG_IHDR_SIZE == 33);
    memcpy(png, file->content, PNG_HEADER_SIZE + PNG_IHDR_SIZE);

    png->ihdr.length = SWAP_ENDIAN_BIG(png->ihdr.length);
    png->ihdr.type = SWAP_ENDIAN_BIG(png->ihdr.type);
    png->ihdr.width = SWAP_ENDIAN_BIG(png->ihdr.width);
    png->ihdr.height = SWAP_ENDIAN_BIG(png->ihdr.height);
    png->ihdr.crc = SWAP_ENDIAN_BIG(png->ihdr.crc);
}

// Below you will often see code like SWAP_ENDIAN_BIG(BITS_GET_16(BYTES_MERGE_2()))
//      1. Merge two bytes together creating a "new" data structure from which we can easily read bits
//          1.1. This is required to read bits that cross multiple bytes
//          1.2. Only if you read more than 8 bits will you need to merge 4 bytes
//      2. Now we can retrieve the bits from this data structure at a position with a length
//      3. Whenever we use the result as an integer (16 or 32 bits) we need to consider the endianness
bool image_png_generate(const FileBody* src_data, Image* image, RingMemory* ring, int32 steps = 8)
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

    // @todo Consider to support (0, 2, 3, 4, and 6)
    //      A simple black and white image or a image without alpha should be supported
    if (src.ihdr.bit_depth != 8
        || src.ihdr.colory_type != 6
        || src.ihdr.compression != 0
        || src.ihdr.filter != 0
        || src.ihdr.interlace != 0
    ) {
        // We don't support this type of png (see comment below)
        ASSERT_SIMPLE(false);

        /*
        Color   Allowed     Interpretation
        Type    Bit Depths

        0       1,2,4,8,16  Each pixel is a grayscale sample.
        2       8,16        Each pixel is an R,G,B triple.
        3       1,2,4,8     Each pixel is a palette index, a PLTE chunk must appear.
        4       8,16        Each pixel is a grayscale sample, followed by an alpha sample.
        6       8,16        Each pixel is an R,G,B triple, followed by an alpha sample.
        */

        return false;
    }

    // @performance Could we probably avoid this? There is some overhead using this.
    //      We are only using it because there might be situations where there is a bit overhang to another chunk
    BitWalk stream;
    // Note: If we would support more png formats this offset would be wrong
    stream.pos = src_data->content + PNG_IHDR_SIZE + PNG_HEADER_SIZE;
    stream.bit_pos = 0;

    uint32 literal_length_dist_table[512];

    PngHuffman* literal_length_huffman = (PngHuffman *) ring_get_memory(ring, sizeof(PngHuffman));
    literal_length_huffman->max_code_length = 15;
    literal_length_huffman->count = 1 << literal_length_huffman->max_code_length;

    PngHuffman* distance_huffman = (PngHuffman *) ring_get_memory(ring, sizeof(PngHuffman));
    distance_huffman->max_code_length = 15;
    distance_huffman->count = 1 << distance_huffman->max_code_length;

    PngHuffman* dictionary_huffman = (PngHuffman *) ring_get_memory(ring, sizeof(PngHuffman));
    dictionary_huffman->max_code_length = 7;
    dictionary_huffman->count = 1 << dictionary_huffman->max_code_length;

    // We need full width * height, since we don't know how much data this IDAT actually holds
    uint8* finalized = ring_get_memory(ring, src.ihdr.width * src.ihdr.height * 4);

    // Needs some extra space
    uint8* decompressed = ring_get_memory(ring, src.ihdr.width * src.ihdr.height * 4 + src.ihdr.height);

    uint8* dest = decompressed;

    // @bug We might not be able/allowed to simply iterate this loop below since data might be split accross chunks
    //      If that is the case we have to first create a linked list of all the actual data and then we perform the actions below on this linked list
    //      This ofcourse poses the challenge of handling the border between two list elements
    //      Copying data would be slow so we ideally would like to iterate through that list and just handle the border
    //      since the border only becomes relevant at the beginning of every loop we should be fine, no?

    uint8 BFINAL = 0;
    while(stream.pos - src_data->content < src.size && BFINAL == 0) {
        PngChunk chunk;
        PngIDATHeader idat_header;

        // @bug the code below doesn't need bit walk on the first loop, what about the second loop?
        // For our png reader, we only care about IDAT
        //  @question consider PLTE, tRNS, gAMA, iCCP
        chunk.length = SWAP_ENDIAN_BIG(*((uint32 *) stream.pos));
        stream.pos += sizeof(chunk.length);

        chunk.type = *((uint32 *) stream.pos);
        stream.pos += sizeof(chunk.type);

        if (chunk.type == SWAP_ENDIAN_BIG('IEND')) {
            // we arrived at the end of the file
            break;
        } else if (chunk.type != SWAP_ENDIAN_BIG('IDAT')) {
            // some other data

            // Jump to next chunk
            stream.pos += chunk.length + sizeof(chunk.crc);

            continue;
        }

        // @question Not sure if this below is actually the case
        // @bug Is this even correct, we might have an overhang from the previous chunk
        //  Then we need to:
        //      read n bits from the previous chunk
        //      move accross the chunk header data
        //      read another x bits from the new chunk
        //
        //  This means we cannot jump here (or better we need to check if the bit position is != 0)
        // BUT WE MIGHT NOT CARE ABOUT MULTIPLE IDAT CHUNKS?
        idat_header.zlib_method_flag = *stream.pos;
        ++stream.pos;

        idat_header.add_flag = *stream.pos;
        ++stream.pos;

        uint8 CM = idat_header.zlib_method_flag & 0xF;
        uint8 FDICT = (idat_header.add_flag >> 5) & 0x1;

        if (CM != 8 || FDICT != 0) {
            // Not supported
            return false;
        }

        // This data might be stored in the prvious IDAT chunk?!
        BFINAL = (uint8) SWAP_ENDIAN_BIG(BITS_GET_8(*stream.pos, stream.bit_pos, 1));
        bits_walk(&stream, 1);

        uint32 BTYPE = SWAP_ENDIAN_BIG(BITS_GET_8(BYTES_MERGE_2(stream.pos), stream.bit_pos, 2));
        bits_walk(&stream, 2);

        if (BTYPE == 0) {
            // starts at uint8 boundary -> position = +1 of previous uint8
            bits_flush(&stream);

            uint16 len = *((uint16 *) stream.pos);
            stream.pos += 2;

            uint16 nlen = *((uint16 *) stream.pos);
            stream.pos += 2;

            ASSERT_SIMPLE(len == ~nlen);

            memcpy(dest, &stream.pos, len);
            stream.pos += len;
        } else if (BTYPE == 3) {
            // Invalid BTYPE
            ASSERT_SIMPLE(false);
        } else {
            // @question is this even required or are we overwriting anyways?
            memset(&literal_length_dist_table, 0, sizeof(literal_length_dist_table));
            memset(literal_length_huffman->entries, 0, sizeof(PngHuffmanEntry) * literal_length_huffman->max_code_length);
            memset(distance_huffman->entries, 0, sizeof(PngHuffmanEntry) * distance_huffman->max_code_length);
            memset(dictionary_huffman->entries, 0, sizeof(PngHuffmanEntry) * dictionary_huffman->max_code_length);

            uint32 huffman_literal = 0;
            uint32 huffman_dist = 0;

            if (BTYPE == 2) {
                // Compressed with dynamic Huffman code
                huffman_literal = SWAP_ENDIAN_BIG(BITS_GET_16(BYTES_MERGE_2(stream.pos), stream.bit_pos, 5));
                bits_walk(&stream, 5);

                huffman_dist = SWAP_ENDIAN_BIG(BITS_GET_16(BYTES_MERGE_2(stream.pos), stream.bit_pos, 5));
                bits_walk(&stream, 5);

                uint32 huffman_code_length = SWAP_ENDIAN_BIG(BITS_GET_16(BYTES_MERGE_2(stream.pos), stream.bit_pos, 4));
                bits_walk(&stream, 5);

                huffman_literal += 257;
                huffman_dist += 1;
                huffman_code_length += 4;

                uint32 huffman_code_length_table[ARRAY_COUNT(HUFFMAN_CODE_LENGTH_ALPHA)] = {};

                for (uint32 j = 0; j < huffman_code_length; ++j) {
                    huffman_code_length_table[HUFFMAN_CODE_LENGTH_ALPHA[j]] = SWAP_ENDIAN_BIG(BITS_GET_16(BYTES_MERGE_2(stream.pos), stream.bit_pos, 3));
                    bits_walk(&stream, 3);
                }

                huffman_png_compute(ARRAY_COUNT(HUFFMAN_CODE_LENGTH_ALPHA), huffman_code_length_table, dictionary_huffman);

                uint32 literal_length_count = 0;
                uint32 length_count = huffman_literal + huffman_dist;

                while (literal_length_count < length_count) {
                    // @todo implement
                    uint32 rep_count = 1;
                    uint32 rep_val = 0;

                    uint32 encoded_length = huffman_png_decode(dictionary_huffman, &stream);

                    if (encoded_length <= 15) {
                        rep_val = encoded_length;
                    } else if (encoded_length == 16) {
                        rep_count = 3 + SWAP_ENDIAN_BIG(BITS_GET_8(BYTES_MERGE_2(stream.pos), stream.bit_pos, 2));
                        bits_walk(&stream, 2);

                        rep_val = literal_length_dist_table[literal_length_count - 1];
                    } else if (encoded_length == 17) {
                        rep_count = 3 + SWAP_ENDIAN_BIG(BITS_GET_8(BYTES_MERGE_2(stream.pos), stream.bit_pos, 3));
                        bits_walk(&stream, 3);
                    } else if (encoded_length == 18) {
                        rep_count = 11 + SWAP_ENDIAN_BIG(BITS_GET_8(BYTES_MERGE_2(stream.pos), stream.bit_pos, 7));
                        bits_walk(&stream, 7);
                    }

                    while (rep_count--) {
                        literal_length_dist_table[literal_length_count++] = rep_val;
                    }
                }
            } else if (BTYPE == 1) {
                // Compressed with fixed Huffman code
                huffman_literal = 288;
                huffman_dist = 32;

                uint32 bit_index = 0;
                for(uint32 range_index = 0; range_index < ARRAY_COUNT(HUFFMAN_BIT_COUNTS); ++range_index) {
                    uint32 bit_count = HUFFMAN_BIT_COUNTS[range_index][1];
                    uint32 last = HUFFMAN_BIT_COUNTS[range_index][0];

                    while(bit_index <= last) {
                        literal_length_dist_table[bit_index++] = bit_count;
                    }
                }
            }

            huffman_png_compute(huffman_literal, literal_length_dist_table, literal_length_huffman);
            huffman_png_compute(huffman_dist, literal_length_dist_table + huffman_literal, distance_huffman);

            while (true) {
                uint32 literal_length = huffman_png_decode(literal_length_huffman, &stream);
                if (literal_length == 256) {
                    break;
                }

                if (literal_length <= 255) {
                    *dest++ = (literal_length & 0xFF);
                } else {
                    uint32 length_tab_index = literal_length - 257;
                    PngHuffmanEntry length_tab = PNG_LENGTH_EXTRA[length_tab_index];
                    uint32 length = length_tab.symbol;

                    if (length_tab.bits_used) {
                        // @performance If we knew that bits_used is always <= 15 we could use more efficient MERGE/GET
                        uint32 extra_bits = SWAP_ENDIAN_BIG(BITS_GET_32(BYTES_MERGE_4(stream.pos), stream.bit_pos, length_tab.bits_used));
                        bits_walk(&stream, length_tab.bits_used);

                        length += extra_bits;
                    }

                    uint32 dist_tab_index = huffman_png_decode(distance_huffman, &stream);

                    PngHuffmanEntry dist_tab = PNG_DIST_EXTRA[dist_tab_index];
                    uint32 dist = dist_tab.symbol;

                    if (dist_tab.bits_used) {
                        // @performance If we knew that bits_used is always <= 15 we could use more efficient MERGE/GET
                        uint32 extra_bits = SWAP_ENDIAN_BIG(BITS_GET_32(BYTES_MERGE_4(stream.pos), stream.bit_pos, dist_tab.bits_used));
                        bits_walk(&stream, dist_tab.bits_used);

                        dist += extra_bits;
                    }

                    // @performance Maybe we could use memcopy depending on length and dist
                    uint8* source = dest - dist;
                    while (length--) {
                        *dest++ = *source++;
                    }
                }
            }
        }

        // Skip the CRC
        stream.pos += sizeof(chunk.crc);
        stream.bit_pos = 0;
    }

    image->width = src.ihdr.width;
    image->height = src.ihdr.height;
    image->pixel_count = image->width * image->height;
    image->has_alpha = true;
    image->order_pixels = IMAGE_PIXEL_ORDER_RGBA;
    image->order_rows = IMAGE_ROW_ORDER_TOP_TO_BOTTOM;

    png_filter_reconstruct(src.ihdr.width, src.ihdr.height, decompressed, finalized, steps);

    return true;
}

#endif