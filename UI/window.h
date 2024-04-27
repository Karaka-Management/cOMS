#ifndef UI_WINDOW_H
#define UI_WINDOW_H

#ifdef _WIN32
    #include <windows.h>
    #include <windowsx.h>

    LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
#endif

namespace UI
{
    typedef struct {
        unsigned int width;
        unsigned int height;

        #ifdef _WIN32
            HWND hwnd;
        #endif
    } window;

    void window_open(window* w)
    {
        #ifdef _WIN32
            ShowWindow(w->hwnd, SW_SHOW);
        #endif
    }

    #ifdef _WIN32
    void window_create_directx(window* w)
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

    window* window_create(window* w)
    {
        #ifdef _WIN32
            window_create_directx(w);
        #endif

        return w;
    }
}

#endif