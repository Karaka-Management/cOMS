/**
 * Jingga
 *
 * @copyright Jingga
 * @license   OMS License 2.0
 * @version   1.0.0
 * @link      https://jingga.app
 */
#ifndef TOS_ARCHITECTURE_INTRINSICS_H
#define TOS_ARCHITECTURE_INTRINSICS_H

#if ARM
    #include "arm/Intrinsics.h"
#else
    #include "x86/Intrinsics.h"
#endif

#endif