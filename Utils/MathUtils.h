/**
 * Karaka
 *
 * @package   Utils
 * @copyright Dennis Eichhorn
 * @license   OMS License 1.0
 * @version   1.0.0
 * @link      https://karaka.app
 */
#ifndef UTILS_MATH_UTILS_H
#define UTILS_MATH_UTILS_H

#define OMS_PI 3.14159265358979323846

#define max(a, b) \
    ({ __typeof__ (a) _a = (a); \
        __typeof__ (b) _b = (b); \
        _a > _b ? _a : _b; })

#define min(a, b) \
    ({ __typeof__ (a) _a = (a); \
        __typeof__ (b) _b = (b); \
        _a < _b ? _a : _b; })

#define abs(a) \
    ({ __typeof__ (a) _a = (a); \
        _a > 0 ? _a : -_a; })

#define deg2rad(angle) \
    ({ __typeof__ (angle) _angle = (angle); \
        (_angle) * OMS_PI / 180.0; })

#define rad2deg(angle) \
    ({ __typeof__ (angle) _angle = (angle); \
        (_angle) * 180.0 / M_PI; })

#endif
