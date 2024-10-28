/**
 * Jingga
 *
 * @copyright Jingga
 * @license   OMS License 2.0
 * @version   1.0.0
 * @link      https://jingga.app
 */
#ifndef TOS_PLATFORM_WIN32_INPUT_RAW_H
#define TOS_PLATFORM_WIN32_INPUT_RAW_H

#include <windows.h>

#include "../../../stdlib/Types.h"
#include "../../../input/Input.h"
#include "../../../input/ControllerInput.h"
#include "controller/DualShock4.h"
#include "../../../utils/TestUtils.h"
#include "../../../utils/MathUtils.h"
#include "../../../memory/RingMemory.h"
#include "../../../memory/BufferMemory.h"
#include "../../../stdlib/simd/SIMD_I8.h"
#include <winDNS.h>

#define INPUT_MOUSE_BUTTON_1 1
#define INPUT_MOUSE_BUTTON_2 2
#define INPUT_MOUSE_BUTTON_3 4
#define INPUT_MOUSE_BUTTON_4 8
#define INPUT_MOUSE_BUTTON_5 16

// IMPORTANT:
// Even if it is nowhere documented (at least not to our knowledge) the GetRawInputDeviceInfoA, GetRawInputBuffer functions requried
// aligned memory. So far we only figured out that 4 bytes works, maybe this needs to be 8 in the future?!

int rawinput_init_mousekeyboard(HWND hwnd, Input* __restrict states, RingMemory* ring)
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
    int32 controller_found = 0;

    int32 i;
    for (i = 0; i < device_count; ++i) {
        cb_size = sizeof(RID_DEVICE_INFO);
        RID_DEVICE_INFO rdi;
        GetRawInputDeviceInfoA(pRawInputDeviceList[i].hDevice, RIDI_DEVICEINFO, &rdi, &cb_size);

        RAWINPUTDEVICE rid[1];

        switch (rdi.dwType) {
            case RIM_TYPEMOUSE: {
                    if (states[mouse_found].handle_mouse != NULL) {
                        ++mouse_found;
                    }

                    states[mouse_found].handle_mouse = pRawInputDeviceList[i].hDevice;
                    states[mouse_found].connection_type = INPUT_CONNECTION_TYPE_USB;

                    // Mouse
                    rid[0].usUsagePage = 0x01; // @todo doesn't work with 0x05 for games?
                    rid[0].usUsage     = 0x02;
                    rid[0].dwFlags     = RIDEV_DEVNOTIFY;
                    rid[0].hwndTarget  = hwnd;

                    if (!RegisterRawInputDevices((PCRAWINPUTDEVICE) rid, 1, sizeof(RAWINPUTDEVICE))) {
                        // @todo Log
                        ASSERT_SIMPLE(false);
                    }
                } break;
            case RIM_TYPEKEYBOARD: {
                if (states[keyboard_found].handle_keyboard != NULL) {
                        ++keyboard_found;
                    }

                    states[keyboard_found].handle_keyboard = pRawInputDeviceList[i].hDevice;
                    states[keyboard_found].connection_type = INPUT_CONNECTION_TYPE_USB;

                    // Keyboard
                    rid[0].usUsagePage = 0x01; // @todo doesn't work with 0x05 for games?
                    rid[0].usUsage	   = 0x06;
                    rid[0].dwFlags     = RIDEV_DEVNOTIFY;
                    rid[0].hwndTarget = hwnd;

                    if (!RegisterRawInputDevices((PCRAWINPUTDEVICE) rid, 1, sizeof(RAWINPUTDEVICE))) {
                        // @todo Log
                        ASSERT_SIMPLE(false);
                    }
                } break;
        }
    }

    return i;
}

