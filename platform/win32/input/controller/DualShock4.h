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
    controller->stick_left_x = *data++;
    controller->stick_left_x = OMS_MIN(controller->stick_left_x - 128, 127);
    controller->stick_left_y = *data++;
    controller->stick_left_y = OMS_MIN(controller->stick_left_y - 128, 127);

    controller->stick_right_x = *data++;
    controller->stick_right_x = OMS_MIN(controller->stick_right_x - 128, 127);
    controller->stick_right_y = *data++;
    controller->stick_right_y = OMS_MIN(controller->stick_right_y - 128, 127);

    controller->button_T = BITS_GET_8_L2R(*data, 0, 1);
    controller->button_C = BITS_GET_8_L2R(*data, 1, 1);
    controller->button_X = BITS_GET_8_L2R(*data, 2, 1);
    controller->button_S = BITS_GET_8_L2R(*data, 3, 1);

    uint32 d_pad_state = BITS_GET_8_L2R(*data, 4, 4);
    if (d_pad_state == 8) {
        controller->dpad_left = 0;
        controller->dpad_right = 0;
        controller->dpad_up = 0;
        controller->dpad_down = 0;
    } else if (d_pad_state == 0) {
        controller->dpad_left = 0;
        controller->dpad_right = 0;
        controller->dpad_up = 127;
        controller->dpad_down = 0;
    } else if (d_pad_state == 1) {
        controller->dpad_left = 0;
        controller->dpad_right = 127;
        controller->dpad_up = 127;
        controller->dpad_down = 0;
    } else if (d_pad_state == 2) {
        controller->dpad_left = 0;
        controller->dpad_right = 127;
        controller->dpad_up = 0;
        controller->dpad_down = 0;
    } else if (d_pad_state == 3) {
        controller->dpad_left = 0;
        controller->dpad_right = 127;
        controller->dpad_up = 0;
        controller->dpad_down = 127;
    } else if (d_pad_state == 4) {
        controller->dpad_left = 0;
        controller->dpad_right = 0;
        controller->dpad_up = 0;
        controller->dpad_down = 127;
    } else if (d_pad_state == 5) {
        controller->dpad_left = 127;
        controller->dpad_right = 0;
        controller->dpad_up = 0;
        controller->dpad_down = 127;
    } else if (d_pad_state == 6) {
        controller->dpad_left = 127;
        controller->dpad_right = 0;
        controller->dpad_up = 0;
        controller->dpad_down = 0;
    } else if (d_pad_state == 7) {
        controller->dpad_left = 127;
        controller->dpad_right = 0;
        controller->dpad_up = 127;
        controller->dpad_down = 0;
    }

    ++data;

    controller->stick_right_button = BITS_GET_8_L2R(*data, 0, 1);
    controller->stick_left_button = BITS_GET_8_L2R(*data, 1, 1);

    controller->button_other[0] = BITS_GET_8_L2R(*data, 2, 1); // option
    controller->button_other[1] = BITS_GET_8_L2R(*data, 3, 1); // share

    controller->shoulder_button_right = BITS_GET_8_L2R(*data, 6, 1); // tpad
    controller->shoulder_button_left = BITS_GET_8_L2R(*data, 7, 1); // ps

    ++data;

    controller->button_other[2] = BITS_GET_8_L2R(*data, 6, 1);
    controller->button_other[3] = BITS_GET_8_L2R(*data, 7, 1);

    ++data;

    controller->shoulder_trigger_right = *data++;
    controller->shoulder_trigger_left = *data++;

    data += 3;

    controller->gyro_x = *data++;
    controller->gyro_y = *data++;
    controller->gyro_z = *data++;
}

#endif