/**
 * Karaka
 *
 * @package   Utils
 * @copyright Dennis Eichhorn
 * @license   OMS License 1.0
 * @version   1.0.0
 * @link      https://karaka.app
 */
#ifndef UTILS_OS_WRAPPER_H
#define UTILS_OS_WRAPPER_H

#include <stdio.h>
#include <stdlib.h>

#ifdef _WIN32
    #include <direct.h>
    #include <io.h>

    typedef _chdir chdir;
    typedef _getcwd getcwd;

    #ifdef _MSC_VER
        typedef _access_s access;
    #else
        #include <unistd.h>
    #endif
#else
    #include <unistd.h>
#endif

#endif