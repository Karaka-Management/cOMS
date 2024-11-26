/**
 * Jingga
 *
 * @copyright Jingga
 * @license   OMS License 2.0
 * @version   1.0.0
 * @link      https://jingga.app
 */
#ifndef TOS_PLATFORM_LINUX_THREADING_SPINLOCK_H
#define TOS_PLATFORM_LINUX_THREADING_SPINLOCK_H

#include <pthread.h>
#include "../../../stdlib/Types.h"

typedef volatile int32 spinlock32;

inline
void spinlock_start(spinlock32* lock) {
    while (__atomic_exchange_n(lock, 1, __ATOMIC_ACQUIRE)) {}
}

inline
void spinlock_end(spinlock32* lock) {
    __atomic_store_n(lock, 0, __ATOMIC_RELEASE);
}

#endif