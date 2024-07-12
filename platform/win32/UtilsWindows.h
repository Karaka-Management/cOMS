/**
 * Jingga
 *
 * @copyright Jingga
 * @license   OMS License 2.0
 * @version   1.0.0
 * @link      https://jingga.app
 */
#ifndef TOS_UTILS_WINDOWS_H
#define TOS_UTILS_WINDOWS_H

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
};

void window_create(Window* window, void* proc)
{
    WNDPROC wndproc = (WNDPROC) proc;
    WNDCLASSEX wc = {};
    HINSTANCE hinstance = GetModuleHandle(0);

    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = CS_OWNDC;
    wc.lpfnWndProc = wndproc;
    wc.hInstance = hinstance;
    wc.lpszClassName = (LPCWSTR) window->name;

    RegisterClassEx(&wc);

    if (window->is_fullscreen) {
        window->width  = GetSystemMetrics(SM_CXSCREEN);
	    window->height = GetSystemMetrics(SM_CYSCREEN);

        DEVMODE screen_settings;

        memset(&screen_settings, 0, sizeof(screen_settings));
		screen_settings.dmSize       = sizeof(screen_settings);
		screen_settings.dmPelsWidth  = (unsigned long) window->width;
		screen_settings.dmPelsHeight = (unsigned long) window->height;
		screen_settings.dmBitsPerPel = 32;
		screen_settings.dmFields     = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		ChangeDisplaySettings(&screen_settings, CDS_FULLSCREEN);

        window->x = 0;
        window->y = 0;
    }

    window->hwnd = CreateWindowEx((DWORD) NULL,
        wc.lpszClassName, NULL,
        WS_OVERLAPPEDWINDOW,
        window->x, window->y,
        window->width,
        window->height,
        NULL, NULL, hinstance, window
    );

    //SetWindowLongA(window->hwnd, GWL_STYLE, 0);
}

void window_open(const Window* window)
{
    ShowWindow(window->hwnd, SW_SHOW);
    SetForegroundWindow(window->hwnd);
	SetFocus(window->hwnd);
    ShowCursor(false);
    UpdateWindow(window->hwnd);
}

void window_close(Window* window)
{
    CloseWindow(window->hwnd);
}

#endif