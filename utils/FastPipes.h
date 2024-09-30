/**
 * Jingga
 *
 * @copyright Jingga
 * @license   OMS License 2.0
 * @version   1.0.0
 * @link      https://jingga.app
 */
#ifndef TOS_UTILS_FAST_PIPES_H
#define TOS_UTILS_FAST_PIPES_H

// requires kernel32.lib and user32.lib
#include "../stdlib/Types.h"

#if _WIN32
    #include <windows.h>
    #include <io.h>
    #include <fcntl.h>
    #include <stdio.h>

    static int ENABLE_FAST_PIPES()
    {
        int32 result = 0;

        wchar_t pipe_name[32];
        wsprintfW(pipe_name, L"\\\\.\\pipe\\fastpipe%x", GetCurrentProcessId());
        HANDLE fast_pip = CreateFileW(pipe_name, GENERIC_READ | GENERIC_WRITE, 0, 0, OPEN_EXISTING, 0, 0);

        if(fast_pip != INVALID_HANDLE_VALUE) {
            SetStdHandle(STD_OUTPUT_HANDLE, fast_pip);
            SetStdHandle(STD_INPUT_HANDLE, fast_pip);

            int32 std_out = _open_osfhandle((intptr_t) fast_pip, O_WRONLY | O_TEXT);
            int32 std_in = _open_osfhandle((intptr_t) fast_pip, O_RDONLY | O_TEXT);

            _dup2(std_out, _fileno(stdout));
            _dup2(std_in, _fileno(stdin));

            _close(std_out);
            _close(std_in);

            result = 1;
        }

        return result;
    }
#else
    #define ENABLE_FAST_PIPES(...) 0
#endif

#endif