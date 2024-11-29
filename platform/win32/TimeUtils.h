/**
 * Jingga
 *
 * @copyright Jingga
 * @license   OMS License 2.0
 * @version   1.0.0
 * @link      https://jingga.app
 */
#ifndef TOS_PLATFORM_WIN32_TIME_UTILS_H
#define TOS_PLATFORM_WIN32_TIME_UTILS_H

#include <stdio.h>
#include <windows.h>
#include <time.h>
#include "../../log/Debug.cpp"

void usleep(uint64 microseconds)
{
    if ((microseconds % 1000) == 0) {
        Sleep((DWORD) (microseconds / 1000));
        return;
    }

    LARGE_INTEGER frequency;
    QueryPerformanceFrequency(&frequency);

    LARGE_INTEGER start, end;
    QueryPerformanceCounter(&start);
    long long target = start.QuadPart + (microseconds * frequency.QuadPart) / 1000000;

    do {
        QueryPerformanceCounter(&end);
    } while (end.QuadPart < target);
}

inline
time_t system_time()
{
    SYSTEMTIME systemTime;
    FILETIME fileTime;
    ULARGE_INTEGER largeInt;

    GetLocalTime(&systemTime);
    SystemTimeToFileTime(&systemTime, &fileTime);

    // Convert FILETIME to a 64-bit integer
    largeInt.LowPart = fileTime.dwLowDateTime;
    largeInt.HighPart = fileTime.dwHighDateTime;

    return ((time_t) (largeInt.QuadPart / 10000000ULL)) - ((time_t) 11644473600ULL);
}

// doesn't return clock time, only to return time since program start
// -> can be used for profiling
inline
uint64 time_mu()
{
    LARGE_INTEGER counter;
    QueryPerformanceCounter(&counter);

    return (counter.QuadPart * 1000000) / debug_container->performance_count_frequency;
}

inline
time_t unix_epoch_s()
{
    FILETIME ft;
    GetSystemTimeAsFileTime(&ft);

    ULARGE_INTEGER li;
    li.LowPart = ft.dwLowDateTime;
    li.HighPart = ft.dwHighDateTime;

    time_t seconds_since_epoch = (li.QuadPart - 116444736000000000ULL) / 10000000ULL;

    return seconds_since_epoch;
}

static DWORD timespec_to_ms(const timespec* abstime)
{
    if (abstime == NULL) {
        return INFINITE;
    }

    time_t seconds_since_epoch = unix_epoch_s();
    DWORD t = (DWORD) (((abstime->tv_sec - seconds_since_epoch) * 1000) + (abstime->tv_nsec / 1000000));

    return t < 0 ? 1 : t;
}

#endif