/**
 * Jingga
 *
 * @copyright Jingga
 * @license   OMS License 2.0
 * @version   1.0.0
 * @link      https://jingga.app
 */
#ifndef TOS_PLATFORM_WIN32_THREADING_SEMAPHORE_H
#define TOS_PLATFORM_WIN32_THREADING_SEMAPHORE_H

#include <windows.h>
#include "../../../stdlib/Types.h"

typedef HANDLE sem_t;

void sem_init(sem_t* semaphore, int32 value)
{
    *semaphore = CreateSemaphore(NULL, value, MAX_INT32, NULL);
}

void sem_destroy(sem_t* semaphore)
{
    CloseHandle(*semaphore);
}

// decrement if != 0, if = 0 wait
void sem_wait(sem_t* semaphore) {
    WaitForSingleObject(*semaphore, INFINITE);
}

// increment
void sem_post(sem_t* semaphore) {
    ReleaseSemaphore(*semaphore, 1, NULL);
}

#endif