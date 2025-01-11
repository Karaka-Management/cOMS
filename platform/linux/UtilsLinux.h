/**
 * Jingga
 *
 * @copyright Jingga
 * @license   OMS License 2.0
 * @version   1.0.0
 * @link      https://jingga.app
 */
#ifndef TOS_UTILS_LINUX_H
#define TOS_UTILS_LINUX_H

#include "../../stdlib/Types.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <X11/Xlib.h>
#include <X11/Xatom.h>


int32 sprintf_s(char *buffer, size_t sizeOfBuffer, const char *format, ...) {
    int32 result;
    va_list args;

    if (buffer == NULL || format == NULL || sizeOfBuffer == 0) {
        return -1;
    }

    va_start(args, format);
    result = vsnprintf(buffer, sizeOfBuffer, format, args);
    va_end(args);

    if (result >= 0 && (size_t) result >= sizeOfBuffer) {
        buffer[sizeOfBuffer - 1] = '\0';
        return 80;
    }

    // Return the result
    return result;
}

void clipboard_get(char* text, int32 max_length)
{
    *text = '\0';

    Display *display = XOpenDisplay(NULL);
    if (display == NULL) {
        return;
    }

    Atom clipboard = XInternAtom(display, "CLIPBOARD", false);
    Atom utf8_string = XInternAtom(display, "UTF8_STRING", false);
    Atom xa_string = XInternAtom(display, "STRING", false);
    Window window = XDefaultRootWindow(display);

    XConvertSelection(display, clipboard, utf8_string, xa_string, window, CurrentTime);
    XEvent event;
    XNextEvent(display, &event);

    if (event.type == SelectionNotify) {
        if (event.xselection.property) {
            Atom type;
            int32 format;
            unsigned long nitems, bytes_after;
            byte* data = NULL;

            XGetWindowProperty(
                display, event.xselection.requestor,
                event.xselection.property, 0, (~0L), false,
                AnyPropertyType, &type, &format, &nitems,
                &bytes_after, &data
            );

            if (data) {
                str_copy_short(text, clipboard_text, max_length);
                XFree(data);
            }
        }
    }

    XCloseDisplay(display);
}

#endif