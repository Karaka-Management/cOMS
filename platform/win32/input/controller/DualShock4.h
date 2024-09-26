/**
 * Jingga
 *
 * @copyright Jingga
 * @license   OMS License 2.0
 * @version   1.0.0
 * @link      https://jingga.app
 */
#ifndef TOS_PLATFORM_WIN32_INPUT_CONTROLLER_DUALSHOCK4_H
#define TOS_PLATFORM_WIN32_INPUT_CONTROLLER_DUALSHOCK4_H

#include "../../../../stdlib/Types.h"
#include "../../../../input/ControllerInput.h"
#include "../../../../utils/BitUtils.h"

inline
void input_map_dualshock4(ControllerInput* controller, byte* data)
{
    ++data;

    // 0 is not the origin -> need to shift
    controller->button[CONTROLLER_BUTTON_STICK_LEFT_HORIZONTAL] = *data++;
    controller->button[CONTROLLER_BUTTON_STICK_LEFT_HORIZONTAL] = OMS_MIN(controller->button[CONTROLLER_BUTTON_STICK_LEFT_HORIZONTAL] - 128, 127);
    controller->is_analog[CONTROLLER_BUTTON_STICK_LEFT_HORIZONTAL] = true;

    controller->button[CONTROLLER_BUTTON_STICK_LEFT_VERTICAL] = *data++;
    controller->button[CONTROLLER_BUTTON_STICK_LEFT_VERTICAL] = OMS_MIN(controller->button[CONTROLLER_BUTTON_STICK_LEFT_VERTICAL] - 128, 127);
    controller->is_analog[CONTROLLER_BUTTON_STICK_LEFT_VERTICAL] = true;

    controller->button[CONTROLLER_BUTTON_STICK_RIGHT_HORIZONTAL] = *data++;
    controller->button[CONTROLLER_BUTTON_STICK_RIGHT_HORIZONTAL] = OMS_MIN(controller->button[CONTROLLER_BUTTON_STICK_RIGHT_HORIZONTAL] - 128, 127);
    controller->is_analog[CONTROLLER_BUTTON_STICK_RIGHT_HORIZONTAL] = true;

    controller->button[CONTROLLER_BUTTON_STICK_RIGHT_VERTICAL] = *data++;
    controller->button[CONTROLLER_BUTTON_STICK_RIGHT_VERTICAL] = OMS_MIN(controller->button[CONTROLLER_BUTTON_STICK_RIGHT_VERTICAL] - 128, 127);
    controller->is_analog[CONTROLLER_BUTTON_STICK_RIGHT_VERTICAL] = true;

    controller->button[CONTROLLER_BUTTON_T] = BITS_GET_8_L2R(*data, 0, 1);
    controller->button[CONTROLLER_BUTTON_C] = BITS_GET_8_L2R(*data, 1, 1);
    controller->button[CONTROLLER_BUTTON_X] = BITS_GET_8_L2R(*data, 2, 1);
    controller->button[CONTROLLER_BUTTON_S] = BITS_GET_8_L2R(*data, 3, 1);

    uint32 d_pad_state = BITS_GET_8_L2R(*data, 4, 4);
    if (d_pad_state == 8) {
        controller->button[CONTROLLER_BUTTON_DPAD_LEFT] = 0;
        controller->button[CONTROLLER_BUTTON_DPAD_RIGHT] = 0;
        controller->button[CONTROLLER_BUTTON_DPAD_UP] = 0;
        controller->button[CONTROLLER_BUTTON_DPAD_DOWN] = 0;
    } else if (d_pad_state == 0) {
        controller->button[CONTROLLER_BUTTON_DPAD_LEFT] = 0;
        controller->button[CONTROLLER_BUTTON_DPAD_RIGHT] = 0;
        controller->button[CONTROLLER_BUTTON_DPAD_UP] = 127;
        controller->button[CONTROLLER_BUTTON_DPAD_DOWN] = 0;
    } else if (d_pad_state == 1) {
        controller->button[CONTROLLER_BUTTON_DPAD_LEFT] = 0;
        controller->button[CONTROLLER_BUTTON_DPAD_RIGHT] = 127;
        controller->button[CONTROLLER_BUTTON_DPAD_UP] = 127;
        controller->button[CONTROLLER_BUTTON_DPAD_DOWN] = 0;
    } else if (d_pad_state == 2) {
        controller->button[CONTROLLER_BUTTON_DPAD_LEFT] = 0;
        controller->button[CONTROLLER_BUTTON_DPAD_RIGHT] = 127;
        controller->button[CONTROLLER_BUTTON_DPAD_UP] = 0;
        controller->button[CONTROLLER_BUTTON_DPAD_DOWN] = 0;
    } else if (d_pad_state == 3) {
        controller->button[CONTROLLER_BUTTON_DPAD_LEFT] = 0;
        controller->button[CONTROLLER_BUTTON_DPAD_RIGHT] = 127;
        controller->button[CONTROLLER_BUTTON_DPAD_UP] = 0;
        controller->button[CONTROLLER_BUTTON_DPAD_DOWN] = 127;
    } else if (d_pad_state == 4) {
        controller->button[CONTROLLER_BUTTON_DPAD_LEFT] = 0;
        controller->button[CONTROLLER_BUTTON_DPAD_RIGHT] = 0;
        controller->button[CONTROLLER_BUTTON_DPAD_UP] = 0;
        controller->button[CONTROLLER_BUTTON_DPAD_DOWN] = 127;
    } else if (d_pad_state == 5) {
        controller->button[CONTROLLER_BUTTON_DPAD_LEFT] = 127;
        controller->button[CONTROLLER_BUTTON_DPAD_RIGHT] = 0;
        controller->button[CONTROLLER_BUTTON_DPAD_UP] = 0;
        controller->button[CONTROLLER_BUTTON_DPAD_DOWN] = 127;
    } else if (d_pad_state == 6) {
        controller->button[CONTROLLER_BUTTON_DPAD_LEFT] = 127;
        controller->button[CONTROLLER_BUTTON_DPAD_RIGHT] = 0;
        controller->button[CONTROLLER_BUTTON_DPAD_UP] = 0;
        controller->button[CONTROLLER_BUTTON_DPAD_DOWN] = 0;
    } else if (d_pad_state == 7) {
        controller->button[CONTROLLER_BUTTON_DPAD_LEFT] = 127;
        controller->button[CONTROLLER_BUTTON_DPAD_RIGHT] = 0;
        controller->button[CONTROLLER_BUTTON_DPAD_UP] = 127;
        controller->button[CONTROLLER_BUTTON_DPAD_DOWN] = 0;
    }

    ++data;

    controller->button[CONTROLLER_BUTTON_STICK_LEFT_BUTTON] = BITS_GET_8_L2R(*data, 0, 1);
    controller->button[CONTROLLER_BUTTON_STICK_RIGHT_BUTTON] = BITS_GET_8_L2R(*data, 1, 1);

    controller->button[CONTROLLER_BUTTON_OTHER_0] = BITS_GET_8_L2R(*data, 2, 1); // option
    controller->button[CONTROLLER_BUTTON_OTHER_1] = BITS_GET_8_L2R(*data, 3, 1); // share

    controller->button[CONTROLLER_BUTTON_SHOULDER_RIGHT_BUTTON] = BITS_GET_8_L2R(*data, 6, 1);
    controller->button[CONTROLLER_BUTTON_SHOULDER_LEFT_BUTTON] = BITS_GET_8_L2R(*data, 7, 1);

    ++data;

    controller->button[CONTROLLER_BUTTON_OTHER_2] = BITS_GET_8_L2R(*data, 6, 1); // tpad
    controller->button[CONTROLLER_BUTTON_OTHER_3] = BITS_GET_8_L2R(*data, 7, 1); // ps

    ++data;

    controller->button[CONTROLLER_BUTTON_SHOULDER_RIGHT_TRIGGER] = *data++;
    controller->is_analog[CONTROLLER_BUTTON_SHOULDER_RIGHT_TRIGGER] = true;

    controller->button[CONTROLLER_BUTTON_SHOULDER_LEFT_TRIGGER] = *data++;
    controller->is_analog[CONTROLLER_BUTTON_SHOULDER_LEFT_TRIGGER] = true;

    data += 3;

    controller->gyro_x = *data++;
    controller->gyro_y = *data++;
    controller->gyro_z = *data++;
}

#endif