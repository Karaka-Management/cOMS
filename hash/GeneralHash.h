/**
 * Jingga
 *
 * @copyright Jingga
 * @license   OMS License 2.0
 * @version   1.0.0
 * @link      https://jingga.app
 */
#ifndef TOS_HASH_GENERAL_H
#define TOS_HASH_GENERAL_H

#include "../stdlib/Types.h"

inline constexpr
uint64 hash_djb2(const char* key) {
    uint64 hash = 5381;
    int32 c;

    while ((c = *key++)) {
        hash = ((hash << 5) + hash) + c;
    }

    return hash;
}

inline
uint64 hash_sdbm(const byte* key)
{
    uint64 hash = 0;
    int32 c;

    while (c = *key++) {
        hash = c + (hash << 6) + (hash << 16) - hash;
    }

    return hash;
}

inline
uint64 hash_lose_lose(const byte* key)
{
	uint64 hash = 0;
	int32 c;

	while (c = *key++) {
	    hash += c;
    }

	return hash;
}

inline
uint64 hash_polynomial_rolling(const char* str)
{
    const int32 p = 31;
    const int32 m = 1000000009;
    uint64 hash = 0;
    uint64 p_pow = 1;

    while (*str) {
        hash = (hash + (*str - 'a' + 1) * p_pow) % m;
        p_pow = (p_pow * p) % m;
        str++;
    }

    return hash;
}

inline
uint64 hash_fnv1a(const char* str)
{
    const uint64 FNV_OFFSET_BASIS = 14695981039346656037UL;
    const uint64 FNV_PRIME = 1099511628211UL;
    uint64 hash = FNV_OFFSET_BASIS;

    while (*str) {
        hash ^= (byte) *str;
        hash *= FNV_PRIME;
        str++;
    }

    return hash;
}

inline
uint32 hash_oat(const char* str)
{
    uint32 hash = 0;

    while(*str) {
        hash += *str++;
        hash += (hash << 10);
        hash ^= (hash >> 6);
    }

    hash += (hash << 3);
    hash ^= (hash >> 11);
    hash += (hash << 15);

    return hash;
}

inline
uint32 hash_ejb(const char* str)
{
	const uint32 PRIME1 = 37;
	const uint32 PRIME2 = 1048583;
	uint32 hash = 0;

	while (*str) {
		hash = hash * PRIME1 ^ (*str++ - ' ');
	}

	return hash % PRIME2;
}

////////////////////////////////////
// Seeded hash functions
////////////////////////////////////

inline constexpr
uint64 hash_djb2_seeded(const char* key, int32 seed)
{
    uint64 hash = 5381;
    int32 c;

    while ((c = *key++)) {
        hash = ((hash << 5) + hash) + c;
    }

    return hash ^ (seed + (seed << 6) + (seed >> 2));
}

inline
uint64 hash_sdbm_seeded(const char* key, int32 seed)
{
    uint64 hash = 0;
    int32 c;

    while (c = *key++) {
        hash = c + (hash << 6) + (hash << 16) - hash;
    }

    return hash ^ (seed + (seed << 6) + (seed >> 2));
}

inline
uint64 hash_lose_lose_seeded(const char* key, int32 seed)
{
	uint64 hash = 0;
	int32 c;

	while (c = *key++) {
	    hash += c;
    }

	return hash ^ (seed + (seed << 6) + (seed >> 2));
}

inline
uint64 hash_polynomial_rolling_seeded(const char* str, int32 seed)
{
    const int32 p = 31;
    const int32 m = 1000000009;
    uint64 hash = 0;
    uint64 p_pow = 1;

    while (*str) {
        hash = (hash + (*str - 'a' + 1) * p_pow) % m;
        p_pow = (p_pow * p) % m;
        str++;
    }

    return hash ^ (seed + (seed << 6) + (seed >> 2));
}

inline
uint64 hash_fnv1a_seeded(const char* str, int32 seed)
{
    const uint64 FNV_OFFSET_BASIS = 14695981039346656037UL;
    const uint64 FNV_PRIME = 1099511628211UL;
    uint64 hash = FNV_OFFSET_BASIS;

    while (*str) {
        hash ^= (byte) *str;
        hash *= FNV_PRIME;
        str++;
    }

    return hash ^ (seed + (seed << 6) + (seed >> 2));
}

inline
uint64 hash_oat_seeded(const char* str, int32 seed)
{
    uint64 hash = 0;

    while(*str) {
        hash += *str++;
        hash += (hash << 10);
        hash ^= (hash >> 6);
    }

    hash += (hash << 3);
    hash ^= (hash >> 11);
    hash += (hash << 15);

    return hash ^ (seed + (seed << 6) + (seed >> 2));;
}

inline
uint64 hash_ejb_seeded(const char* str, int32 seed)
{
	const uint64 PRIME1 = 37;
	const uint64 PRIME2 = 1048583;
	uint64 hash = 0;

	while (*str) {
		hash = hash * PRIME1 ^ (*str++ - ' ');
	}

	return (hash % PRIME2) ^ (seed + (seed << 6) + (seed >> 2));;
}

#endif