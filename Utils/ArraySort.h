/**
 * Jingga
 *
 * @package   Utils
 * @copyright Dennis Eichhorn
 * @license   OMS License 1.0
 * @version   1.0.0
 * @link      https://jingga.app
 */
#ifndef UTILS_ARRAY_SORT_H
#define UTILS_ARRAY_SORT_H

#include <stdio.h>
#include <stdlib.h>

namespace Utils::ArraySort
{
    inline
    void reverse_int(int64_t *arr, size_t size)
    {
        for (size_t low = 0, high = size - 1; low < high; ++low, --high) {
            int64_t tmp   = arr[low];
            arr[low]  = arr[high];
            arr[high] = tmp;
        }
    }

    inline
    void reverse_float(float *arr, size_t size)
    {
        for (size_t low = 0, high = size - 1; low < high; ++low, --high) {
            float tmp = arr[low];
            arr[low]  = arr[high];
            arr[high] = tmp;
        }
    }

    inline
    void reverse_double(double *arr, size_t size)
    {
        for (size_t low = 0, high = size - 1; low < high; ++low, --high) {
            double tmp = arr[low];
            arr[low]   = arr[high];
            arr[high]  = tmp;
        }
    }

    inline
    void reverse_char(char **arr, size_t size)
    {
        for (size_t low = 0, high = size - 1; low < high; ++low, --high) {
            char* tmp = arr[low];
            arr[low]  = arr[high];
            arr[high] = tmp;
        }
    }
}

#endif