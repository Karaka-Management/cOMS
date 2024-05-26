#ifndef UI_WINDOW_H
#define UI_WINDOW_H

#include <time.h>

#ifdef _WIN32
    #include <windows.h>
    #include <windowsx.h>

    LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
#endif

namespace UI
{
    struct WindowStat {
        double oldFrame = 0;
        double newFrame = 0;
        double dt = 0;
    };

    void update_window_stat(WindowStat *stat)
    {
        stat->newFrame = (double) clock() / CLOCKS_PER_SEC;
        stat->dt = stat->newFrame - stat->oldFrame;
        stat->oldFrame = stat->newFrame;
    }

    struct window {
        unsigned int width;
        unsigned int height;

        #ifdef _WIN32
            HWND hwnd;
        #endif
    };

    void window_open(window* w)
    {
        #ifdef _WIN32
            ShowWindow(w->hwnd, SW_SHOW);
        #endif
    }

    #if defined(_WIN32) && defined(DIRECTX)
    void window_create_windows(window* w)
    {
        HINSTANCE hinstance = GetModuleHandle(nullptr);

        WNDCLASSEX wc;
        ZeroMemory(&wc, sizeof(WNDCLASSEX));

        wc.cbSize = sizeof(WNDCLASSEX);
        wc.style = CS_HREDRAW | CS_VREDRAW;
        wc.lpfnWndProc = WindowProc;
        wc.hInstance = hinstance;
        wc.hCursor = LoadCursor(NULL, IDC_ARROW);
        wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
        wc.lpszClassName = L"WindowClass1";

        RegisterClassEx(&wc);

        RECT wr = {0, 0, 800, 600};
        AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, FALSE);

        w->hwnd = CreateWindowEx((DWORD) NULL,
            wc.lpszClassName, NULL,
            WS_OVERLAPPEDWINDOW,
            CW_USEDEFAULT, CW_USEDEFAULT,
            wr.right - wr.left,
            wr.bottom - wr.top,
            NULL, NULL, hinstance, w
        );
    }
    #else if defined(_WIN32) && defined(OPENGL)
    void window_create_opengl(window* w)
    {
        HINSTANCE hinstance = GetModuleHandle(nullptr);

        WNDCLASSEX wc;
        ZeroMemory(&wc, sizeof(WNDCLASSEX));

        wc.cbSize = sizeof(WNDCLASSEX);
        wc.style = CS_HREDRAW | CS_VREDRAW;
        wc.lpfnWndProc = WindowProc;
        wc.hInstance = hinstance;
        wc.hCursor = LoadCursor(NULL, IDC_ARROW);
        wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
        wc.lpszClassName = L"WindowClass1";

        RegisterClassEx(&wc);

        RECT wr = {0, 0, 800, 600};
        AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, FALSE);

        w->hwnd = CreateWindowEx((DWORD) NULL,
            wc.lpszClassName, NULL,
            WS_OVERLAPPEDWINDOW,
            CW_USEDEFAULT, CW_USEDEFAULT,
            wr.right - wr.left,
            wr.bottom - wr.top,
            NULL, NULL, hinstance, w
        );
    }
    #endif

    void window_create(window* w)
    {
        #ifdef _WIN32
            #if defined(DIRECTX)
                window_create_directx(w);
            #else if defined(_WIN32) && defined(OPENGL)
                window_create_opengl(w);
            #endif
        #endif
    }
}

#endif