/**
 * Jingga
 *
 * @copyright Jingga
 * @license   OMS License 2.0
 * @version   1.0.0
 * @link      https://jingga.app
 */
#ifndef TOS_STDLIB_MATHTYPES_H
#define TOS_STDLIB_MATHTYPES_H

#include "Types.h"

// @todo Move to matrix

struct v2_int32 {
    union {
        struct {
            int32 x;
            int32 y;
        };

        struct {
            int32 width;
            int32 height;
        };

        int32 v[2];
    };
};

struct v3_int32 {
    union {
        struct {
            union {
                int32 x;
                int32 r;
            };
            union {
                int32 y;
                int32 g;
            };
            union {
                int32 z;
                int32 b;
            };
        };

        int32 v[3];
    };
};

struct v4_int32 {
    union {
        struct {
            int32 x;
            int32 y;
            int32 z;
            int32 w;
        };

        int32 v[4];
    };
};

struct v2_int64 {
    union {
        struct {
            int64 x;
            int64 y;
        };

        int64 v[2];
    };
};

struct v3_int64 {
    union {
        struct {
            union {
                int64 x;
                int64 r;
            };
            union {
                int64 y;
                int64 g;
            };
            union {
                int64 z;
                int64 b;
            };
        };

        int64 v[3];
    };
};

struct v4_int64 {
    union {
        struct {
            int64 x;
            int64 y;
            int64 z;
            int64 w;
        };

        int64 v[4];
    };
};

struct v2_f32 {
    union {
        struct {
            f32 x;
            f32 y;
        };

        f32 v[2];
    };
};

struct v3_f32 {
    union {
        struct {
            union {
                f32 x;
                f32 r;
                f32 pitch;
                f32 u;
            };
            union {
                f32 y;
                f32 g;
                f32 yaw;
                f32 v;
            };
            union {
                f32 z;
                f32 b;
                f32 roll;
                f32 w;
            };
        };

        f32 vec[3];
    };
};

struct v4_f32 {
    union {
        struct {
            f32 x;
            f32 y;
            f32 z;
            f32 w;
        };

        f32 vec[4];
    };
};

struct v2_f64 {
    union {
        struct {
            f64 x;
            f64 y;
        };

        f64 v[2];
    };
};

struct v3_f64 {
    union {
        struct {
            union {
                f64 x;
                f64 r;
            };
            union {
                f64 y;
                f64 g;
            };
            union {
                f64 z;
                f64 b;
            };
        };

        f64 v[3];
    };
};

struct v4_f64 {
    union {
        struct {
            f64 x;
            f64 y;
            f64 z;
            f64 w;
        };

        f64 v[4];
    };
};

struct m_int32 {
    int32 *e;
    size_t m, n;
};

struct m_int64 {
    int64 *e;
    size_t m, n;
};

struct m_f32 {
    f32 *e;
    size_t m, n;
};

struct m_f64 {
    f64 *e;
    size_t m, n;
};

#endif