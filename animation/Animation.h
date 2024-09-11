/**
 * Jingga
 *
 * @package   Utils
 * @copyright Jingga
 * @license   OMS License 2.0
 * @version   1.0.0
 * @link      https://jingga.app
 */
#ifndef TOS_ANIMATION_H
#define TOS_ANIMATION_H

#include "../utils/MathUtils.h"

#include "AnimationEaseType.h"

inline
float lerp(float a, float b, float t)
{
    return a + t * (b - a);
}

float smoothstep(float t) {
    return t * t * (3 - 2 * t);
}

float anim_ease(float t, AnimationEaseType type) {
    switch(type) {
        case ANIMATION_EASE_IN_SINE: {
                return anim_ease_in_sine(t);
            };
        case ANIMATION_EASE_OUT_SINE: {
                return anim_ease_out_sine(t);
            };
        case ANIMATION_EASE_IN_OUT_SINE: {
                return anim_ease_in_out_sine(t);
            };
        case ANIMATION_EASE_IN_QUAD: {
                return anim_ease_in_quad(t);
            };
        case ANIMATION_EASE_OUT_QUAD: {
                return anim_ease_out_quad(t);
            };
        case ANIMATION_EASE_IN_OUT_QUAD: {
                return anim_ease_in_out_quad(t);
            };
        case ANIMATION_EASE_IN_CUBIC: {
                return anim_ease_in_cubic(t);
            };
        case ANIMATION_EASE_OUT_CUBIC: {
                return anim_ease_out_cubic(t);
            };
        case ANIMATION_EASE_IN_OUT_CUBIC: {
                return anim_ease_in_out_cubic(t);
            };
        case ANIMATION_EASE_IN_QUART: {
                return anim_ease_in_quart(t);
            };
        case ANIMATION_EASE_OUT_QUART: {
                return anim_ease_out_quart(t);
            };
        case ANIMATION_EASE_IN_OUT_QUART: {
                return anim_ease_in_out_quart(t);
            };
        case ANIMATION_EASE_IN_QUINT: {
                return anim_ease_in_quint(t);
            };
        case ANIMATION_EASE_OUT_QUINT: {
                return anim_ease_out_quint(t);
            };
        case ANIMATION_EASE_IN_OUT_QUINT: {
                return anim_ease_in_out_quint(t);
            };
        case ANIMATION_EASE_IN_EXPO: {
                return anim_ease_in_expo(t);
            };
        case ANIMATION_EASE_OUT_EXPO: {
                return anim_ease_out_expo(t);
            };
        case ANIMATION_EASE_IN_OUT_EXPO: {
                return anim_ease_in_out_expo(t);
            };
        case ANIMATION_EASE_IN_CIRC: {
                return anim_ease_in_circ(t);
            };
        case ANIMATION_EASE_OUT_CIRC: {
                return anim_ease_out_circ(t);
            };
        case ANIMATION_EASE_IN_OUT_CIRC: {
                return anim_ease_in_out_circ(t);
            };
        case ANIMATION_EASE_IN_BACK: {
                return anim_ease_in_back(t);
            };
        case ANIMATION_EASE_OUT_BACK: {
                return anim_ease_out_back(t);
            };
        case ANIMATION_EASE_IN_OUT_BACK: {
                return anim_ease_in_out_back(t);
            };
        case ANIMATION_EASE_IN_ELASTIC: {
                return anim_ease_in_elastic(t);
            };
        case ANIMATION_EASE_OUT_ELASTIC: {
                return anim_ease_out_elastic(t);
            };
        case ANIMATION_EASE_IN_OUT_ELASTIC: {
                return anim_ease_in_out_elastic(t);
            };
        case ANIMATION_EASE_IN_BOUNCE: {
                return anim_ease_in_bounce(t);
            };
        case ANIMATION_EASE_OUT_BOUNCE: {
                return anim_ease_out_bounce(t);
            };
        case ANIMATION_EASE_IN_OUT_BOUNCE: {
                return anim_ease_in_out_bounce(t);
            };
        default:
            return t;
    }
}

inline
float anim_ease_linear(float t) {
    return t;
}

inline
float anim_ease_in_sine(float t) {
    return 1 - cosf_approx((t * OMS_PI) / 2);
}

inline
float anim_ease_out_sine(float t) {
    return sinf_approx((t * OMS_PI) / 2);
}

inline
float anim_ease_in_out_sine(float t) {
    return -(cosf_approx(OMS_PI * t) - 1) / 2;
}

inline
float anim_ease_in_quad(float t) {
    return t * t;
}

inline
float anim_ease_out_quad(float t) {
    return 1 - (1 - t) * (1 - t);
}

inline
float anim_ease_in_out_quad(float t) {
    return t < 0.5
        ? 2 * t * t
        : 1 - pow(-2 * t + 2, 2) / 2;
}

inline
float anim_ease_in_cubic(float t) {
    return t * t * t;
}

