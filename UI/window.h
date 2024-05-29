#ifndef UI_WINDOW_H
#define UI_WINDOW_H

#ifdef _WIN32
    #include <windows.h>
#endif

#include "../Stdlib/Types.h"

namespace UI
{
    struct Window {
        int32 width;
        int32 height;
        char wName[32];

        int32 x;
        int32 y;

        #ifdef _WIN32
            HWND hwnd;
        #endif
    };

    void window_open(const Window* window)
    {
        #ifdef _WIN32
            ShowWindow(window->hwnd, SW_SHOW);
            UpdateWindow(window->hwnd);
        #endif
    }

    void window_create_windows(Window* window, WNDPROC proc)
     {
        WNDCLASSEX wc = {};
        HINSTANCE hinstance = GetModuleHandle(0);

        wc.cbSize = sizeof(WNDCLASSEX);
        wc.style = CS_OWNDC;
        wc.lpfnWndProc = proc;
        wc.hInstance = hinstance;
        wc.lpszClassName = (LPCWSTR) window->wName;

        RegisterClassEx(&wc);

        window->hwnd = CreateWindowEx((DWORD) NULL,
            wc.lpszClassName, NULL,
            WS_OVERLAPPEDWINDOW,
            CW_USEDEFAULT, CW_USEDEFAULT,
            window->width,
            window->height,
            NULL, NULL, hinstance, window
        );

        //SetWindowLongA(window->hwnd, GWL_STYLE, 0);
    }

    void window_close(Window* window)
    {
        #ifdef _WIN32
        CloseWindow(window->hwnd);
        #endif
    }
}

#endif