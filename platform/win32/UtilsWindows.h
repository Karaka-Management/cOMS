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
#include "Window.h"
#include "../../stdlib/Types.h"
#include "../../utils/TestUtils.h"

inline
void window_inactive(Window* w)
{
    LONG_PTR style = GetWindowLongPtrA(w->hwnd, GWL_STYLE);
    style |= WS_OVERLAPPEDWINDOW;
    SetWindowLongPtr(w->hwnd, GWL_STYLE, style);

    ClipCursor(NULL);

    // WARNING: Apparently this has an internal reference count, effecting if true/false actually take effect!
    ShowCursor(true);

    w->mouse_captured = false;
}

inline
void monitor_resolution(const Window* __restrict w, v2_int32* __restrict resolution)
{
    resolution->width = GetDeviceCaps(w->hdc, HORZRES);
    resolution->height = GetDeviceCaps(w->hdc, VERTRES);
}

inline
void monitor_resolution(Window* __restrict w)
{
    w->width = GetDeviceCaps(w->hdc, HORZRES);
    w->height = GetDeviceCaps(w->hdc, VERTRES);
}

inline
void window_active(Window* __restrict w)
{
    LONG_PTR style = GetWindowLongPtrA(w->hwnd, GWL_STYLE);
    style &= ~WS_OVERLAPPEDWINDOW;
    SetWindowLongPtr(w->hwnd, GWL_STYLE, style);

    SetWindowPos(
        w->hwnd, HWND_TOP,
        w->x, w->y,
        w->width, w->height,
        SWP_NOACTIVATE | SWP_NOZORDER
    );

    RECT rect;
    GetWindowRect(w->hwnd, &rect);
    ClipCursor(&rect);

    // WARNING: Apparently this has an internal reference count, effecting if true/false actually take effect!
    ShowCursor(false);

    w->mouse_captured = true;
}

inline
void window_fullscreen(Window* __restrict w)
{
    monitor_resolution(w);
    w->x = 0;
    w->y = 0;

    LONG style = GetWindowLong(w->hwnd, GWL_STYLE);
    SetWindowLongPtr(w->hwnd, GWL_STYLE, style & ~WS_OVERLAPPEDWINDOW);

    SetWindowPos(w->hwnd, HWND_TOP, 0, 0, w->width, w->height, SWP_NOACTIVATE | SWP_NOZORDER | SWP_NOMOVE);
}

inline
void window_restore(Window* __restrict w)
{
    window_restore_state(w);

    SetWindowLongPtr(w->hwnd, GWL_STYLE, w->state_old.style);
    SetWindowPos(
        w->hwnd, HWND_TOP,
        w->state_old.x, w->state_old.y,
        w->state_old.width, w->state_old.height,
        SWP_NOACTIVATE | SWP_NOZORDER
    );
}

void window_create(Window* __restrict window, void* proc)
{
    ASSERT_SIMPLE(proc);

    WNDPROC wndproc = (WNDPROC) proc;
    WNDCLASSEXA wc = {};
    HINSTANCE hinstance = GetModuleHandle(0);

    wc.cbSize = sizeof(WNDCLASSEXA);
    wc.style = CS_OWNDC;
    wc.lpfnWndProc = wndproc;
    wc.hInstance = hinstance;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.lpszClassName = (LPCSTR) window->name;

    if (!RegisterClassExA(&wc)) {
        return;
    }

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

    window->hwnd = CreateWindowExA((DWORD) NULL,
        wc.lpszClassName, NULL,
        WS_OVERLAPPEDWINDOW,
        window->x, window->y,
        window->width,
        window->height,
        NULL, NULL, hinstance, window
    );

    window->hdc = GetDC(window->hwnd);

    ASSERT_SIMPLE(window->hwnd);
}

void window_open(const Window* __restrict window)
{
    ShowWindow(window->hwnd, SW_SHOW);
    SetForegroundWindow(window->hwnd);
	SetFocus(window->hwnd);
    UpdateWindow(window->hwnd);
}

void window_close(Window* __restrict window)
{
    CloseWindow(window->hwnd);
}

#endif