inline
float anim_ease_out_cubic(float t) {
    return 1 - pow(1 - t, 3);
}

inline
float anim_ease_in_out_cubic(float t) {
    return t < 0.5
        ? 4 * t * t * t
        : 1 - pow(-2 * t + 2, 3) / 2;
}

inline
float anim_ease_in_quart(float t) {
    return t * t * t * t;
}

inline
float anim_ease_out_quart(float t) {
    return 1 - pow(1 - t, 4);
}

inline
float anim_ease_in_perlin(float t) {
    return t * t * t * (t * (t * 6 - 15) + 10);
}

inline
float anim_ease_in_out_quart(float t) {
    return t < 0.5
        ? 8 * t * t * t * t
        : 1 - pow(-2 * t + 2, 4) / 2;
}

inline
float anim_ease_in_quint(float t) {
    return t * t * t * t * t;
}

inline
float anim_ease_out_quint(float t) {
    return 1 - pow(1 - t, 5);
}

inline
float anim_ease_in_out_quint(float t) {
    return t < 0.5
        ? 16 * t * t * t * t * t
        : 1 - pow(-2 * t + 2, 5) / 2;
}

inline
float anim_ease_in_expo(float t) {
    return t == 0
        ? 0 : pow(2, 10 * t - 10);
}

inline
float anim_ease_out_expo(float t) {
    return t == 1
        ? 1
        : 1 - pow(2, -10 * t);
}

inline
float anim_ease_in_out_expo(float t) {
    if (t == 0 || t == 1) {
        return t;
    }

    return t < 0.5
        ? pow(2, 20 * t - 10) / 2
        : (2 - pow(2, -20 * t + 10)) / 2;
}

inline
float anim_ease_in_circ(float t) {
    return 1 - sqrt(1 - pow(t, 2));
}

inline
float anim_ease_out_circ(float t) {
    return sqrt(1 - pow(t - 1, 2));
}

inline
float anim_ease_in_out_circ(float t) {
    return t < 0.5
        ? (1 - sqrt(1 - pow(2 * t, 2))) / 2
        : (sqrt(1 - pow(-2 * t + 2, 2)) + 1) / 2;
}

inline
float anim_ease_in_back(float t) {
    const float c1 = 1.70158;
    const float c3 = c1 + 1;

    return c3 * t * t * t - c1 * t * t;
}

inline
float anim_ease_out_back(float t) {
    const float c1 = 1.70158;
    const float c3 = c1 + 1;

    return 1 + c3 * pow(t - 1, 3) + c1 * pow(t - 1, 2);
}

inline
float anim_ease_in_out_back(float t) {
    const float c1 = 1.70158;
    const float c2 = c1 * 1.525;

    return t < 0.5
        ? (pow(2 * t, 2) * ((c2 + 1) * 2 * t - c2)) / 2
        : (pow(2 * t - 2, 2) * ((c2 + 1) * (t * 2 - 2) + c2) + 2) / 2;
}

inline
float anim_ease_in_elastic(float t) {
    const float c4 = (2 * OMS_PI) / 3;

    if (t == 0 || t == 1) {
        return t;
    }

    return -pow(2, 10 * t - 10) * sinf_approx((t * 10 - 10.75) * c4);
}

inline
float anim_ease_out_elastic(float t) {
    const float c4 = (2 * OMS_PI) / 3;

    if (t == 0.0 || t == 1.0) {
        return t;
    }

    return pow(2, -10 * t) * sinf_approx((t * 10 - 0.75) * c4) + 1;
}

inline
float anim_ease_in_out_elastic(float t) {
    const float c5 = (2 * OMS_PI) / 4.5;

    if (t == 0.0 || t == 1.0) {
        return t;
    } else if (t < 0.5) {
        return -(pow(2, 20 * t - 10) * sinf_approx((20 * t - 11.125) * c5)) / 2;
    }

    return (pow(2, -20 * t + 10) * sinf_approx((20 * t - 11.125) * c5)) / 2 + 1;
}

inline
float anim_ease_in_bounce(float t) {
    return 1 - anim_ease_out_bounce(1 - t);
}

inline
float anim_ease_out_bounce(float t) {
    const float n1 = 7.5625;
    const float d1 = 2.75;

    if (t < 1 / d1) {
        return n1 * t * t;
    } else if (t < 2 / d1) {
        return n1 * (t -= 1.5 / d1) * t + 0.75;
    } else if (t < 2.5 / d1) {
        return n1 * (t -= 2.25 / d1) * t + 0.9375;
    }

    return n1 * (t -= 2.625 / d1) * t + 0.984375;
}

inline
float anim_ease_in_out_bounce(float t) {
    return t < 0.5
        ? (1 - anim_ease_out_bounce(1 - 2 * t)) / 2
        : (1 + anim_ease_out_bounce(2 * t - 1)) / 2;
}

#endif