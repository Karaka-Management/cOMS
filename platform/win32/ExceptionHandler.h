/**
 * Jingga
 *
 * @copyright Jingga
 * @license   OMS License 2.0
 * @version   1.0.0
 * @link      https://jingga.app
 */
#ifndef TOS_PLATFORM_WIN32_EXCEPTION_HANDLER_H
#define TOS_PLATFORM_WIN32_EXCEPTION_HANDLER_H

#include <windows.h>
#include <dbghelp.h>
#include <stdio.h>
#include <stdlib.h>

void create_minidump(EXCEPTION_POINTERS *exception_pointers) {
    // Open the dump file
    HANDLE hFile = CreateFileA("crash_dump.dmp", GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

    if (hFile == INVALID_HANDLE_VALUE) {
        return;
    }

    // Write the minidump
    MINIDUMP_EXCEPTION_INFORMATION mdei;
    mdei.ThreadId = GetCurrentThreadId();
    mdei.ExceptionPointers = exception_pointers;
    mdei.ClientPointers = FALSE;

    MiniDumpWriteDump(
        GetCurrentProcess(),
        GetCurrentProcessId(),
        hFile,
        (MINIDUMP_TYPE) (MiniDumpWithDataSegs | MiniDumpWithHandleData | MiniDumpWithModuleHeaders |
            MiniDumpWithUnloadedModules | MiniDumpWithProcessThreadData |
            MiniDumpWithFullMemoryInfo | MiniDumpWithThreadInfo | MiniDumpWithTokenInformation),
        (exception_pointers != NULL) ? &mdei : NULL,
        NULL,
        NULL
    );

    CloseHandle(hFile);
}

void print_stack_trace(CONTEXT *context) {
    // Initialize the SYMBOL_INFO structure
    SymInitialize(GetCurrentProcess(), NULL, TRUE);

    STACKFRAME64 stack_frame;
    memset(&stack_frame, 0, sizeof(STACKFRAME64));

    // Determine if we are running on x86 or x64 architecture
    DWORD machine_type = IMAGE_FILE_MACHINE_I386;

#ifdef _M_X64
    machine_type = IMAGE_FILE_MACHINE_AMD64;
    stack_frame.AddrPC.Offset = context->Rip;
    stack_frame.AddrFrame.Offset = context->Rbp;
    stack_frame.AddrStack.Offset = context->Rsp;
#elif _M_IX86
    stack_frame.AddrPC.Offset = context->Eip;
    stack_frame.AddrFrame.Offset = context->Ebp;
    stack_frame.AddrStack.Offset = context->Esp;
#endif

    stack_frame.AddrPC.Mode = AddrModeFlat;
    stack_frame.AddrFrame.Mode = AddrModeFlat;
    stack_frame.AddrStack.Mode = AddrModeFlat;

    // Traverse the stack frames
    while (StackWalk64(
        machine_type,
        GetCurrentProcess(),
        GetCurrentThread(),
        &stack_frame,
        context,
        NULL,
        SymFunctionTableAccess64,
        SymGetModuleBase64,
        NULL)
    ) {
        if (stack_frame.AddrPC.Offset == 0) {
            break;
        }

        // Get the symbol for the current stack frame
        DWORD64 symbol_offset = 0;
        SYMBOL_INFO *symbol = (SYMBOL_INFO *)malloc(sizeof(SYMBOL_INFO) + 256);
        symbol->MaxNameLen = 255;
        symbol->SizeOfStruct = sizeof(SYMBOL_INFO);

        SymFromAddr(GetCurrentProcess(), stack_frame.AddrPC.Offset, &symbol_offset, symbol);

        free(symbol);
    }

    SymCleanup(GetCurrentProcess());
}

#endif