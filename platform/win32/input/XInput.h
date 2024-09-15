/**
 * Jingga
 *
 * @copyright Jingga
 * @license   OMS License 2.0
 * @version   1.0.0
 * @link      https://jingga.app
 */
#ifndef TOS_INPUT_XINPUT_H
#define TOS_INPUT_XINPUT_H

#include <XInput.h>
#include <windows.h>

#include "../../../input/Input.h"
#include "../../../stdlib/Types.h"
#include "../../../utils/MathUtils.h"

// @todo consider to remove some global_persist and defines since we are never calling it somewhere else

// BEGIN: Dynamically load XInput
typedef DWORD WINAPI x_input_get_state(DWORD, XINPUT_STATE*);
DWORD WINAPI XInputGetStateStub(DWORD, XINPUT_STATE*) {
    return 0;
}
global_persist x_input_get_state* XInputGetState_ = XInputGetStateStub;
#define XInputGetState XInputGetState_

typedef DWORD WINAPI x_input_set_state(DWORD, XINPUT_VIBRATION*);
DWORD WINAPI XInputSetStateStub(DWORD, XINPUT_VIBRATION*) {
    return 0;
}
global_persist x_input_set_state* XInputSetState_ = XInputSetStateStub;
#define XInputSetState XInputSetState_

void xinput_load() {
    HMODULE lib = LoadLibraryExA((LPCSTR) "xinput1_4.dll", NULL, LOAD_LIBRARY_SEARCH_SYSTEM32);
    if(!lib) {
        // @todo Log
        lib = LoadLibraryExA((LPCSTR) "xinput1_3.dll", NULL, LOAD_LIBRARY_SEARCH_SYSTEM32);
    }

    if (!lib) {
        // @todo Log
        return;
    }

    XInputGetState = (x_input_get_state *) GetProcAddress(lib, "XInputGetState");
    XInputSetState = (x_input_set_state *) GetProcAddress(lib, "XInputSetState");

    if (!XInputGetState || !XInputSetState) {
        // @todo Log
        return;
    }
}
// END: Dynamically load XInput

ControllerState* init_controllers()
{
    uint32 c = 0;
    for (uint32 controller_index = 0; controller_index < XUSER_MAX_COUNT; ++controller_index) {
        XINPUT_STATE controller_state;
        if (XInputGetState(controller_index, &controller_state) == ERROR_SUCCESS) {
            ++c;
        }
    }

    // We always want at least one empty controller slot
    // @todo Change so that we store the actual number of devices
    ControllerState *controllers = (ControllerState *) calloc((c + 1), sizeof(ControllerState));

    if (c == 0) {
        return controllers;
    }

    c = 0;
    for (uint32 controller_index = 0; controller_index < XUSER_MAX_COUNT; ++controller_index) {
        XINPUT_STATE controller_state;
        if (XInputGetState(controller_index, &controller_state) == ERROR_SUCCESS) {
            ++c;

            controllers[c].id = controller_index;
            controllers[c].is_connected = true;
        }
    }

    return controllers;
}

void handle_controller_input(ControllerState* states)
{
    uint32 controller_index = 0;
    while(states[controller_index].is_connected) {
        XINPUT_STATE controller_state;
        if (XInputGetState(controller_index, &controller_state) != ERROR_SUCCESS) {
            ++controller_index;

            continue;
        }

        /*
        states[controller_index].up = controller_state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP;
        states[controller_index].down = controller_state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN;
        states[controller_index].left = controller_state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT;
        states[controller_index].right = controller_state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT;
        states[controller_index].button[6] = controller_state.Gamepad.wButtons & XINPUT_GAMEPAD_START;
        states[controller_index].button[7] = controller_state.Gamepad.wButtons & XINPUT_GAMEPAD_BACK;

        states[controller_index].button[4] = controller_state.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER;
        states[controller_index].button[5] = controller_state.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER;

        states[controller_index].trigger[0] = controller_state.Gamepad.bLeftTrigger;
        states[controller_index].trigger[1] = controller_state.Gamepad.bRightTrigger;

        states[controller_index].button[0] = controller_state.Gamepad.wButtons & XINPUT_GAMEPAD_A;
        states[controller_index].button[1] = controller_state.Gamepad.wButtons & XINPUT_GAMEPAD_B;
        states[controller_index].button[2] = controller_state.Gamepad.wButtons & XINPUT_GAMEPAD_X;
        states[controller_index].button[3] = controller_state.Gamepad.wButtons & XINPUT_GAMEPAD_Y;

        states[controller_index].stickl_x = controller_state.Gamepad.sThumbLX;
        states[controller_index].stickl_y = controller_state.Gamepad.sThumbLY;
        states[controller_index].stickl_press = controller_state.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_THUMB;

        states[controller_index].stickr_x = controller_state.Gamepad.sThumbRX;
        states[controller_index].stickr_y = controller_state.Gamepad.sThumbRY;
        states[controller_index].stickr_press = controller_state.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_THUMB;
        */

        ++controller_index;
    }
}

#endif