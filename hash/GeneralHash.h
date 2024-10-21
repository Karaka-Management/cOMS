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

uint64 hash_djb2(const char* key) {
    uint64 hash = 5381;
    int32 c;

    while ((c = *key++)) {
        hash = ((hash << 5) + hash) + c;
    }

    return hash;
}

uint64 hash_sdbm(const byte* key)
{
    uint64 hash = 0;
    int32 c;

    while (c = *key++) {
        hash = c + (hash << 6) + (hash << 16) - hash;
    }

    return hash;
}

uint64 hash_lose_lose(const byte* key)
{
	uint64 hash = 0;
	int32 c;

	while (c = *key++) {
	    hash += c;
    }

	return hash;
}

uint64 hash_polynomial_rolling(const char* str) {
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

uint64 hash_fnv1a(const char* str) {
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

uint32 hash_oat(const char* str)
{
    uint32 h = 0;

    while(*str) {
        h += *str++;
        h += (h << 10);
        h ^= (h >> 6);
    }

    h += (h << 3);
    h ^= (h >> 11);
    h += (h << 15);

    return h;
}

uint32 hash_ejb(const char* str)
{
	const uint32 PRIME1 = 37;
	const uint32 PRIME2 = 1048583;
	uint32 h = 0;

	while (*str) {
		h = h * PRIME1 ^ (*str++ - ' ');
	}

	return h % PRIME2;
}

// CONSTEXPR

constexpr
uint64 hash_djb2_const(const char* key) {
    uint64 hash = 5381;
    int32 c;

    while ((c = *key++)) {
        hash = ((hash << 5) + hash) + c;
    }

    return hash;
}

constexpr
uint64 hash_sdbm_const(const byte* key)
{
    uint64 hash = 0;
    int32 c;

    while (c = *key++) {
        hash = c + (hash << 6) + (hash << 16) - hash;
    }

    return hash;
}

constexpr
uint64 hash_lose_lose_const(const byte* key)
{
	uint64 hash = 0;
	int32 c;

	while (c = *key++) {
	    hash += c;
    }

	return hash;
}

constexpr
uint64 hash_polynomial_rolling_const(const char* str) {
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

constexpr
uint64 hash_fnv1a_const(const char* str) {
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

constexpr
uint32 hash_oat_const(const char* str)
{
    uint32 h = 0;

    while(*str) {
        h += *str++;
        h += (h << 10);
        h ^= (h >> 6);
    }

    h += (h << 3);
    h ^= (h >> 11);
    h += (h << 15);

    return h;
}

constexpr
uint32 hash_ejb_const(const char* str)
{
	const uint32 PRIME1 = 37;
	const uint32 PRIME2 = 1048583;
	uint32 h = 0;

	while (*str) {
		h = h * PRIME1 ^ (*str++ - ' ');
	}

	return h % PRIME2;
}

#endif