int rawinput_init_controllers(HWND hwnd, Input* __restrict states, RingMemory* ring)
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
    int32 controller_found = 0;

    int32 i;
    for (i = 0; i < device_count; ++i) {
        cb_size = sizeof(RID_DEVICE_INFO);
        RID_DEVICE_INFO rdi;
        GetRawInputDeviceInfoA(pRawInputDeviceList[i].hDevice, RIDI_DEVICEINFO, &rdi, &cb_size);

        RAWINPUTDEVICE rid[1];

        switch (rdi.dwType) {
            case RIM_TYPEHID: {
                    if (rdi.hid.usUsage == 0x05) {
                        if (states[controller_found].handle_controller != NULL) {
                            ++controller_found;
                        }

                        states[controller_found].handle_controller = pRawInputDeviceList[i].hDevice;
                        // @bug This is not always true, how to check?
                        states[controller_found].connection_type = INPUT_CONNECTION_TYPE_USB;

                        // Gamepad
                        rid[0].usUsagePage = 0x01;
                        rid[0].usUsage	   = 0x05;
                        rid[0].dwFlags     = RIDEV_DEVNOTIFY;
                        rid[0].hwndTarget = hwnd;

                        if (!RegisterRawInputDevices((PCRAWINPUTDEVICE) rid, 1, sizeof(RAWINPUTDEVICE))) {
                            // @todo Log
                            ASSERT_SIMPLE(false);
                        }
                    } else if (rdi.hid.usUsage == 0x04) {
                        if (states[controller_found].handle_controller != NULL) {
                            ++controller_found;
                        }

                        states[controller_found].handle_controller = pRawInputDeviceList[i].hDevice;
                        // @bug This is not always true, how to check?
                        states[controller_found].connection_type = INPUT_CONNECTION_TYPE_USB;

                        // Joystick
                        rid[0].usUsagePage = 0x01;
                        rid[0].usUsage	   = 0x04;
                        rid[0].dwFlags     = RIDEV_DEVNOTIFY;
                        rid[0].hwndTarget = hwnd;

                        if (!RegisterRawInputDevices((PCRAWINPUTDEVICE) rid, 1, sizeof(RAWINPUTDEVICE))) {
                            // @todo Log
                            ASSERT_SIMPLE(false);
                        }
                    }
                } break;
            default: {

            }
        }
    }

    return i;
}

void input_mouse_position(HWND hwnd, v2_int32* pos)
{
    POINT p;
    if (GetCursorPos(&p) && ScreenToClient(hwnd, &p)) {
        pos->x = p.x;
        pos->y = p.y;
    }
}

