#ifndef HASH_MD5_H
#define HASH_MD5_H

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

// https://www.rfc-editor.org/rfc/rfc1321

#define F(x, y, z) ((z) ^ ((x) & ((y) ^ (z))))
#define G(x, y, z) ((y) ^ ((z) & ((x) ^ (y))))
#define H(x, y, z) ((x) ^ (y) ^ (z))
#define I(x, y, z) ((y) ^ ((x) | ~(z)))

#define ROUND_OP(f, a, b, c, d, x, t, s)                        \
    (a) += f((b), (c), (d)) + (x) + (t);                        \
    (a)  = (((a) << (s)) | (((a) & 0xffffffff) >> (32 - (s)))); \
    (a) += (b);

#define SET_BLOCK(n)                                                                  \
    (ctx->block[(n)] = (uint32_t) ptr[(n) * 4] | ((uint32_t) ptr[(n) * 4 + 1] << 8) | \
                       ((uint32_t) ptr[(n) * 4 + 2] << 16) | ((uint32_t) ptr[(n) * 4 + 3] << 24))

namespace Hash
{
    typedef struct {
        uint32_t lo, hi;
        uint32_t a, b, c, d;
        unsigned char buffer[64];
        uint32_t block[16];
    } md5_context;

    static const void *body(md5_context *ctx, const void *data, size_t size)
    {
        const unsigned char *ptr;

        uint32_t a, b, c, d;
        uint32_t aa, bb, cc, dd;

        ptr = (const unsigned char *) data;

        a = ctx->a;
        b = ctx->b;
        c = ctx->c;
        d = ctx->d;

        do {
            aa = a;
            bb = b;
            cc = c;
            dd = d;

            /* Round 1 */
            ROUND_OP(F, a, b, c, d, SET_BLOCK(0), 0xd76aa478, 7)
            ROUND_OP(F, d, a, b, c, SET_BLOCK(1), 0xe8c7b756, 12)
            ROUND_OP(F, c, d, a, b, SET_BLOCK(2), 0x242070db, 17)
            ROUND_OP(F, b, c, d, a, SET_BLOCK(3), 0xc1bdceee, 22)
            ROUND_OP(F, a, b, c, d, SET_BLOCK(4), 0xf57c0faf, 7)
            ROUND_OP(F, d, a, b, c, SET_BLOCK(5), 0x4787c62a, 12)
            ROUND_OP(F, c, d, a, b, SET_BLOCK(6), 0xa8304613, 17)
            ROUND_OP(F, b, c, d, a, SET_BLOCK(7), 0xfd469501, 22)
            ROUND_OP(F, a, b, c, d, SET_BLOCK(8), 0x698098d8, 7)
            ROUND_OP(F, d, a, b, c, SET_BLOCK(9), 0x8b44f7af, 12)
            ROUND_OP(F, c, d, a, b, SET_BLOCK(10), 0xffff5bb1, 17)
            ROUND_OP(F, b, c, d, a, SET_BLOCK(11), 0x895cd7be, 22)
            ROUND_OP(F, a, b, c, d, SET_BLOCK(12), 0x6b901122, 7)
            ROUND_OP(F, d, a, b, c, SET_BLOCK(13), 0xfd987193, 12)
            ROUND_OP(F, c, d, a, b, SET_BLOCK(14), 0xa679438e, 17)
            ROUND_OP(F, b, c, d, a, SET_BLOCK(15), 0x49b40821, 22)

            /* Round 2 */
            ROUND_OP(G, a, b, c, d, (ctx->block[1]), 0xf61e2562, 5)
            ROUND_OP(G, d, a, b, c, (ctx->block[6]), 0xc040b340, 9)
            ROUND_OP(G, c, d, a, b, (ctx->block[11]), 0x265e5a51, 14)
            ROUND_OP(G, b, c, d, a, (ctx->block[0]), 0xe9b6c7aa, 20)
            ROUND_OP(G, a, b, c, d, (ctx->block[5]), 0xd62f105d, 5)
            ROUND_OP(G, d, a, b, c, (ctx->block[10]), 0x02441453, 9)
            ROUND_OP(G, c, d, a, b, (ctx->block[15]), 0xd8a1e681, 14)
            ROUND_OP(G, b, c, d, a, (ctx->block[4]), 0xe7d3fbc8, 20)
            ROUND_OP(G, a, b, c, d, (ctx->block[9]), 0x21e1cde6, 5)
            ROUND_OP(G, d, a, b, c, (ctx->block[14]), 0xc33707d6, 9)
            ROUND_OP(G, c, d, a, b, (ctx->block[3]), 0xf4d50d87, 14)
            ROUND_OP(G, b, c, d, a, (ctx->block[8]), 0x455a14ed, 20)
            ROUND_OP(G, a, b, c, d, (ctx->block[13]), 0xa9e3e905, 5)
            ROUND_OP(G, d, a, b, c, (ctx->block[2]), 0xfcefa3f8, 9)
            ROUND_OP(G, c, d, a, b, (ctx->block[7]), 0x676f02d9, 14)
            ROUND_OP(G, b, c, d, a, (ctx->block[12]), 0x8d2a4c8a, 20)

            /* Round 3 */
            ROUND_OP(H, a, b, c, d, (ctx->block[5]), 0xfffa3942, 4)
            ROUND_OP(H, d, a, b, c, (ctx->block[8]), 0x8771f681, 11)
            ROUND_OP(H, c, d, a, b, (ctx->block[11]), 0x6d9d6122, 16)
            ROUND_OP(H, b, c, d, a, (ctx->block[14]), 0xfde5380c, 23)
            ROUND_OP(H, a, b, c, d, (ctx->block[1]), 0xa4beea44, 4)
            ROUND_OP(H, d, a, b, c, (ctx->block[4]), 0x4bdecfa9, 11)
            ROUND_OP(H, c, d, a, b, (ctx->block[7]), 0xf6bb4b60, 16)
            ROUND_OP(H, b, c, d, a, (ctx->block[10]), 0xbebfbc70, 23)
            ROUND_OP(H, a, b, c, d, (ctx->block[13]), 0x289b7ec6, 4)
            ROUND_OP(H, d, a, b, c, (ctx->block[0]), 0xeaa127fa, 11)
            ROUND_OP(H, c, d, a, b, (ctx->block[3]), 0xd4ef3085, 16)
            ROUND_OP(H, b, c, d, a, (ctx->block[6]), 0x04881d05, 23)
            ROUND_OP(H, a, b, c, d, (ctx->block[9]), 0xd9d4d039, 4)
            ROUND_OP(H, d, a, b, c, (ctx->block[12]), 0xe6db99e5, 11)
            ROUND_OP(H, c, d, a, b, (ctx->block[15]), 0x1fa27cf8, 16)
            ROUND_OP(H, b, c, d, a, (ctx->block[2]), 0xc4ac5665, 23)

            /* Round 4 */
            ROUND_OP(I, a, b, c, d, (ctx->block[0]), 0xf4292244, 6)
            ROUND_OP(I, d, a, b, c, (ctx->block[7]), 0x432aff97, 10)
            ROUND_OP(I, c, d, a, b, (ctx->block[14]), 0xab9423a7, 15)
            ROUND_OP(I, b, c, d, a, (ctx->block[5]), 0xfc93a039, 21)
            ROUND_OP(I, a, b, c, d, (ctx->block[12]), 0x655b59c3, 6)
            ROUND_OP(I, d, a, b, c, (ctx->block[3]), 0x8f0ccc92, 10)
            ROUND_OP(I, c, d, a, b, (ctx->block[10]), 0xffeff47d, 15)
            ROUND_OP(I, b, c, d, a, (ctx->block[1]), 0x85845dd1, 21)
            ROUND_OP(I, a, b, c, d, (ctx->block[8]), 0x6fa87e4f, 6)
            ROUND_OP(I, d, a, b, c, (ctx->block[15]), 0xfe2ce6e0, 10)
            ROUND_OP(I, c, d, a, b, (ctx->block[6]), 0xa3014314, 15)
            ROUND_OP(I, b, c, d, a, (ctx->block[13]), 0x4e0811a1, 21)
            ROUND_OP(I, a, b, c, d, (ctx->block[4]), 0xf7537e82, 6)
            ROUND_OP(I, d, a, b, c, (ctx->block[11]), 0xbd3af235, 10)
            ROUND_OP(I, c, d, a, b, (ctx->block[2]), 0x2ad7d2bb, 15)
            ROUND_OP(I, b, c, d, a, (ctx->block[9]), 0xeb86d391, 21)

            a += aa;
            b += bb;
            c += cc;
            d += dd;

            ptr += 64;
        } while (size -= 64);

        ctx->a = a;
        ctx->b = b;
        ctx->c = c;
        ctx->d = d;

        return ptr;
    }

