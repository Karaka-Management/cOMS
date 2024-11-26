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
int32 atomic_get(volatile int32* value)
{
    return (int32) InterlockedCompareExchange((long *) value, 0, 0);
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

#endif