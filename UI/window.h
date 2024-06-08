#ifndef UI_WINDOW_H
#define UI_WINDOW_H

#ifdef OPENGL
    #include "../Resources/opengl/glfw/include/glfw3.h"

    #ifdef GLFW_EXPOSE_NATIVE_WIN32
        #include "../Resources/opengl/glfw/include/glfw3native.h"
    #endif
#else
    #ifdef _WIN32
        #include <windows.h>
    #endif
#endif

#include "../Stdlib/Types.h"

namespace UI
{
    struct Window {
        int32 width;
        int32 height;
        char name[32];

        int32 x;
        int32 y;

        #ifdef OPENGL
            GLFWwindow* hwnd_lib;
        #endif

        #ifdef _WIN32
            HWND hwnd;
        #endif
    };

    void window_open(const Window* window)
    {
        #ifdef OPENGL
            return;
        #else
            #ifdef _WIN32
                ShowWindow(window->hwnd, SW_SHOW);
                UpdateWindow(window->hwnd);

                return;
            #endif
        #endif
    }

    void window_create_windows(Window* window, WNDPROC proc)
    {
        #if defined(_WIN32) && !defined(OPENGL)
        WNDCLASSEX wc = {};
        HINSTANCE hinstance = GetModuleHandle(0);

        wc.cbSize = sizeof(WNDCLASSEX);
        wc.style = CS_OWNDC;
        wc.lpfnWndProc = proc;
        wc.hInstance = hinstance;
        wc.lpszClassName = (LPCWSTR) window->name;

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
        #endif
    }

    void window_create_opengl(Window* window)
    {
        #ifdef OPENGL
            //GLFWmonitor *monitor = glfwGetPrimaryMonitor();
            window->hwnd_lib = glfwCreateWindow(
                window->width,
                window->height,
                window->name,
                NULL,
                NULL
            );

            #ifdef GLFW_EXPOSE_NATIVE_WIN32
                window->hwnd = glfwGetWin32Window(window->hwnd_lib);
            #endif
        #endif
    }

    void window_create(Window* window, void* data)
    {
        #ifdef OPENGL
            window_create_opengl(window);
            return;
        #else
            #ifdef _WIN32
                window_create_windows(window, (WNDPROC) data);
                return;
            #endif
        #endif
    }

    void window_close(Window* window)
    {
        #ifdef OPENGL
            glfwWindowShouldClose(window->hwnd_lib);
            return;
        #else
            #ifdef _WIN32
                CloseWindow(window->hwnd);
                return;
            #endif
        #endif
    }
}

#endif