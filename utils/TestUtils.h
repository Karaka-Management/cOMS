/**
 * Jingga
 *
 * @copyright Jingga
 * @license   OMS License 2.0
 * @version   1.0.0
 * @link      https://jingga.app
 */
#ifndef COMS_UTILS_TEST_UTILS_H
#define COMS_UTILS_TEST_UTILS_H

#if DEBUG
    #define ASSERT_SIMPLE(a) if (!(a)) { *(volatile int *)0 = 0; }
    #define ASSERT_SIMPLE_CONST(a) if constexpr (!(a)) { *(volatile int *)0 = 0; }
#else
    #define ASSERT_SIMPLE(a) ((void)0)
    #define ASSERT_SIMPLE_CONST(a) ((void)0)
#endif

#endif
