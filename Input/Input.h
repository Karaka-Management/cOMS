/**
 * Karaka
 *
 * @package   Stdlib
 * @copyright Dennis Eichhorn
 * @license   OMS License 1.0
 * @version   1.0.0
 * @link      https://jingga.app
 */
#ifndef INPUT_H
#define INPUT_H

#ifdef _WIN32
    #include <windows.h>
#else
#endif

#include "../Stdlib/Types.h"
#include "../Utils/TestUtils.h"
#include "../Utils/MathUtils.h"
#include "../UI/Window.h"

#define MAX_KEY_PRESSES 4
#define MIN_INPUT_DEVICES 2

#define INPUT_TYPE_MOUSE 0x01
#define INPUT_TYPE_KEYBOARD 0x02
#define INPUT_TYPE_OTHER 0x03

namespace Input {
    struct InputState {
        // Device
        bool is_connected = false;
        char name[256];
        byte type = INPUT_TYPE_OTHER;
        HANDLE handle_keyboard;
        HANDLE handle_mouse;

        // Keyboard
        bool key_down = false;
        bool key_up = false;
        uint16 key = 0;

        // After handling the keyboard state change the game loop should set this to false
        bool state_change_keyboard = false;

        // We only consider up to 4 pressed keys
        // Depending on the keyboard you may only be able to detect a limited amount of key presses anyway
        uint16 keys_down[MAX_KEY_PRESSES];

        // Mouse
        // After handling the mouse state change the game loop should set this to false
        bool state_change_mouse = false;

        uint32 x;
        uint32 y;

        uint32 x_last;
        uint32 y_last;

        bool mouse1_down = false;
        bool mouse1_up = false;

        bool mouse2_down = false;
        bool mouse2_up = false;

        bool mouse3_down = false;
        bool mouse3_up = false;

        bool mouse4_down = false;
        bool mouse4_up = false;

        bool mouse5_down = false;
        bool mouse5_up = false;

        int16 wheel_delta = 0;
        uint32 raw_button = 0;
    };

