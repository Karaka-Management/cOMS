#ifndef TOS_SORT_BINARY_SEARCH_H
#define TOS_SORT_BINARY_SEARCH_H

#include "../stdlib/Types.h"

// WARNING: the prefetching is usually only useful, if we go into 512KB array size
/*
int32 lower_bound(int32* t, size_t len, int32 x) {
    int32 *base = t;
    while (len > 1) {
        int32 half = len / 2;
        len -= half;

        intrin_prefetch(&base[len / 2 - 1]);
        intrin_prefetch(&base[half + len / 2 - 1]);

        base += (base[half - 1] < x) * half;
    }
    return *base;
}
*/

#endif