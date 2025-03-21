/**
 * Jingga
 *
 * @copyright Jingga
 * @license   OMS License 2.0
 * @version   1.0.0
 * @link      https://jingga.app
 */
#ifndef COMS_PLATFORM_LINUX_TIME_UTILS_H
#define COMS_PLATFORM_LINUX_TIME_UTILS_H

#include <time.h>
#include <sys/time.h>
#include "../../stdlib/Types.h"

uint64 system_time() {
    struct timeval tv;
    gettimeofday(&tv, NULL);

    return (uint64) tv.tv_sec * 1000000ULL + (uint64) tv.tv_usec;
}

uint64 time_mu() {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);

    return (uint64) ts.tv_sec * 1000000ULL + (uint64) (ts.tv_nsec / 1000);
}

#endif