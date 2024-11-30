/**
 * Jingga
 *
 * @copyright Jingga
 * @license   OMS License 2.0
 * @version   1.0.0
 * @link      https://jingga.app
 */
#ifndef TOS_PLATFORM_WIN32_THREADING_SPINLOCK_C
#define TOS_PLATFORM_WIN32_THREADING_SPINLOCK_C

#include <windows.h>
#include "../TimeUtils.h"

inline
void spinlock_start(spinlock32* lock, int32 delay = 10) {
    while (InterlockedExchange(lock, 1) != 0) {
        usleep(delay);
    }
}

inline
void spinlock_end(spinlock32* lock) {
    InterlockedExchange(lock, 0);
}

#endif