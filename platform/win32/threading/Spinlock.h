/**
 * Jingga
 *
 * @copyright Jingga
 * @license   OMS License 2.0
 * @version   1.0.0
 * @link      https://jingga.app
 */
#ifndef TOS_PLATFORM_WIN32_THREADING_SPINLOCK_H
#define TOS_PLATFORM_WIN32_THREADING_SPINLOCK_H

#include <windows.h>

typedef volatile long spinlock32;

inline
void spinlock_start(spinlock32* lock) {
    while (InterlockedExchange(lock, 1) == 1) {}
}

inline
void spinlock_end(spinlock32* lock) {
    InterlockedExchange(lock, 0);
}

#endif