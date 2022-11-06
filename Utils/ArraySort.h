/**
 * Karaka
 *
 * @package   Utils
 * @copyright Dennis Eichhorn
 * @license   OMS License 1.0
 * @version   1.0.0
 * @link      https://karaka.app
 */
#ifndef UTILS_ARRAY_SORT_H
#define UTILS_ARRAY_SORT_H

#include <stdio.h>
#include <stdlib.h>

namespace Utils
{
    namespace ArraySort
    {
        inline
        void reverse_int(int *arr, int size)
        {
            for (int low = 0, high = size - 1; low < high; ++low, --high) {
                int tmp   = arr[low];
                arr[low]  = arr[high];
                arr[high] = tmp;
            }
        }

        inline
        void reverse_float(float *arr, int size)
        {
            for (int low = 0, high = size - 1; low < high; ++low, --high) {
                float tmp   = arr[low];
                arr[low]  = arr[high];
                arr[high] = tmp;
            }
        }

        inline
        void reverse_double(double *arr, int size)
        {
            for (int low = 0, high = size - 1; low < high; ++low, --high) {
                double tmp   = arr[low];
                arr[low]  = arr[high];
                arr[high] = tmp;
            }
        }

        inline
        void reverse_char(char **arr, int size)
        {
            for (int low = 0, high = size - 1; low < high; ++low, --high) {
                char* tmp = arr[low];
                arr[low]  = arr[high];
                arr[high] = tmp;
            }
        }
    }
}

#endif