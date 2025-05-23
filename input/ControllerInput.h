/**
 * Jingga
 *
 * @copyright Jingga
 * @license   OMS License 2.0
 * @version   1.0.0
 * @link      https://jingga.app
 */
#ifndef COMS_PLATFORM_WIN32_INPUT_CONTROLLER_CONTROLLER_INPUT_H
#define COMS_PLATFORM_WIN32_INPUT_CONTROLLER_CONTROLLER_INPUT_H

#include "../stdlib/Types.h"

#define MAX_CONTROLLER_KEYS 32

enum ControllerButton {
    CONTROLLER_BUTTON_STICK_LEFT_BUTTON,
    CONTROLLER_BUTTON_STICK_LEFT_HORIZONTAL,
    CONTROLLER_BUTTON_STICK_LEFT_VERTICAL,

    CONTROLLER_BUTTON_STICK_RIGHT_BUTTON,
    CONTROLLER_BUTTON_STICK_RIGHT_HORIZONTAL,
    CONTROLLER_BUTTON_STICK_RIGHT_VERTICAL,

    CONTROLLER_BUTTON_SHOULDER_LEFT_TRIGGER,
    CONTROLLER_BUTTON_SHOULDER_LEFT_BUTTON,

    CONTROLLER_BUTTON_SHOULDER_RIGHT_TRIGGER,
    CONTROLLER_BUTTON_SHOULDER_RIGHT_BUTTON,

    CONTROLLER_BUTTON_X,
    CONTROLLER_BUTTON_C,
    CONTROLLER_BUTTON_T,
    CONTROLLER_BUTTON_S,

    CONTROLLER_BUTTON_DPAD_LEFT,
    CONTROLLER_BUTTON_DPAD_RIGHT,
    CONTROLLER_BUTTON_DPAD_UP,
    CONTROLLER_BUTTON_DPAD_DOWN,

    CONTROLLER_BUTTON_OTHER_0,
    CONTROLLER_BUTTON_OTHER_1,
    CONTROLLER_BUTTON_OTHER_2,
    CONTROLLER_BUTTON_OTHER_3,
    CONTROLLER_BUTTON_OTHER_4,
    CONTROLLER_BUTTON_OTHER_5,
    CONTROLLER_BUTTON_OTHER_6,
    CONTROLLER_BUTTON_OTHER_7,

    CONTROLLER_BUTTON_SIZE,
};

struct ControllerInput {
    // @todo should probably include controller_id for xinput and LPDIRECTINPUTDEVICE8 for directinput

    int8 button[MAX_CONTROLLER_KEYS];
    bool is_analog[MAX_CONTROLLER_KEYS]; // = uses deadzone

    int16 gyro_x;
    int16 gyro_y;
    int16 gyro_z;
};

#endif