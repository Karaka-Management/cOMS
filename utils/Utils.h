/**
 * Jingga
 *
 * @copyright Jingga
 * @license   OMS License 2.0
 * @version   1.0.0
 * @link      https://jingga.app
 */
#ifndef TOS_UTILS_H
#define TOS_UTILS_H

#include "../stdlib/Types.h"

#define sizeof_array(a) (sizeof(a) / sizeof((a)[0]))

struct FileBody {
    uint64 size = 0; // doesn't include null termination (same as strlen)
    byte* content;
};

global_persist uint32 fast_seed;
#define FAST_RAND_MAX 32767

inline
uint32 fast_rand1(void) {
    fast_seed = (214013 * fast_seed + 2531011);

    return (fast_seed >> 16) & 0x7FFF;
}

uint32 fast_rand2(uint32* state) {
    uint32 x = *state;

    x ^= x << 13;
    x ^= x >> 17;
    x ^= x << 5;

    *state = x;

    return x;
}

inline
f32 fast_rand_percentage(void) {
    return (f32) fast_rand1() / (f32) FAST_RAND_MAX;
}

inline
bool is_bit_set(byte data, int bit)
{
    return data & (1 << bit);
}

inline
bool is_bit_set(int data, int bit)
{
    return data & (1 << bit);
}

inline
bool is_bit_set(uint32 data, int bit)
{
    return data & (1 << bit);
}

inline
byte get_bits(byte data, int bits_to_read, int start_pos)
{
    byte mask = (1 << bits_to_read) - 1;
    return (data >> (8 - start_pos - bits_to_read)) & mask;
}

inline
uint64 get_bits(const byte* data, int bits_to_read, int start_pos)
{
    if (bits_to_read <= 0 || bits_to_read > sizeof(uint64)) {
        return 0;
    }

    int byte_index = start_pos / 8;
    int bit_offset = start_pos % 8;

    uint64_t mask = (1ULL << bits_to_read) - 1;
    uint64_t result = 0;

    int bits_read = 0;

    while (bits_read < bits_to_read) {
        int bits_in_current_byte = 8 - bit_offset;
        int bits_to_take = bits_to_read - bits_read;

        if (bits_to_take > bits_in_current_byte) {
            bits_to_take = bits_in_current_byte;
        }

        uint8_t current_byte = data[byte_index];
        current_byte >>= bit_offset;
        current_byte &= (1 << bits_to_take) - 1;

        result |= ((uint64_t)current_byte << bits_read);

        bits_read += bits_to_take;
        bit_offset = 0;
        byte_index++;
    }

    result &= mask;

    return result;
}

inline
uint32 reverse_bits(uint32 data, uint32 count)
{
    uint32 reversed = 0;
    for (uint32 i = 0; i <= (count / 2); ++i) {
        uint32 inv = count - i - 1;
        reversed |= ((data >> i) & 0x1) << inv;
        reversed |= ((data >> inv) & 0x1) << i;
    }

    return reversed;
}

/**
 * Picks n random elements from end and stores them in begin.
 */
inline
void random_unique(int* array, int size) {
    for (int i = size - 1; i > 0; --i) {
        int j = rand() % (i + 1);

        int temp = array[i];
        array[i] = array[j];
        array[j] = temp;
    }
}

/**
 * Gets random index based value probability
 */
int random_weighted_index(int* arr, int array_count)
{
    uint32 prob_sum = 0;
    for (int i = 0; i < array_count; ++i) {
        prob_sum += arr[i];
    }

    uint32 random_prob = rand() % (prob_sum + 1);
    uint32 current_rarity = 0;
    int item_rarity = array_count - 1;
    for (int i = 0; i < array_count - 1; ++i) {
        current_rarity += arr[i];

        if (current_rarity < random_prob) {
            item_rarity = i;
            break;
        }
    }

    return item_rarity;
}

#endif