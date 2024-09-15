/**
 * Jingga
 *
 * @copyright Jingga
 * @license   OMS License 2.0
 * @version   1.0.0
 * @link      https://jingga.app
 */
#ifndef TOS_INPUT_RAW_H
#define TOS_INPUT_RAW_H

#include <windows.h>

#include "../../../stdlib/Types.h"
#include "../../../input/Input.h"
#include "../../../utils/TestUtils.h"
#include "../../../utils/MathUtils.h"
#include "../../../memory/RingMemory.h"
#include "../../../memory/BufferMemory.h"
#include <winDNS.h>

// IMPORTANT:
// Even if it is nowhere documented (at least not to our knowledge) the GetRawInputDeviceInfoA, GetRawInputBuffer functions requried
// aligned memory. So far we only figured out that 4 bytes works, maybe this needs to be 8 in the future?!

int input_init(HWND hwnd, InputState* states, RingMemory* ring, BufferMemory* buf)
{
    uint32 device_count;
    GetRawInputDeviceList(NULL, &device_count, sizeof(RAWINPUTDEVICELIST));
    PRAWINPUTDEVICELIST pRawInputDeviceList = (PRAWINPUTDEVICELIST) ring_get_memory(ring, sizeof(RAWINPUTDEVICELIST) * device_count, 4);
    device_count = GetRawInputDeviceList(pRawInputDeviceList, &device_count, sizeof(RAWINPUTDEVICELIST));

    // We always want at least one empty input device slot
    // @todo Change so that we store the actual number of devices
    if (device_count == 0) {
        return 0;
    }

    uint32 cb_size = 256;

    int32 mouse_found = 0;
    int32 keyboard_found = 0;

    int32 i;
    for (i = 0; i < device_count; ++i) {
        cb_size = sizeof(RID_DEVICE_INFO);
        RID_DEVICE_INFO rdi;
        GetRawInputDeviceInfoA(pRawInputDeviceList[i].hDevice, RIDI_DEVICEINFO, &rdi, &cb_size);

        switch (rdi.dwType) {
            case RIM_TYPEMOUSE: {
                    // @bug Would need fixing once we support controllers here
                    if (states[mouse_found].handle_mouse != NULL) {
                        ++mouse_found;
                    }

                    states[mouse_found].handle_mouse = pRawInputDeviceList[i].hDevice;
                    states[mouse_found].is_connected = true;
                    states[mouse_found].type = INPUT_TYPE_MOUSE_KEYBOARD;
                } break;
            case RIM_TYPEKEYBOARD: {
                // @bug Would need fixing once we support controllers here (keyboard + controller in one input bug)
                if (states[keyboard_found].handle_keyboard != NULL) {
                        ++keyboard_found;
                    }

                    states[keyboard_found].handle_keyboard = pRawInputDeviceList[i].hDevice;
                    states[keyboard_found].is_connected = true;
                    states[keyboard_found].type = INPUT_TYPE_MOUSE_KEYBOARD;
                } break;
            case RIM_TYPEHID: {
                    states[i].type = INPUT_TYPE_OTHER;
                } break;
            default: {

            }
        }
    }

    RAWINPUTDEVICE rid[4];

    // Mouse
    rid[0].usUsagePage = 0x01; // @todo doesn't work with 0x05 for games?
    rid[0].usUsage     = 0x02;
    rid[0].dwFlags     = RIDEV_DEVNOTIFY;
    rid[0].hwndTarget  = hwnd;

    // Joystick
    rid[1].usUsagePage = 0x01; // @todo doesn't work with 0x05 for games?
    rid[1].usUsage	   = 0x04;
    rid[1].dwFlags     = RIDEV_DEVNOTIFY;
    rid[1].hwndTarget = hwnd;

    // Gamepad
    rid[2].usUsagePage = 0x01; // @todo doesn't work with 0x05 for games?
    rid[2].usUsage	   = 0x05;
    rid[2].dwFlags     = RIDEV_DEVNOTIFY;
    rid[2].hwndTarget = hwnd;

    // Keyboard
    rid[3].usUsagePage = 0x01; // @todo doesn't work with 0x05 for games?
    rid[3].usUsage	   = 0x06;
    rid[3].dwFlags     = RIDEV_DEVNOTIFY;
    rid[3].hwndTarget = hwnd;

    if (!RegisterRawInputDevices((PCRAWINPUTDEVICE) rid, 4, sizeof(RAWINPUTDEVICE))) {
        // @todo Log
        ASSERT_SIMPLE(false);
    }

    return i;
}

