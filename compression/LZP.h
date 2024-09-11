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
#include <string.h>

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

int find_longest_match(char *window, int window_start, char *buffer, int buffer_size, int *match_position) {
    int best_length = 0;
    int best_offset = 0;

    for (int i = window_start; i < 4096   && i < buffer_size; ++i) {
        int length = 0;

        while (length < 18 &&
               i + length < 4096   &&
               buffer[length] == window[i + length]) {
            length++;
        }

        if (length > best_length) {
            best_length = length;
            best_offset = i;
        }
    }

    *match_position = best_offset;

    return best_length;
}

uint32 encode_lzp3(const byte* in, size_t length, byte* out) {
    char window[4096] = {0};
    int window_start = 0;

    int out_size = 0;

    int i = 0;
    while (i < length) {
        int match_position = 0;
        int match_length = find_longest_match(window, window_start, (char *)&in[i], (int) (length - i), &match_position);

        if (match_length > 2) {
            out[out_size++] = 0xFF;
            out[out_size++] = match_position & 0xFF;
            out[out_size++] = match_length & 0xFF;

            i += match_length;
        } else {
            out[out_size++] = in[i];
            ++i;
        }

        int shift_length = match_length > 0 ? match_length : 1;
        memmove(window, window + shift_length, 4096 - shift_length);
        memcpy(window + (4096 - shift_length), &in[i - shift_length], shift_length);
        window_start = (window_start + shift_length) >= 4096 ? 0 : window_start + shift_length;
    }

    return out_size;
}

uint32 decode_lzp3(const byte* in, size_t length, byte* out) {
    char window[4096] = {0};
    int window_start = 0;

    int out_size = 0;

    int i = 0;
    while (i < length) {
        if (in[i] == 0xFF) {
            int match_position = in[i + 1];
            int match_length = in[i + 2];

            for (int j = 0; j < match_length; j++) {
                out[out_size++] = window[(match_position + j) % 4096];
            }

            memmove(window, window + match_length, 4096 - match_length);
            memcpy(window + (4096 - match_length), &out[out_size - match_length], match_length);
            window_start = (window_start + match_length) >= 4096 ? 0 : window_start + match_length;

            i += 3;
        } else {
            out[out_size++] = in[i];

            memmove(window, window + 1, 4096 - 1);
            window[4096 - 1] = in[i];
            window_start = (window_start + 1) >= 4096 ? 0 : window_start + 1;

            ++i;
        }
    }

    return out_size;
}

#endif