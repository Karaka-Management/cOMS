/**
 * Jingga
 *
 * @copyright Jingga
 * @license   OMS License 2.0
 * @version   1.0.0
 * @link      https://jingga.app
 */
#ifndef TOS_PLATFORM_WIN32_INPUT_CONTROLLER_CONTROLLER_INPUT_H
#define TOS_PLATFORM_WIN32_INPUT_CONTROLLER_CONTROLLER_INPUT_H

#include "../stdlib/Types.h"

enum ControllerButton {
    CONTROLLER_BUTTON_NONE, // Needs to be skipped for input system (see -1)

    CONTROLLER_BUTTON_STICK_LEFT_BUTTON,
    CONTROLLER_BUTTON_STICK_LEFT_HORIZONTAL,
    CONTROLLER_BUTTON_STICK_LEFT_VERTOCAL,

    CONTROLLER_BUTTON_STICK_RIGHT_BUTTON,
    CONTROLLER_BUTTON_STICK_RIGHT_HORIZONTAL,
    CONTROLLER_BUTTON_STICK_RIGHT_VERTOCAL,

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
};

struct ControllerInput {
    uint8 stick_left_button;
    int8 stick_left_x;
    int8 stick_left_y;

    uint8 stick_right_button;
    int8 stick_right_x;
    int8 stick_right_y;

    int8 shoulder_trigger_left;
    int8 shoulder_trigger_right;

    uint8 shoulder_button_left;
    uint8 shoulder_button_right;

    uint8 button_X;
    uint8 button_C;
    uint8 button_T;
    uint8 button_S;

    uint8 dpad_left;
    uint8 dpad_right;
    uint8 dpad_up;
    uint8 dpad_down;

    int16 gyro_x;
    int16 gyro_y;
    int16 gyro_z;

    uint8 button_other[8];
};

#endif