    InputState* init_input(const UI::Window* w)
    {
        uint32 nDevices;
        GetRawInputDeviceList(NULL, &nDevices, sizeof(RAWINPUTDEVICELIST));
        PRAWINPUTDEVICELIST pRawInputDeviceList = (PRAWINPUTDEVICELIST) malloc(sizeof(RAWINPUTDEVICELIST) * nDevices);
        nDevices = GetRawInputDeviceList(pRawInputDeviceList, &nDevices, sizeof(RAWINPUTDEVICELIST));

        nDevices = OMS_MAX(MIN_INPUT_DEVICES, nDevices);

        // We always want at least one empty input device slot
        // @todo Change so that we store the actual number of devices
        InputState *inputs = (InputState *) calloc((nDevices + 1), sizeof(InputState));

        if (nDevices == 0) {
            free(pRawInputDeviceList);

            return inputs;
        }

        uint32 cb_size = 256;

        for (uint32 i = 0; i < nDevices; ++i) {
            GetRawInputDeviceInfoA(pRawInputDeviceList[i].hDevice, RIDI_DEVICENAME, inputs[i].name, &cb_size);

            cb_size = sizeof(RID_DEVICE_INFO);
            RID_DEVICE_INFO rdi;
            GetRawInputDeviceInfoA(pRawInputDeviceList[i].hDevice, RIDI_DEVICEINFO, &rdi, &cb_size);

            switch (rdi.dwType) {
                case RIM_TYPEMOUSE: {
                        inputs[i].handle_mouse = pRawInputDeviceList[i].hDevice;
                        inputs[i].is_connected = true;
                        inputs[i].type = INPUT_TYPE_MOUSE;
                    } break;
                case RIM_TYPEKEYBOARD: {
                        inputs[i].handle_keyboard = pRawInputDeviceList[i].hDevice;
                        inputs[i].is_connected = true;
                        inputs[i].type = INPUT_TYPE_KEYBOARD;
                    } break;
                case RIM_TYPEHID: {
                        inputs[i].type = INPUT_TYPE_OTHER;
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
        rid[0].hwndTarget  = w->hwnd;

        // Joystick
        rid[1].usUsagePage = 0x01; // @todo doesn't work with 0x05 for games?
        rid[1].usUsage	   = 0x04;
        rid[1].dwFlags     = RIDEV_DEVNOTIFY;
        rid[1].hwndTarget = w->hwnd;

        // Gamepad
        rid[2].usUsagePage = 0x01; // @todo doesn't work with 0x05 for games?
        rid[2].usUsage	   = 0x05;
        rid[2].dwFlags     = RIDEV_DEVNOTIFY;
        rid[2].hwndTarget = w->hwnd;

        // Keyboard
        rid[3].usUsagePage = 0x01; // @todo doesn't work with 0x05 for games?
        rid[3].usUsage	   = 0x06;
        rid[3].dwFlags     = RIDEV_DEVNOTIFY;
        rid[3].hwndTarget = w->hwnd;

        if (!RegisterRawInputDevices((PCRAWINPUTDEVICE) rid, 4, sizeof(RAWINPUTDEVICE))) {
            // @todo Log
        }

        // free(rid);
        free(pRawInputDeviceList);

        return inputs;
    }

    void handle_input(LPARAM lParam, InputState* states)
    {
        uint32 db_size;
        GetRawInputData((HRAWINPUT)lParam, RID_INPUT, NULL, &db_size, sizeof(RAWINPUTHEADER));

        // @todo pull out, we only need to register this memory once
        //      Maybe even put it into the general memory pool
        LPBYTE lpb = (BYTE *) malloc(db_size * sizeof(BYTE));

        GetRawInputData((HRAWINPUT)lParam, RID_INPUT, lpb, &db_size, sizeof(RAWINPUTHEADER));

        RAWINPUT* raw = (RAWINPUT*) lpb;

        uint32 i = 0;
        if (raw->header.dwType == RIM_TYPEMOUSE) {
            // @todo Change so we can directly access the correct state (maybe map handle address to index?)
            while (states[i].is_connected && states[i].handle_mouse != raw->header.hDevice) {++i;}

            if (!states[i].is_connected) {
                return;
            }

            // https://learn.microsoft.com/en-us/windows/win32/api/winuser/ns-winuser-rawmouse
            if (raw->data.mouse.usFlags & MOUSE_MOVE_ABSOLUTE) {
                RECT rect;
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

                states[i].x_last = states[i].x;
                states[i].y_last = states[i].y;

                states[i].x = MulDiv(raw->data.mouse.lLastX, rect.right, 65535) + rect.left;
                states[i].y = MulDiv(raw->data.mouse.lLastY, rect.bottom, 65535) + rect.top;

                states[i].state_change_mouse = true;
            } else if (raw->data.mouse.lLastX != 0 || raw->data.mouse.lLastY != 0) {
                states[i].x_last = states[i].x;
                states[i].y_last = states[i].y;

                states[i].x = states[i].x + raw->data.mouse.lLastX;
                states[i].y = states[i].y + raw->data.mouse.lLastY;

                states[i].state_change_mouse = true;
            }
        } else if (raw->header.dwType == RIM_TYPEKEYBOARD) {
            // @todo Change so we can directly access the correct state (maybe map handle address to index?)
            while (states[i].is_connected && states[i].handle_keyboard != raw->header.hDevice) {++i;}

            if (!states[i].is_connected) {
                return;
            }

            // https://learn.microsoft.com/en-us/windows/win32/api/winuser/ns-winuser-rawkeyboard

            RAWKEYBOARD rawKB = raw->data.keyboard;

            states[i].key = raw->data.keyboard.MakeCode;
            states[i].key_up = raw->data.keyboard.Flags & RI_KEY_BREAK;
            states[i].key_down = raw->data.keyboard.Flags & RI_KEY_MAKE;

            if (states[i].key_down) {
                for (int j = 0; j < MAX_KEY_PRESSES; ++j) {
                    if (states[i].keys_down[j] == NULL) {
                        states[i].keys_down[j] = states[i].key;
                    }
                }
            } else if (states[i].key_up) {
                for (int j = 0; j < MAX_KEY_PRESSES; ++j) {
                    if (states[i].keys_down[j] == states[i].key) {
                        states[i].keys_down[j] = NULL;
                    }
                }
            }

            states[i].state_change_keyboard = true;
        }
    }
}

#endif