void input_raw_handle(RAWINPUT* raw, InputState* states, int state_count)
{
    uint32 i = 0;
    if (raw->header.dwType == RIM_TYPEMOUSE) {
        // @todo Change so we can directly access the correct state (maybe map handle address to index?)
        while (i < state_count
            && states[i].handle_mouse != raw->header.hDevice
        ) {
            ++i;
        }

        if (i >= state_count || !states[i].is_connected) {
            return;
        }

        InputState* input_state = states + i;

        if (raw->data.mouse.usFlags & MOUSE_MOVE_ABSOLUTE) {
            RECT rect;

            // @todo move out, this is slow and should be stored in Window
            // @performance this is slow and should be handled in the WindowProc !!!
            if (raw->data.mouse.usFlags & MOUSE_VIRTUAL_DESKTOP) {
                rect.left = GetSystemMetrics(SM_XVIRTUALSCREEN);
                rect.top = GetSystemMetrics(SM_YVIRTUALSCREEN);
                rect.right = GetSystemMetrics(SM_CXVIRTUALSCREEN);
                rect.bottom = GetSystemMetrics(SM_CYVIRTUALSCREEN);
            } else {
                rect.left = 0;
                rect.top = 0;
                rect.right = GetSystemMetrics(SM_CXSCREEN);
                rect.bottom = GetSystemMetrics(SM_CYSCREEN);
            }

            input_state->x_last = input_state->x;
            input_state->y_last = input_state->y;

            input_state->x = MulDiv(raw->data.mouse.lLastX, rect.right, 65535) + rect.left;
            input_state->y = MulDiv(raw->data.mouse.lLastY, rect.bottom, 65535) + rect.top;

            input_state->state_change_mouse = true;
        } else if (raw->data.mouse.lLastX != 0 || raw->data.mouse.lLastY != 0) {
            input_state->x_last = input_state->x;
            input_state->y_last = input_state->y;

            input_state->x = input_state->x + raw->data.mouse.lLastX;
            input_state->y = input_state->y + raw->data.mouse.lLastY;

            input_state->state_change_mouse = true;
        }
    } else if (raw->header.dwType == RIM_TYPEKEYBOARD) {
        // @todo Change so we can directly access the correct state (maybe map handle address to index?)
        while (i < state_count
            && states[i].handle_keyboard != raw->header.hDevice
        ) {
            ++i;
        }

        if (i >= state_count || !states[i].is_connected) {
            return;
        }

        InputState* input_state = states + i;

        RAWKEYBOARD raw_kb =  raw->data.keyboard;

        if (raw_kb.Flags & RI_KEY_BREAK) {
            input_state->keys_down_old[input_state->up_index++] = (uint8) raw_kb.VKey;
        }

        if (raw_kb.Flags & RI_KEY_MAKE) {
            input_state->keys_down[input_state->down_index++] = (uint8) raw_kb.VKey;
        }

        input_state->state_change_keyboard = true;
    }
}

void input_handle(LPARAM lParam, InputState* states, int state_count, RingMemory* ring)
{
    uint32 db_size;
    GetRawInputData((HRAWINPUT) lParam, RID_INPUT, NULL, &db_size, sizeof(RAWINPUTHEADER));

    // @todo pull out, we only need to register this memory once
    //      Maybe even put it into the general memory pool
    LPBYTE lpb = (BYTE *) ring_get_memory(ring, db_size * sizeof(BYTE), 4);
    uint32 size = GetRawInputData((HRAWINPUT) lParam, RID_INPUT, lpb, &db_size, sizeof(RAWINPUTHEADER));

    if (db_size != size) {
        return;
    }

    input_raw_handle((RAWINPUT *) lpb, states, state_count);
}

void input_handle_buffered(LPARAM lParam, int buffer_size, InputState* states, int state_count, RingMemory* ring)
{
    uint32 cb_size;
    GetRawInputBuffer(NULL, &cb_size, sizeof(RAWINPUTHEADER));

    // Max input messages (e.g. 16)
    cb_size *= buffer_size;

    PRAWINPUT raw_input = (PRAWINPUT) ring_get_memory(ring, cb_size, 4);

    uint32 input;
    uint32 cb_size_t = cb_size;

    while ((input = GetRawInputBuffer(raw_input, &cb_size_t, sizeof(RAWINPUTHEADER))) > 0) {
        PRAWINPUT pri = raw_input;
        for (uint32 i = 0; i < input; ++i) {
            input_raw_handle(pri, states, state_count);

            pri = NEXTRAWINPUTBLOCK(pri);
        }

        // @question is this asign necessary?
        cb_size_t = cb_size;
    }
}

#endif