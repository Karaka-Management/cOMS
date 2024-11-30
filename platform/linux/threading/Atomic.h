/**
 * Jingga
 *
 * @copyright Jingga
 * @license   OMS License 2.0
 * @version   1.0.0
 * @link      https://jingga.app
 */
#ifndef TOS_PLATFORM_LINUX_THREADING_ATOMIC_H
#define TOS_PLATFORM_LINUX_THREADING_ATOMIC_H

#include <stdatomic.h>
#include "../../../stdlib/Types.h"

inline
void atomic_set(volatile int32* value, int32 new_value)
{
    __atomic_store_n(value, new_value, __ATOMIC_SEQ_CST);
}

inline
void atomic_set(volatile int64* value, int64 new_value)
{
    __atomic_store_n(value, new_value, __ATOMIC_SEQ_CST);
}

inline
int32 atomic_set_fetch(volatile int32* value, int32 new_value) {
    return __atomic_exchange_n(value, new_value, __ATOMIC_SEQ_CST);
}

inline
int64 atomic_set_fetch(volatile int64* value, int64 new_value) {
    return __atomic_exchange_n(value, new_value, __ATOMIC_SEQ_CST);
}

inline
void atomic_get(volatile byte* value, byte data[16])
{
    __atomic_store((volatile __int128 *) value, (__int128 *) data, __ATOMIC_SEQ_CST);
}

inline
int32 atomic_get(volatile int32* value)
{
    return __atomic_load_n((int32 *) value, __ATOMIC_SEQ_CST);
}

inline
int64 atomic_get(volatile int64* value)
{
    return __atomic_load_n((int64 *) value, __ATOMIC_SEQ_CST);
}

inline
void atomic_get(volatile byte* value, byte data[16])
{
    __atomic_load((volatile __int128 *) value, (__int128 *) data, __ATOMIC_SEQ_CST);
}

inline
void atomic_increment(volatile int32* value) {
    __atomic_fetch_add(value, 1, __ATOMIC_SEQ_CST);
}

inline
void atomic_decrement(volatile int32* value) {
    __atomic_fetch_sub(value, 1, __ATOMIC_SEQ_CST);
}

inline
void atomic_add(volatile int32* value, int32 increment) {
    __atomic_fetch_add(value, increment, __ATOMIC_SEQ_CST);
}

inline
void atomic_sub(volatile int32* value, int32 decrement) {
    __atomic_fetch_sub(value, decrement, __ATOMIC_SEQ_CST);
}

inline
int32 atomic_compare_exchange_weak(volatile int32* value, int32* expected, int32 desired) {
    __atomic_compare_exchange_n(value, expected, desired, 0, __ATOMIC_SEQ_CST, __ATOMIC_SEQ_CST);
    return *expected;
}

inline
int32 atomic_fetch_add(volatile int32* value, int32 operand) {
    return __atomic_fetch_add(value, operand, __ATOMIC_SEQ_CST);
}

inline
int32 atomic_fetch_sub(volatile int32* value, int32 operand) {
    return __atomic_fetch_sub(value, operand, __ATOMIC_SEQ_CST);
}

#endif