    void MD5Init(md5_context *ctx)
    {
        ctx->a = 0x67452301;
        ctx->b = 0xefcdab89;
        ctx->c = 0x98badcfe;
        ctx->d = 0x10325476;

        ctx->lo = 0;
        ctx->hi = 0;
    }

    void MD5Update(md5_context *ctx, const void *data, size_t size)
    {
        int32_t saved_lo;
        uint32_t used, free;

        saved_lo = ctx->lo;
        if ((ctx->lo = (saved_lo + size) & 0x1fffffff) < saved_lo) {
            ctx->hi++;
        }
        ctx->hi += size >> 29;

        used = saved_lo & 0x3f;

        if (used) {
            free = 64 - used;

            if (size < free) {
                memcpy(&ctx->buffer[used], data, size);
                return;
            }

            memcpy(&ctx->buffer[used], data, free);
            data  = (unsigned char *) data + free;
            size -= free;
            body(ctx, ctx->buffer, 64);
        }

        if (size >= 64) {
            data  = body(ctx, data, size & ~(size_t) 0x3f);
            size &= 0x3f;
        }

        memcpy(ctx->buffer, data, size);
    }

    void MD5Final(unsigned char *result, md5_context *ctx)
    {
        uint32_t used, free;

        used                = ctx->lo & 0x3f;
        ctx->buffer[used++] = 0x80;
        free                = 64 - used;

        if (free < 8) {
            memset(&ctx->buffer[used], 0, free);
            body(ctx, ctx->buffer, 64);
            used = 0;
            free = 64;
        }

        memset(&ctx->buffer[used], 0, free - 8);

        ctx->lo         <<= 3;
        ctx->buffer[56]   = ctx->lo;
        ctx->buffer[57]   = ctx->lo >> 8;
        ctx->buffer[58]   = ctx->lo >> 16;
        ctx->buffer[59]   = ctx->lo >> 24;
        ctx->buffer[60]   = ctx->hi;
        ctx->buffer[61]   = ctx->hi >> 8;
        ctx->buffer[62]   = ctx->hi >> 16;
        ctx->buffer[63]   = ctx->hi >> 24;

        body(ctx, ctx->buffer, 64);

        result[0]  = ctx->a;
        result[1]  = ctx->a >> 8;
        result[2]  = ctx->a >> 16;
        result[3]  = ctx->a >> 24;
        result[4]  = ctx->b;
        result[5]  = ctx->b >> 8;
        result[6]  = ctx->b >> 16;
        result[7]  = ctx->b >> 24;
        result[8]  = ctx->c;
        result[9]  = ctx->c >> 8;
        result[10] = ctx->c >> 16;
        result[11] = ctx->c >> 24;
        result[12] = ctx->d;
        result[13] = ctx->d >> 8;
        result[14] = ctx->d >> 16;
        result[15] = ctx->d >> 24;

        memset(ctx, 0, sizeof(*ctx));
    }

    char *md5(const char *string, bool binary = false)
    {
        md5_context context;
        unsigned char *hash = (unsigned char *) malloc(16 * sizeof(unsigned char));

        MD5Init(&context);
        MD5Update(&context, string, strlen(string));
        MD5Final(hash, &context);

        if (binary) {
            return (char *) hash;
        }

        char *hexHash = (char *) malloc(33 * sizeof(char));

        static const char hexChars[17] = "0123456789abcdef";

        for (int i = 0; i < 16; ++i) {
            hexHash[i * 2]       = hexChars[hash[i] >> 4];
            hexHash[(i * 2) + 1] = hexChars[hash[i] & 0x0F];
        }

        hexHash[16 * 2] = '\0';
        free(hash);

        return hexHash;
    }
}; // namespace Hash

#undef F
#undef G
#undef H
#undef I
#undef ROUND_OP
#undef SET_BLOCK

#endif