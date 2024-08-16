/**
 * Jingga
 *
 * @copyright Jingga
 * @license   OMS License 2.0
 * @version   1.0.0
 * @link      https://jingga.app
 */
#ifndef TOS_INPUT_H
#define TOS_INPUT_H

#define MAX_KEY_PRESSES 5
#define MIN_INPUT_DEVICES 2

#define INPUT_TYPE_MOUSE 0x01
#define INPUT_TYPE_KEYBOARD 0x02
#define INPUT_TYPE_OTHER 0x03

#define MIN_CONTROLLER_DEVICES 4

#include "../stdlib/Types.h"

#ifdef _WIN32
    #include <windows.h>
#endif

struct InputState {
    // Device
    bool is_connected = false;
    char name[256];
    byte type = INPUT_TYPE_OTHER;
    double time;

    #ifdef _WIN32
        // @todo maybe replace with id?!
        //      -> remove _WIN32 section
        HANDLE handle_keyboard;
        HANDLE handle_mouse;
    #endif

    // After handling the keyboard state change the game loop should set this to false
    bool state_change_keyboard = false;

    // We only consider up to 4 pressed keys
    // Depending on the keyboard you may only be able to detect a limited amount of key presses anyway
    int up_index;
    uint16 keys_down_old[MAX_KEY_PRESSES];

    int down_index;
    uint16 keys_down[MAX_KEY_PRESSES];

    // Mouse
    // After handling the mouse state change the game loop should set this to false
    bool state_change_mouse = false;

    uint32 x;
    uint32 y;

    uint32 x_last;
    uint32 y_last;

    // https://usb.org/sites/default/files/hid1_11.pdf Page 71 or 61
    // @question consider to use bit field (one int32 would be sufficient)
    bool mouse_down_old[18];
    bool mouse1_down[18];

    int16 wheel_delta = 0;
    uint32 raw_button = 0;
};

struct ControllerState {
    uint32 id = 0;
    bool is_connected = false;

    // After handling the state change the game loop should set this to false
    bool state_change = false;

    // @question maybe make part of button
    bool up = false;
    bool down = false;
    bool left = false;
    bool right = false;

    byte trigger_old[4];
    byte trigger[4];

    // @question consider to use bit field (one int32 would be sufficient)
    bool button_old[10];
    bool button[10];

    int16 stickl_x = 0;
    int16 stickl_y = 0;
    bool stickl_press = false;

    int16 stickr_x = 0;
    int16 stickr_y = 0;
    bool stickr_press = false;
};

#endif