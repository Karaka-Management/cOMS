/**
 * Karaka
 *
 * @package   Image
 * @copyright Dennis Eichhorn
 * @license   OMS License 1.0
 * @version   1.0.0
 * @link      https://karaka.app
 */
#ifndef UTILS_MATH_UTILS_H
#define UTILS_MATH_UTILS_H

#define max(a, b) \
    ({ __typeof__ (a) _a = (a); \
        __typeof__ (b) _b = (b); \
        _a > _b ? _a : _b; })

#define min(a, b) \
    ({ __typeof__ (a) _a = (a); \
        __typeof__ (b) _b = (b); \
        _a < _b ? _a : _b; })

#define deg2rad(angle) \
    ({ __typeof__ (angle) _angle = (angle); \
        (_angle) * M_PI / 180.0; })

#define rad2deg(angle) \
    ({ __typeof__ (angle) _angle = (angle); \
        (_angle) * 180.0 / M_PI; })

#endif