/**
 * Jingga
 *
 * @package   Utils
 * @copyright Dennis Eichhorn
 * @license   OMS License 1.0
 * @version   1.0.0
 * @link      https://jingga.app
 */
#ifndef UTILS_MATH_UTILS_H
#define UTILS_MATH_UTILS_H

#include <stdio.h>

#define OMS_PI 3.14159265358979323846

#define oms_max(a, b) ((a) > (b) ? (a) : (b))
#define oms_min(a, b) ((a) > (b) ? (b) : (a))
#define oms_abs(a) ((a) > 0 ? (a) : -(a))
#define oms_deg2rad(angle) ((angle) * OMS_PI / 180.0)
#define oms_rad2deg(angle) ((angle) * 180.0 / OMS_PI)
#define round_to_nearest(a, b) (((a) + ((b) - 1)) & ~((b) - 1))

#endif
