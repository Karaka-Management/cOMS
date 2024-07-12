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

#define MAX_KEY_PRESSES 4
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

struct ControllerState {
    uint32 id = 0;
    bool is_connected = false;

    // After handling the state change the game loop should set this to false
    bool state_change = false;

    bool up = false;
    bool down = false;
    bool left = false;
    bool right = false;
    bool start = false;
    bool back = false;

    bool shoulder_left = false;
    bool shoulder_right = false;

    byte trigger_left = 0;
    byte trigger_right = 0;

    bool button_a = false;
    bool button_b = false;
    bool button_x = false;
    bool button_y = false;

    int16 stickl_x = 0;
    int16 stickl_y = 0;
    bool stickl_press = false;

    int16 stickr_x = 0;
    int16 stickr_y = 0;
    bool stickr_press = false;
};

#endif