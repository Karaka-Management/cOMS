#ifndef MATH_MATRIX_FLOAT32_H
#define MATH_MATRIX_FLOAT32_H

#include "../Stdlib/Types.h"

#define LOG_TYPE_STRING 0
#define LOG_TYPE_INT 1
#define LOG_TYPE_BOOL 2
#define LOG_TYPE_REF 3

struct GenericStringLogMessage {
    byte datatype = LOG_TYPE_STRING;
    uint32 type;
    char title[32];

    union {
        char val_str[128];
        int64 val_int;
        bool val_bool;
        void* val_ref;
    };
};

#endif