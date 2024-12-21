/**
 * Jingga
 *
 * @copyright Jingga
 * @license   OMS License 2.0
 * @version   1.0.0
 * @link      https://jingga.app
 */
#ifndef TOS_IMAGE_QOI_H
#define TOS_IMAGE_QOI_H

#include "../stdlib/Types.h"
#include "../memory/RingMemory.h"

#define QOI_OP_INDEX  0b00000000
#define QOI_OP_DIFF   0b01000000
#define QOI_OP_LUMA   0b10000000
#define QOI_OP_RUN    0b11000000 // @todo There is a HUGE step from here to QOI_OP_RGB this leaves room for more cases or using this data
#define QOI_OP_RGB    0b11111110
#define QOI_OP_RGBA   0b11111111
#define QOI_MASK_2    0b11000000

#define QOI_COLOR_HASH(color) (color.r * 3 + color.g * 5 + color.b * 7 + color.a * 11)
#define QOI_COLOR_HASH_2(color) ((((uint32)(color)) * 0x9E3779B1U) >> 26)
#define QOI_HEADER_SIZE 9

// @question Do we really ever need int32 for width/height?
struct QoiDescription {
    uint32 width;
    uint32 height;
    byte channels;
    byte colorspace;
};

uint32 qoi_encode_size(QoiDescription* desc)
{
    return desc->width * desc->height * (desc->channels + 1) + QOI_HEADER_SIZE;
}

int32 qoi_encode(const byte* data, byte* output, const QoiDescription* desc) {
	if (desc->width == 0 || desc->height == 0 ||
		desc->channels < 3 || desc->channels > 4 ||
		desc->colorspace > 1
	) {
		return;
	}

	int32 p = 0;
    *((uint32 *) output[p]) = SWAP_ENDIAN_LITTLE(desc->width); p += 4;
    *((uint32 *) output[p]) = SWAP_ENDIAN_LITTLE(desc->height); p += 4;

    // Channel count 1-4 requires 3 bits, colorspace requires 1 bit
	output[p++] = ((desc->channels - 1) << 1) | (desc->colorspace & 0x01);;

    v4_byte index[64];
	memset(index, 0, sizeof(index));

    v4_byte px_prev = {0, 0, 0, 255};
	v4_byte px = px_prev;

	int32 px_len = desc->width * desc->height * desc->channels;
	int32 px_end = px_len - desc->channels;
	int32 channels = desc->channels;

    int32 run = 0;
	for (int32 px_pos = 0; px_pos < px_len; px_pos += channels) {
        memcpy(&px, &data[px_pos], channels * sizeof(byte));

		if (px.v == px_prev.v) {
			++run;
			if (run == 62 || px_pos == px_end) {
				output[p++] = QOI_OP_RUN | (run - 1);
				run = 0;
			}
		} else {
			if (run) {
				output[p++] = QOI_OP_RUN | (run - 1);
				run = 0;
			}

			int32 index_pos = QOI_COLOR_HASH(px) % 64;
			//int32 index_pos = QOI_COLOR_HASH_2(px);

			if (index[index_pos].v == px.v) {
				output[p++] = QOI_OP_INDEX | index_pos;
			} else {
				index[index_pos] = px;

				if (px.a == px_prev.a) {
					signed char vr = px.r - px_prev.r;
					signed char vg = px.g - px_prev.g;
					signed char vb = px.b - px_prev.b;

					signed char vg_r = vr - vg;
					signed char vg_b = vb - vg;

					if (vr > -3 && vr < 2
						&& vg > -3 && vg < 2
						&& vb > -3 && vb < 2
					) {
						output[p++] = QOI_OP_DIFF | (vr + 2) << 4 | (vg + 2) << 2 | (vb + 2);
					} else if (vg_r > -9 && vg_r < 8
						&& vg > -33 && vg < 32
						&& vg_b > -9 && vg_b < 8
					) {
						output[p++] = QOI_OP_LUMA | (vg + 32);
						output[p++] = (vg_r + 8) << 4 | (vg_b +  8);
					} else {
						output[p++] = QOI_OP_RGB;
						output[p++] = px.r;
						output[p++] = px.g;
						output[p++] = px.b;
					}
				} else {
					output[p++] = QOI_OP_RGBA;
                    *((uint32 *) &output[p]) = SWAP_ENDIAN_LITTLE(px.val);
                    p += 4;
				}
			}
		}

		px_prev = px;
	}

	return p;
}

