/**
 * Jingga
 *
 * @copyright Jingga
 * @license   OMS License 2.0
 * @version   1.0.0
 * @link      https://jingga.app
 */
#ifndef TOS_PLATFORM_WIN32_THREADING_ATOMIC_H
#define TOS_PLATFORM_WIN32_THREADING_ATOMIC_H

#include <windows.h>
#include "../../../stdlib/Types.h"

inline
void atomic_set(volatile int32* value, int32 new_value)
{
    InterlockedExchange((long *) value, new_value);
}

inline
void atomic_set(volatile byte* value, const byte new_value[16])
{
    int64* value64 = (int64*) value;
    const int64* new_value64 = (const int64*) new_value;

    int64 expected_low, expected_high;

    do {
        expected_low = value64[0];
        expected_high = value64[1];
    } while (
        !InterlockedCompareExchange128(
            (volatile long long*) value,
            new_value64[1],
            new_value64[0],
            &expected_low
        )
    );
}

inline
int32 atomic_get(volatile int32* value)
{
    return (int32) InterlockedCompareExchange((long *) value, 0, 0);
}

inline
int64 atomic_get(volatile int64* value)
{
    return (int64) InterlockedCompareExchange((long *) value, 0, 0);
}

inline
void atomic_get(volatile byte* value, byte data[16])
{
    InterlockedCompareExchange128((volatile long long *) value, 0, 0, (long long *) data);
}

inline
void atomic_increment(volatile int32* value) {
    InterlockedIncrement((long *) value);
}

inline
void atomic_decrement(volatile int32* value) {
    InterlockedDecrement((long *) value);
}

inline
int32 atomic_add(volatile int32* value, int32 increment) {
    return InterlockedExchangeAdd((long *) value, increment);
}

inline
int32 atomic_subtract(volatile int32* value, int32 decrement) {
    return InterlockedExchangeAdd((long *) value, -decrement);
}

inline
int32 atomic_compare_exchange_weak(volatile int32* value, int32* expected, int32 desired) {
    return (int32) InterlockedCompareExchange((long *) value, desired, *expected);
}

inline
int32 atomic_fetch_add(volatile int32* value, int32 operand) {
    return (int32) InterlockedExchangeAdd((long *) value, operand);
}

inline
int32 atomic_fetch_sub(volatile int32* value, int32 operand) {
    return (int32) InterlockedExchangeSubtract((unsigned long *) value, operand);
}

#endif