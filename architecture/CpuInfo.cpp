/**
 * Jingga
 *
 * @copyright Jingga
 * @license   OMS License 2.0
 * @version   1.0.0
 * @link      https://jingga.app
 */
#ifndef TOS_ARCHITECTURE_CPU_INFO_C
#define TOS_ARCHITECTURE_CPU_INFO_C

#include "CpuInfo.h"

#if ARM
    #include "arm/CpuInfo.cpp"
#else
    #include "x86/CpuInfo.cpp"
#endif

#endif