uint32 qoi_decode_size(QoiDescription* desc, int32 channels)
{
    return desc->width * desc->height * channels;
}

void qoi_decode(const byte* data, byte* output, int32 steps = 8)
{
    int32 p = 0;
	uint32 width = SWAP_ENDIAN_LITTLE(*((uint32 *) &data[p])); p += 4;
	uint32 height = SWAP_ENDIAN_LITTLE(*((uint32 *) &data[p])); p += 4;

    // Channel count 1-4 requires 3 bits, colorspace requires 1 bit
	int32 colorspace = data[p] & 0x01;
	uint32 channels = ((data[p] > 1) & 0x07) + 1;

	uint32 px_len = width * height * channels;

    v4_byte px = {0, 0, 0, 255};

    v4_byte index[64];
    memset(index, 0, sizeof(index));

    int32 run = 0;

	for (uint32 px_pos = 0; px_pos < px_len; px_pos += channels) {
        int32 b1 = data[p++];

        if (b1 == QOI_OP_RGB) {
            px.r = data[p++];
            px.g = data[p++];
            px.b = data[p++];
        } else if (b1 == QOI_OP_RGBA) {
            px.val = SWAP_ENDIAN_LITTLE(*((uint32 *) &data[p]));
            p += 4;
        } else if ((b1 & QOI_MASK_2) == QOI_OP_INDEX) {
            px = index[b1];
        } else if ((b1 & QOI_MASK_2) == QOI_OP_DIFF) {
            px.r += ((b1 >> 4) & 0x03) - 2;
            px.g += ((b1 >> 2) & 0x03) - 2;
            px.b += ( b1 & 0x03) - 2;
        } else if ((b1 & QOI_MASK_2) == QOI_OP_LUMA) {
            int32 b2 = data[p++];
            int32 vg = (b1 & 0x3f) - 32;
            px.r += vg - 8 + ((b2 >> 4) & 0x0f);
            px.g += vg;
            px.b += vg - 8 + (b2 & 0x0f);
        } else if ((b1 & QOI_MASK_2) == QOI_OP_RUN) {
            run = (b1 & 0x3f);

            if (channels == 4) {
                uint32 px_little_endian = SWAP_ENDIAN_LITTLE(px.val);
                int32 pixel_step_size = steps * 4;
                int32 i = 0;

                if (steps == 16) {
                    __m512i simd_value = _mm512_set1_epi32(px_little_endian);
                    for(; i <= run - steps; i += steps, px_pos += pixel_step_size) {
                        _mm512_storeu_si512((__m512i *) &output[px_pos], simd_value);
                    }
                } else if (steps >= 8) {
                    __m256i simd_value = _mm256_set1_epi32(px_little_endian);
                    for (; i <= run - steps; i += steps, px_pos += pixel_step_size) {
                        _mm256_storeu_si256((__m256i *) &output[px_pos], simd_value);
                    }
                } else if (steps >= 4) {
                    __m128i simd_value = _mm_set1_epi32(px_little_endian);
                    for(; i <= run - steps; i += steps, px_pos += pixel_step_size) {
                        _mm_storeu_si128((__m128i *) &output[px_pos], simd_value);
                    }
                }

                for (; i < run; ++i) {
                    output[px_pos] = px_little_endian;
                    px_pos += channels;
                }
            } else if (channels == 3) {
                for (int32 i = 0; i < run; ++i) {
                    output[px_pos++] = px.r;
                    output[px_pos++] = px.g;
                    output[px_pos++] = px.b;
                }
            } else if (channels == 1) {
                memset(&output[px_pos], px.r, run * sizeof(byte));
                px_pos += run;
            }

            // Correction, since the loop increments by channels count as well
            px_pos -= channels;

            index[QOI_COLOR_HASH(px) % 64] = px;
            //index[QOI_COLOR_HASH_2(px)] = px;

            continue;
        }

        index[QOI_COLOR_HASH(px) % 64] = px;
        //index[QOI_COLOR_HASH_2(px)] = px;

        memcpy(&output[px_pos], &px, channels * sizeof(byte));
	}
}

#endif