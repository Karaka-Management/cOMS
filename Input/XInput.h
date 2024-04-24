/**
 * Karaka
 *
 * @package   Stdlib
 * @copyright Dennis Eichhorn
 * @license   OMS License 1.0
 * @version   1.0.0
 * @link      https://jingga.app
 */
#ifndef INPUT_XINPUT_H
#define INPUT_XINPUT_H

#ifdef _WIN32
    #include <XInput.h>
#else
    #include <linux/joystick.h>
#endif

#include "../Stdlib/Types.h"

uint32 find_joysticks()
{

}

void destory_joysticks()
{

}

void handle_controller_input()
{

    for (uint32 controller_index = 0; controller_index < XUSER_MAX_COUNT; ++controller_index) {
        XINPUT_STATE controller_state;
        if (XInputGetState(controller_index, &controller_state) == ERROR_SUCCESS) {

        } else {

        }
    }
}

#endif