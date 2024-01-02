/**
 * Jingga
 *
 * @package   Utils
 * @copyright Dennis Eichhorn
 * @license   OMS License 1.0
 * @version   1.0.0
 * @link      https://jingga.app
 */
#ifndef UTILS_ARRAY_UTILS_H
#define UTILS_ARRAY_UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "StringUtils.h"

namespace Utils::ArrayUtils
{
    inline
    const char* get_arg(const char *id, const char **argv, size_t size)
    {
        if (Utils::StringUtils::is_number(id)) {
            return argv[atoi(id)];
        }

        for (size_t i = 0; i < size - 1; ++i) {
            if (strcmp(id, argv[i]) == 0) {
                return i + 1 >= size ? NULL : argv[i + 1];
            }
        }

        return NULL;
    }

    inline
    bool has_arg(const char *id, const char **argv, size_t size)
    {
        for (size_t i = 0; i < size; ++i) {
            if (strcmp(id, argv[i]) == 0) {
                return true;
            }
        }

        return false;
    }

    inline
    double array_sum_double(const double *array, size_t size)
    {
        double sum = 0.0;
        for (size_t i = 0; i < size; ++i) {
            sum += array[i];
        }

        return sum;
    }

    inline
    float array_sum_float(const float *array, size_t size)
    {
        float sum = 0.0;
        for (size_t i = 0; i < size; ++i) {
            sum += array[i];
        }

        return sum;
    }

    inline
    int64_t array_sum_int(const int64_t *array, size_t size)
    {
        int64_t sum = 0;
        for (size_t i = 0; i < size; ++i) {
            sum += array[i];
        }

        return sum;
    }

    inline
    size_t find_in_array_string(const char *element, const char **array, size_t size)
    {
        for (size_t i = 0; i < size; ++i) {
            if (strcmp(element, array[i]) == 0) {
                return i;
            }
        }

        return -1;
    }

    inline
    size_t find_in_array_double(double element, const double *array, size_t size)
    {
        for (size_t i = 0; i < size; ++i) {
            if (array[i] == element) {
                return i;
            }
        }

        return -1;
    }

    inline
    size_t find_in_array_float(float element, const float *array, size_t size)
    {
        for (size_t i = 0; i < size; ++i) {
            if (array[i] == element) {
                return i;
            }
        }

        return -1;
    }

    inline
    size_t find_in_array_int(int64_t element, const int64_t *array, size_t size)
    {
        for (size_t i = 0; i < size; ++i) {
            if (array[i] == element) {
                return i;
            }
        }

        return -1;
    }

    inline
    double* merge_arrays_double(const double* array1, size_t size1, const double* array2, size_t size2)
    {
        double* merged = (double*) malloc((size1 + size2) * sizeof(double));
        if (merged == NULL) {
            return NULL;
        }

        for (size_t i = 0; i < size1; ++i) {
            merged[i] = array1[i];
        }

        for (size_t i = 0; i < size2; ++i) {
            merged[size1 + i] = array2[i];
        }

        return merged;
    }

    inline
    float* merge_arrays_float(const float* array1, size_t size1, const float* array2, size_t size2)
    {
        float* merged = (float*) malloc((size1 + size2) * sizeof(float));
        if (merged == NULL) {
            return NULL;
        }

        for (size_t i = 0; i < size1; ++i) {
            merged[i] = array1[i];
        }

        for (size_t i = 0; i < size2; ++i) {
            merged[size1 + i] = array2[i];
        }

        return merged;
    }

    inline
    int64_t* merge_arrays_int(const int64_t* array1, size_t size1, const int64_t* array2, size_t size2)
    {
        int64_t* merged = (int64_t*) malloc((size1 + size2) * sizeof(int64_t));
        if (merged == NULL) {
            return NULL;
        }

        for (size_t i = 0; i < size1; ++i) {
            merged[i] = array1[i];
        }

        for (size_t i = 0; i < size2; ++i) {
            merged[size1 + i] = array2[i];
        }

        return merged;
    }

    inline
    char** merge_arrays_char(const char** array1, size_t size1, const char** array2, size_t size2)
    {
        char** merged = (char**) malloc((size1 + size2) * sizeof(char*));
        if (merged == NULL) {
            return NULL;
        }

        for (size_t i = 0; i < size1; ++i) {
            merged[i] = (char*) malloc((strlen(array1[i]) + 1) * sizeof(char));
            strcpy(merged[i], array1[i]);
        }

        for (size_t i = 0; i < size2; ++i) {
            merged[i] = (char*) malloc((strlen(array2[i]) + 1) * sizeof(char));
            strcpy(merged[i], array2[i]);
        }

        return merged;
    }
}

#endif