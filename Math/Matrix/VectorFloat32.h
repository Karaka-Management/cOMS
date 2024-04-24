/**
 * Karaka
 *
 * @package   Stdlib
 * @copyright Dennis Eichhorn
 * @license   OMS License 1.0
 * @version   1.0.0
 * @link      https://jingga.app
 */
#ifndef MATH_MATRIX_VECTORFLOAT32_H
#define MATH_MATRIX_VECTORFLOAT32_H

namespace Math::Matrix::VectorFloat32
{
    struct v3_f32_4_simd {
        union {
            struct {
                union {
                    f32_4_simd x;
                    f32_4_simd r;
                };
                union {
                    f32_4_simd y;
                    f32_4_simd g;
                };
                union {
                    f32_4_simd z;
                    f32_4_simd b;
                };
            };

            f32_4_simd v[3];
        };
    };

    struct v3_f32_8_simd {
        union {
            struct {
                union {
                    f32_8_simd x;
                    f32_8_simd r;
                };
                union {
                    f32_8_simd y;
                    f32_8_simd g;
                };
                union {
                    f32_8_simd z;
                    f32_8_simd b;
                };
            };

            f32_8_simd v[3];
        };
    };

    struct v3_f32_16_simd {
        union {
            struct {
                union {
                    f32_16_simd x;
                    f32_16_simd r;
                };
                union {
                    f32_16_simd y;
                    f32_16_simd g;
                };
                union {
                    f32_16_simd z;
                    f32_16_simd b;
                };
            };

            f32_16_simd v[3];
        };
    };

    struct v4_f32_4_simd {
        union {
            struct {
                union {
                    f32_4_simd x;
                    f32_4_simd r;
                };
                union {
                    f32_4_simd y;
                    f32_4_simd g;
                };
                union {
                    f32_4_simd z;
                    f32_4_simd b;
                };
                union {
                    f32_4_simd w;
                    f32_4_simd a;
                };
            };

            f32_4_simd v[4];
        };
    };

    struct v4_f32_8_simd {
        union {
            struct {
                union {
                    f32_8_simd x;
                    f32_8_simd r;
                };
                union {
                    f32_8_simd y;
                    f32_8_simd g;
                };
                union {
                    f32_8_simd z;
                    f32_8_simd b;
                };
                union {
                    f32_8_simd w;
                    f32_8_simd a;
                };
            };

            f32_8_simd v[4];
        };
    };

    struct v4_f32_16_simd {
        union {
            struct {
                union {
                    f32_16_simd x;
                    f32_16_simd r;
                };
                union {
                    f32_16_simd y;
                    f32_16_simd g;
                };
                union {
                    f32_16_simd z;
                    f32_16_simd b;
                };
                union {
                    f32_16_simd w;
                    f32_16_simd a;
                };
            };

            f32_16_simd v[4];
        };
    };
} // namespace Math::Matrix::VectorFloat32

#endif
