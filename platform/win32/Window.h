/**
 * Jingga
 *
 * @copyright Jingga
 * @license   OMS License 2.0
 * @version   1.0.0
 * @link      https://jingga.app
 */
#ifndef TOS_UTILS_WINDOW_H
#define TOS_UTILS_WINDOW_H

#include <windows.h>
#include "../../stdlib/Types.h"

struct Window {
    bool is_fullscreen;
    int32 width;
    int32 height;
    char name[32];

    int32 x;
    int32 y;

    HWND hwnd;
    HDC hdc;
};

#endif