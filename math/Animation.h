/**
 * Jingga
 *
 * @package   Utils
 * @copyright Jingga
 * @license   OMS License 2.0
 * @version   1.0.0
 * @link      https://jingga.app
 */
#ifndef TOS_MATH_ANIMATION
#define TOS_MATH_ANIMATION

inline
float lerp_approx(float a, float b, float t)
{
    return a + t * (b - a);
}

#endif