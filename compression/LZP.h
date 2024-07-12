/**
 * Jingga
 *
 * @copyright Jingga
 * @license   OMS License 2.0
 * @version   1.0.0
 * @link      https://jingga.app
 */
#ifndef TOS_COMPRESSION_LZP_H
#define TOS_COMPRESSION_LZP_H

#include <stdio.h>

#include "../stdlib/Types.h"

uint32 encode_lzp(const byte* in, size_t length, byte* out)
{
    byte buf[9];
    byte table[1 << 16] = {0};

    uint16 hash = 0;
    int32 i, j;
    byte mask, c;
    uint32 in_pos = 0, out_pos = 0;

    while(true) {
        j = 1;
        mask = 0;
        for (i = 0; i < 8; ++i) {
            if (in_pos == length) {
                break;
            }

            c = in[in_pos++];
            if (c == table[hash]) {
                mask |= 1 << i;
            } else {
                table[hash] = c;
                buf[j++] = c;
            }

            hash = (hash << 4) ^ c;
        }

        if (i > 0) {
            buf[0] = mask;
            for (i = 0; i < j; ++i) {
                out[out_pos++] = buf[i];
            }
        }

        if (in_pos == length) {
            break;
        }
    }

    return out_pos;
}

uint32 decode_lzp(const byte* in, size_t length, byte* out)
{
    byte buf[8];
    byte table[1 << 16] = {0};

    uint16 hash = 0;
    int i, j;
    byte mask, c;
    uint32 in_pos = 0, out_pos = 0;

    while (true) {
        j = 0;
        if (in_pos == length) {
            break;
        }

        mask = in[in_pos++];
        for (i = 0; i < 8; ++i) {
            if ((mask & (1 << i)) != 0) {
                c = table[hash];
            } else {
                if (in_pos == length) {
                    break;
                }

                c = in[in_pos++];
                table[hash] = c;
            }

            buf[j++] = c;

            hash = (hash << 4) ^ c;
        }

        if (j > 0) {
            for (i = 0; i < j; ++i) {
                out[out_pos++] = buf[i];
            }
        }
    }

    return out_pos;
}

#endif