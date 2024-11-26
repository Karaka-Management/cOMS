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

#include <pthread.h>
#include "../../../stdlib/Types.h"

inline
void atomic_set(volatile int32* value, int32 new_value)
{
    __atomic_store_n(value, new_value, __ATOMIC_SEQ_CST);
}

inline
int32 atomic_get(volatile int32* value)
{
    return __atomic_load_n((int32 *) value, __ATOMIC_SEQ_CST);
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
int32 atomic_add(volatile int32* value, int32 increment) {
    return __atomic_fetch_add(value, increment, __ATOMIC_SEQ_CST);
}

inline
int32 atomic_subtract(volatile int32* value, int32 decrement) {
    return __atomic_fetch_sub(value, decrement, __ATOMIC_SEQ_CST);
}

#endif