int32 input_raw_handle(RAWINPUT* __restrict raw, Input* states, int32 state_count, uint64 time)
{
    int32 input_count = 0;

    int32 i = 0;
    if (raw->header.dwType == RIM_TYPEMOUSE) {
        // @performance Change so we can directly access the correct state (maybe map handle address to index?)
        while (i < state_count
            && states[i].handle_mouse != raw->header.hDevice
        ) {
            ++i;
        }

        if (i >= state_count || !states[i].connection_type) {
            return 0;
        }

        if (raw->data.mouse.usButtonFlags) {
            InputKey key;

            if (raw->data.mouse.usButtonFlags & RI_MOUSE_LEFT_BUTTON_DOWN) {
                key.key_state = KEY_STATE_PRESSED;
                key.key_id = 1;
            } else if (raw->data.mouse.usButtonFlags & RI_MOUSE_LEFT_BUTTON_UP) {
                key.key_state = KEY_STATE_RELEASED;
                key.key_id = 1;
            } else if (raw->data.mouse.usButtonFlags & RI_MOUSE_RIGHT_BUTTON_DOWN) {
                key.key_state = KEY_STATE_PRESSED;
                key.key_id = 2;
            } else if (raw->data.mouse.usButtonFlags & RI_MOUSE_RIGHT_BUTTON_UP) {
                key.key_state = KEY_STATE_RELEASED;
                key.key_id = 2;
            } else if (raw->data.mouse.usButtonFlags & RI_MOUSE_MIDDLE_BUTTON_DOWN) {
                key.key_state = KEY_STATE_PRESSED;
                key.key_id = 3;
            } else if (raw->data.mouse.usButtonFlags & RI_MOUSE_MIDDLE_BUTTON_UP) {
                key.key_state = KEY_STATE_RELEASED;
                key.key_id = 3;
            } else if (raw->data.mouse.usButtonFlags & RI_MOUSE_BUTTON_4_DOWN) {
                key.key_state = KEY_STATE_PRESSED;
                key.key_id = 4;
            } else if (raw->data.mouse.usButtonFlags & RI_MOUSE_BUTTON_4_UP) {
                key.key_state = KEY_STATE_RELEASED;
                key.key_id = 4;
            } else if (raw->data.mouse.usButtonFlags & RI_MOUSE_BUTTON_5_DOWN) {
                key.key_state = KEY_STATE_PRESSED;
                key.key_id = 5;
            } else if (raw->data.mouse.usButtonFlags & RI_MOUSE_BUTTON_5_UP) {
                key.key_state = KEY_STATE_RELEASED;
                key.key_id = 5;
            } else {
                return 0;
            }

            /* @todo implement
            if (raw->data.mouse.usButtonFlags & RI_MOUSE_WHEEL) {
                states[i].state.wheel_delta += raw->data.mouse.usButtonData;
            }

            if (raw->data.mouse.usButtonFlags & RI_MOUSE_HWHEEL) {
                states[i].state.hwheel_delta += raw->data.mouse.usButtonData;
            }
            */

            // @question is mouse wheel really considered a button change?

            ++input_count;

            key.key_id |= INPUT_MOUSE_PREFIX;
            key.value = 0;
            key.time = time;

            input_set_state(&states[i].state, &key);
            states[i].state_change_button = true;
        } else if (states[i].mouse_movement) {
            // do we want to handle mouse movement for every individual movement, or do we want to pull it
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

                states[i].state.dx += raw->data.mouse.lLastX;
                states[i].state.dy += raw->data.mouse.lLastY;

                states[i].state.x = MulDiv(raw->data.mouse.lLastX, rect.right, 65535) + rect.left;
                states[i].state.y = MulDiv(raw->data.mouse.lLastY, rect.bottom, 65535) + rect.top;

                states[i].state_change_mouse = true;
            } else if (raw->data.mouse.lLastX != 0 || raw->data.mouse.lLastY != 0) {
                states[i].state.dx += raw->data.mouse.lLastX;
                states[i].state.dy += raw->data.mouse.lLastY;

                states[i].state.x = states[i].state.x + raw->data.mouse.lLastX;
                states[i].state.y = states[i].state.y + raw->data.mouse.lLastY;

                states[i].state_change_mouse = true;
            }
        }
    } else if (raw->header.dwType == RIM_TYPEKEYBOARD) {
        // @todo Change so we can directly access the correct state (maybe map handle address to index?)
        while (i < state_count
            && states[i].handle_keyboard != raw->header.hDevice
        ) {
            ++i;
        }

        if (i >= state_count || !states[i].connection_type) {
            return 0;
        }

        uint16 new_state;
        if (raw->data.keyboard.Flags == RI_KEY_BREAK) {
            new_state = KEY_STATE_RELEASED;
        } else if (raw->data.keyboard.Flags == RI_KEY_MAKE) {
            new_state = KEY_STATE_PRESSED;
        } else {
            return 0;
        }

        ++input_count;

        // @todo change to MakeCode instead of VKey
        InputKey key = {(uint16) (raw->data.keyboard.VKey | INPUT_KEYBOARD_PREFIX), new_state, 0, time};
        input_set_state(&states[i].state, &key);
        states[i].state_change_button = true;
    } else if (raw->header.dwType == RIM_TYPEHID) {
        if (raw->header.dwSize > sizeof(RAWINPUT)) {
            // @performance This shouldn't be done every time, it should be polling based
            // Controllers often CONSTANTLY send data -> really bad
            // Maybe we can add timer usage instead of polling?
            while (i < state_count
                && states[i].handle_controller != raw->header.hDevice
            ) {
                ++i;
            }

            if (i >= state_count || !states[i].connection_type
                || time - states[i].time_last_input_check < 5
            ) {
                return 0;
            }

            // @todo Find a way to handle most common controllers
            //      DualSense
            //      Xbox
            //      Xinput
            // Best way would probably to define the controller type in the input
            ControllerInput controller = {};
            input_map_dualshock4(&controller, states[i].connection_type, raw->data.hid.bRawData);
            input_set_controller_state(&states[i], &controller, time);

            states[i].time_last_input_check = time;
        }
    }

    return input_count;
}

void input_handle(LPARAM lParam, Input* __restrict states, int state_count, RingMemory* ring, uint64 time)
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

    input_raw_handle((RAWINPUT *) lpb, states, state_count, time);
}

int32 input_handle_buffered(int buffer_size, Input* __restrict states, int state_count, RingMemory* ring, uint64 time)
{
    int32 input_count = 0;
    uint32 cb_size;

    GetRawInputBuffer(NULL, &cb_size, sizeof(RAWINPUTHEADER));
    if (!cb_size) {
        return 0;
    }

    // Max input messages (e.g. 16)
    cb_size *= buffer_size;

    PRAWINPUT raw_input = (PRAWINPUT) ring_get_memory(ring, cb_size, 4);

    uint32 input;

    while (true) {
        uint32 cb_size_t = cb_size;
        input = GetRawInputBuffer(raw_input, &cb_size_t, sizeof(RAWINPUTHEADER));

        if (input == 0 || input == (uint32) -1) {
            break;
        }

        PRAWINPUT pri = raw_input;
        for (uint32 i = 0; i < input; ++i) {
            if (!pri->header.hDevice) {
                break;
            }

            input_count += input_raw_handle(pri, states, state_count, time);

            pri = NEXTRAWINPUTBLOCK(pri);
        }
    }

    ASSERT_SIMPLE(input != (uint32) -1);

    return input_count;
}

#endif