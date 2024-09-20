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

#define INPUT_MOUSE_BUTTON_1 1
#define INPUT_MOUSE_BUTTON_2 2
#define INPUT_MOUSE_BUTTON_3 4
#define INPUT_MOUSE_BUTTON_4 8
#define INPUT_MOUSE_BUTTON_5 16

// IMPORTANT:
// Even if it is nowhere documented (at least not to our knowledge) the GetRawInputDeviceInfoA, GetRawInputBuffer functions requried
// aligned memory. So far we only figured out that 4 bytes works, maybe this needs to be 8 in the future?!

int input_init(HWND hwnd, Input* __restrict states, RingMemory* ring)
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

void input_mouse_position(HWND hwnd, v2_int32* pos)
{
    POINT p;
    if (GetCursorPos(&p) && ScreenToClient(hwnd, &p)) {
        pos->x = p.x;
        pos->y = p.y;
    }
}

void input_raw_handle(RAWINPUT* __restrict raw, Input* states, int state_count, uint64 time)
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

        if (raw->data.mouse.usButtonFlags) {
            // @question should all of these be else ifs?
            if (raw->data.mouse.usButtonFlags & RI_MOUSE_LEFT_BUTTON_DOWN) {
                states[i].state.mouse_down |= INPUT_MOUSE_BUTTON_1;
                states[i].state.keys_down_time[0] = time;
            } else if (raw->data.mouse.usButtonFlags & RI_MOUSE_LEFT_BUTTON_UP) {
                states[i].state.mouse_down &= ~INPUT_MOUSE_BUTTON_1;
            }

            if (raw->data.mouse.usButtonFlags & RI_MOUSE_RIGHT_BUTTON_DOWN) {
                states[i].state.mouse_down |= INPUT_MOUSE_BUTTON_2;
                states[i].state.keys_down_time[1] = time;
            } else if (raw->data.mouse.usButtonFlags & RI_MOUSE_RIGHT_BUTTON_UP) {
                states[i].state.mouse_down &= ~INPUT_MOUSE_BUTTON_2;
            }

            if (raw->data.mouse.usButtonFlags & RI_MOUSE_MIDDLE_BUTTON_DOWN) {
                states[i].state.mouse_down |= INPUT_MOUSE_BUTTON_3;
                states[i].state.keys_down_time[2] = time;
            } else if (raw->data.mouse.usButtonFlags & RI_MOUSE_MIDDLE_BUTTON_UP) {
                states[i].state.mouse_down &= ~INPUT_MOUSE_BUTTON_3;
            }

            if (raw->data.mouse.usButtonFlags & RI_MOUSE_BUTTON_4_DOWN) {
                states[i].state.mouse_down |= INPUT_MOUSE_BUTTON_4;
                states[i].state.keys_down_time[3] = time;
            } else if (raw->data.mouse.usButtonFlags & RI_MOUSE_BUTTON_4_UP) {
                states[i].state.mouse_down &= ~INPUT_MOUSE_BUTTON_4;
            }

            if (raw->data.mouse.usButtonFlags & RI_MOUSE_BUTTON_5_DOWN) {
                states[i].state.mouse_down |= INPUT_MOUSE_BUTTON_5;
                states[i].state.keys_down_time[4] = time;
            } else if (raw->data.mouse.usButtonFlags & RI_MOUSE_BUTTON_5_UP) {
                states[i].state.mouse_down &= ~INPUT_MOUSE_BUTTON_5;
            }

            if (raw->data.mouse.usButtonFlags & RI_MOUSE_WHEEL) {
                states[i].state.wheel_delta += raw->data.mouse.usButtonData;
            }

            if (raw->data.mouse.usButtonFlags & RI_MOUSE_HWHEEL) {
                states[i].state.hwheel_delta += raw->data.mouse.usButtonData;
            }

            states[i].state_change_mouse = true;
            states[i].state_change_mouse_button = true;

            // @question is mouse wheel really considered a button change?
            states[i].state_change_button = true;
        }

        if (states[i].mouse_movement) {
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

        if (i >= state_count || !states[i].is_connected) {
            return;
        }

        // @todo change to MakeCode instead of VKey
        // @performance Some of the things down here seem unneccessary. We shouldn't have to loop all elements!
        if (raw->data.keyboard.Flags == RI_KEY_BREAK) {
            // Key is already released
            if (keyboard_is_released(&states[i].state, (uint8) raw->data.keyboard.VKey)) {
                for (int j = 0; j < MAX_KEY_PRESSES; ++j) {
                    if (states[i].state.keys_down[j] == (uint8) raw->data.keyboard.VKey) {
                        states[i].state.keys_down[j] = 0;

                        break;
                    }
                }

                return;
            }

            bool empty = true;
            for (int j = 0; j < MAX_KEY_PRESSES; ++j) {
                if (empty && states[i].state.keys_up[j] == 0) {
                    states[i].state.keys_up[j] = (uint8) raw->data.keyboard.VKey;

                    empty = false;
                }

                // remove pressed key
                if (states[i].state.keys_down[j] == (uint8) raw->data.keyboard.VKey) {
                    states[i].state.keys_down[j] = 0;
                }
            }
        } else if (raw->data.keyboard.Flags == RI_KEY_MAKE) {
            // Key is already released
            if (keyboard_is_pressed(&states[i].state, (uint8) raw->data.keyboard.VKey)) {
                for (int j = 0; j < MAX_KEY_PRESSES; ++j) {
                    if (states[i].state.keys_up[j] == (uint8) raw->data.keyboard.VKey) {
                        states[i].state.keys_up[j] = 0;

                        break;
                    }
                }

                return;
            }

            bool empty = true;
            for (int j = 0; j < MAX_KEY_PRESSES; ++j) {
                if (empty && states[i].state.keys_down[j] == 0) {
                    states[i].state.keys_down[j] = (uint8) raw->data.keyboard.VKey;
                    states[i].state.keys_down_time[MAX_MOUSE_PRESSES + j] = time;
                    empty = false;
                }

                // remove released key
                if (states[i].state.keys_up[j] == (uint8) raw->data.keyboard.VKey) {
                    states[i].state.keys_up[j] = 0;
                }
            }
        }

        states[i].state_change_button = true;
    }
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

// @bug Somehow this function skips some inputs (input_handle works)!!!!!
void input_handle_buffered(int buffer_size, Input* __restrict states, int state_count, RingMemory* ring, uint64 time)
{
    uint32 cb_size;

    GetRawInputBuffer(NULL, &cb_size, sizeof(RAWINPUTHEADER));
    if (!cb_size) {
        return;
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

            input_raw_handle(pri, states, state_count, time);

            pri = NEXTRAWINPUTBLOCK(pri);
        }
    }

    ASSERT_SIMPLE(input != (uint32) -1)
}

#endif