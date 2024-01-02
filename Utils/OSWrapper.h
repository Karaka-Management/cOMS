/**
 * Jingga
 *
 * @package   Utils
 * @copyright Dennis Eichhorn
 * @license   OMS License 1.0
 * @version   1.0.0
 * @link      https://jingga.app
 */
#ifndef UTILS_OS_WRAPPER_H
#define UTILS_OS_WRAPPER_H

#include <stdio.h>
#include <stdlib.h>

#ifdef _WIN32
    #include <direct.h>
    #include <io.h>

    #define _chdir chdir;
    #define _getcwd getcwd;

    #ifdef _MSC_VER
        #define _access_s access;
    #else
        #include <unistd.h>
    #endif
#else
    #include <unistd.h>
#endif

#endif
