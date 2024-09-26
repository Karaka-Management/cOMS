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

// How many concurrent mouse/secondary input device presses to we recognize
#define MAX_MOUSE_PRESSES 3

// How many concurrent primary key/button presses can be handled?
#define MAX_KEY_PRESSES 5
#define MAX_KEY_STATES (2 * MAX_KEY_PRESSES)

// How many keys/buttons do we support for the devices
#define MAX_KEYBOARD_KEYS 255
#define MAX_MOUSE_KEYS 10
#define MAX_CONTROLLER_KEYS 24

#define MIN_INPUT_DEVICES 2

// How often can a key be asigned to a different hotkey
#define MAX_KEY_TO_HOTKEY 5

// How many buttons together are allowed to form a hotkey
#define MAX_HOTKEY_COMBINATION 3

// These values are used as bit flags to hint if a "key" is a keyboard/primary or mouse/secondary input
// When adding a keybind the "key" can only be uint8 but we expand it to an int and set the first bit accordingly
#define INPUT_MOUSE_PREFIX 0
#define INPUT_KEYBOARD_PREFIX 8192
#define INPUT_CONTROLLER_PREFIX 16384

#define INPUT_TYPE_MOUSE_KEYBOARD 0x01
#define INPUT_TYPE_CONTROLLER 0x02
#define INPUT_TYPE_OTHER 0x03

#define MIN_CONTROLLER_DEVICES 4

#include "../stdlib/Types.h"
#include "../utils/BitUtils.h"
#include "ControllerInput.h"

#ifdef _WIN32
    #include <windows.h>
#endif

// @todo I'm not sure if I like the general input handling
//      Having separate keyboard_down and mouse_down etc. is a little bit weird in the functions below

struct InputMapping {
    // A key/button can be bound to up to 5 different hotkeys
    // This is used to check if a key/button has a hotkey association
    uint8 keys[MAX_MOUSE_KEYS + MAX_KEYBOARD_KEYS + MAX_CONTROLLER_KEYS][MAX_KEY_TO_HOTKEY];

    // A hotkey can be bound to a combination of up to 3 key/button presses
    uint8 hotkey_count;

    // negative hotkeys mean any of them needs to be matched, positive hotkeys means all of them need to be matched
    // mixing positive and negative keys for one hotkey is not possible
    int16* hotkeys;
};

enum KeyState {
    KEY_STATE_PRESSED,
    KEY_STATE_HELD,
    KEY_STATE_RELEASED,
};

struct InputKey {
    // Includes flag for mouse, keyboard, controller
    uint16 key_id;
    uint16 key_state;
    int16 value; // e.g. stick/trigger keys
    uint64 time; // when was this action performed (useful to decide if key state is held vs pressed)
};

// @question Maybe we should also add a third key_down array for controllers and some special controller functions here to just handle everything in one struct
//      Or think about completely splitting all states (mouse, keyboard, other)
struct InputState {
    // State of the hotkeys, resulting from the device input
    uint8 state_hotkeys[MAX_KEY_PRESSES];

    InputKey state_keys[MAX_KEY_STATES];

    int32 dx;
    int32 dy;

    uint32 x;
    uint32 y;

    // Secondary coordinate input (usually used by controllers)
    int32 dx2;
    int32 dy2;

    uint32 x2;
    uint32 y2;

    // Tertiary coordinate input (usually used by controllers)
    int32 dx3;
    int32 dy3;

    uint32 x3;
    uint32 y3;
};

struct Input {
    // Device
    bool is_connected;

    #ifdef _WIN32
        // @question maybe replace with id?!
        //      -> remove _WIN32 section?
        HANDLE handle_keyboard;
        HANDLE handle_mouse;
        HANDLE handle_controller;
    #endif

    bool state_change_button;
    bool state_change_mouse;

    bool mouse_movement;

    InputState state;
    uint64 time_last_input_check;

    uint32 deadzone = 10;

    InputMapping input_mapping1;
    InputMapping input_mapping2;
};

inline
void input_clean_state(InputState* state)
{
    for (int i = 0; i < MAX_KEY_STATES; ++i) {
        if (state->state_keys[i].key_state == KEY_STATE_RELEASED) {
            state->state_keys[i].key_id = 0;
        }
    }
}

inline
bool input_action_exists(const InputState* state, int16 key)
{
    return state->state_keys[0].key_id == key
        || state->state_keys[1].key_id == key
        || state->state_keys[2].key_id == key
        || state->state_keys[3].key_id == key
        || state->state_keys[4].key_id == key
        || state->state_keys[4].key_id == key
        || state->state_keys[5].key_id == key
        || state->state_keys[6].key_id == key
        || state->state_keys[7].key_id == key
        || state->state_keys[8].key_id == key
        || state->state_keys[9].key_id == key;
}

inline
bool input_is_down(const InputState* state, int16 key)
{
    return (state->state_keys[0].key_id == key && state->state_keys[0].key_state != KEY_STATE_RELEASED)
        || (state->state_keys[1].key_id == key && state->state_keys[1].key_state != KEY_STATE_RELEASED)
        || (state->state_keys[2].key_id == key && state->state_keys[2].key_state != KEY_STATE_RELEASED)
        || (state->state_keys[3].key_id == key && state->state_keys[3].key_state != KEY_STATE_RELEASED)
        || (state->state_keys[4].key_id == key && state->state_keys[4].key_state != KEY_STATE_RELEASED)
        || (state->state_keys[4].key_id == key && state->state_keys[4].key_state != KEY_STATE_RELEASED)
        || (state->state_keys[5].key_id == key && state->state_keys[5].key_state != KEY_STATE_RELEASED)
        || (state->state_keys[6].key_id == key && state->state_keys[6].key_state != KEY_STATE_RELEASED)
        || (state->state_keys[7].key_id == key && state->state_keys[7].key_state != KEY_STATE_RELEASED)
        || (state->state_keys[8].key_id == key && state->state_keys[8].key_state != KEY_STATE_RELEASED)
        || (state->state_keys[9].key_id == key && state->state_keys[9].key_state != KEY_STATE_RELEASED);
}

inline
bool input_is_pressed(const InputState* state, int16 key)
{
    return (state->state_keys[0].key_id == key && state->state_keys[0].key_state == KEY_STATE_PRESSED)
        || (state->state_keys[1].key_id == key && state->state_keys[1].key_state == KEY_STATE_PRESSED)
        || (state->state_keys[2].key_id == key && state->state_keys[2].key_state == KEY_STATE_PRESSED)
        || (state->state_keys[3].key_id == key && state->state_keys[3].key_state == KEY_STATE_PRESSED)
        || (state->state_keys[4].key_id == key && state->state_keys[4].key_state == KEY_STATE_PRESSED)
        || (state->state_keys[4].key_id == key && state->state_keys[4].key_state == KEY_STATE_PRESSED)
        || (state->state_keys[5].key_id == key && state->state_keys[5].key_state == KEY_STATE_PRESSED)
        || (state->state_keys[6].key_id == key && state->state_keys[6].key_state == KEY_STATE_PRESSED)
        || (state->state_keys[7].key_id == key && state->state_keys[7].key_state == KEY_STATE_PRESSED)
        || (state->state_keys[8].key_id == key && state->state_keys[8].key_state == KEY_STATE_PRESSED)
        || (state->state_keys[9].key_id == key && state->state_keys[9].key_state == KEY_STATE_PRESSED);
}

inline
bool input_is_held(const InputState* state, int16 key)
{
    return (state->state_keys[0].key_id == key && state->state_keys[0].key_state == KEY_STATE_HELD)
        || (state->state_keys[1].key_id == key && state->state_keys[1].key_state == KEY_STATE_HELD)
        || (state->state_keys[2].key_id == key && state->state_keys[2].key_state == KEY_STATE_HELD)
        || (state->state_keys[3].key_id == key && state->state_keys[3].key_state == KEY_STATE_HELD)
        || (state->state_keys[4].key_id == key && state->state_keys[4].key_state == KEY_STATE_HELD)
        || (state->state_keys[4].key_id == key && state->state_keys[4].key_state == KEY_STATE_HELD)
        || (state->state_keys[5].key_id == key && state->state_keys[5].key_state == KEY_STATE_HELD)
        || (state->state_keys[6].key_id == key && state->state_keys[6].key_state == KEY_STATE_HELD)
        || (state->state_keys[7].key_id == key && state->state_keys[7].key_state == KEY_STATE_HELD)
        || (state->state_keys[8].key_id == key && state->state_keys[8].key_state == KEY_STATE_HELD)
        || (state->state_keys[9].key_id == key && state->state_keys[9].key_state == KEY_STATE_HELD);
}

inline
bool input_is_released(const InputState* state, int16 key)
{
    return (state->state_keys[0].key_id == key && state->state_keys[0].key_state == KEY_STATE_RELEASED)
        || (state->state_keys[1].key_id == key && state->state_keys[1].key_state == KEY_STATE_RELEASED)
        || (state->state_keys[2].key_id == key && state->state_keys[2].key_state == KEY_STATE_RELEASED)
        || (state->state_keys[3].key_id == key && state->state_keys[3].key_state == KEY_STATE_RELEASED)
        || (state->state_keys[4].key_id == key && state->state_keys[4].key_state == KEY_STATE_RELEASED)
        || (state->state_keys[4].key_id == key && state->state_keys[4].key_state == KEY_STATE_RELEASED)
        || (state->state_keys[5].key_id == key && state->state_keys[5].key_state == KEY_STATE_RELEASED)
        || (state->state_keys[6].key_id == key && state->state_keys[6].key_state == KEY_STATE_RELEASED)
        || (state->state_keys[7].key_id == key && state->state_keys[7].key_state == KEY_STATE_RELEASED)
        || (state->state_keys[8].key_id == key && state->state_keys[8].key_state == KEY_STATE_RELEASED)
        || (state->state_keys[9].key_id == key && state->state_keys[9].key_state == KEY_STATE_RELEASED);
}

inline
bool input_was_down(const InputState* state, int16 key)
{
    return (state->state_keys[0].key_id == key && state->state_keys[0].key_state == KEY_STATE_RELEASED)
        || (state->state_keys[1].key_id == key && state->state_keys[1].key_state == KEY_STATE_RELEASED)
        || (state->state_keys[2].key_id == key && state->state_keys[2].key_state == KEY_STATE_RELEASED)
        || (state->state_keys[3].key_id == key && state->state_keys[3].key_state == KEY_STATE_RELEASED)
        || (state->state_keys[4].key_id == key && state->state_keys[4].key_state == KEY_STATE_RELEASED)
        || (state->state_keys[4].key_id == key && state->state_keys[4].key_state == KEY_STATE_RELEASED)
        || (state->state_keys[5].key_id == key && state->state_keys[5].key_state == KEY_STATE_RELEASED)
        || (state->state_keys[6].key_id == key && state->state_keys[6].key_state == KEY_STATE_RELEASED)
        || (state->state_keys[7].key_id == key && state->state_keys[7].key_state == KEY_STATE_RELEASED)
        || (state->state_keys[8].key_id == key && state->state_keys[8].key_state == KEY_STATE_RELEASED)
        || (state->state_keys[9].key_id == key && state->state_keys[9].key_state == KEY_STATE_RELEASED);
}

inline
bool inputs_are_down(
    const InputState* state,
    int16 key0, int16 key1 = 0, int16 key2 = 0, int16 key3 = 0, int16 key4 = 0
) {
    return (key0 != 0 && input_is_down(state, key0))
        && (key1 == 0 || input_is_down(state, key1))
        && (key2 == 0 || input_is_down(state, key2))
        && (key3 == 0 || input_is_down(state, key3))
        && (key4 == 0 || input_is_down(state, key4));
}

// We are binding hotkeys bi-directional
void
input_add_hotkey(
    InputMapping* mapping, uint8 hotkey,
    int32 key0, int32 key1 = 0, int32 key2 = 0
)
{
    int count = 0;

    // Define required keys for hotkey
    if (key0 != 0) {
        // Note: -1 since the hotkeys MUST start at 1 (0 is a special value for empty)
        mapping->hotkeys[(hotkey - 1) * MAX_HOTKEY_COMBINATION] = (int16) key0;
        ++count;
    }

    if (key1 != 0) {
        // Note: -1 since the hotkeys MUST start at 1 (0 is a special value for empty)
        mapping->hotkeys[(hotkey - 1) * MAX_HOTKEY_COMBINATION + count] = (int16) key1;
        ++count;
    }

    if (key2 != 0) {
        // Note: -1 since the hotkeys MUST start at 1 (0 is a special value for empty)
        mapping->hotkeys[(hotkey - 1) * MAX_HOTKEY_COMBINATION + count] = (int16) key2;
    }

    int key0_offset = ((bool) (key0 & INPUT_KEYBOARD_PREFIX)) * MAX_MOUSE_KEYS
        + ((bool) (key0 & INPUT_CONTROLLER_PREFIX)) * (MAX_MOUSE_KEYS + MAX_KEYBOARD_KEYS);

    int key1_offset = ((bool) (key1 & INPUT_KEYBOARD_PREFIX)) * MAX_MOUSE_KEYS
        + ((bool) (key1 & INPUT_CONTROLLER_PREFIX)) * (MAX_MOUSE_KEYS + MAX_KEYBOARD_KEYS);

    int key2_offset = ((bool) (key2 & INPUT_KEYBOARD_PREFIX)) * MAX_MOUSE_KEYS
        + ((bool) (key2 & INPUT_CONTROLLER_PREFIX)) * (MAX_MOUSE_KEYS + MAX_KEYBOARD_KEYS);

    key0 = (key0 & ~(INPUT_KEYBOARD_PREFIX | INPUT_CONTROLLER_PREFIX));
    key1 = (key1 & ~(INPUT_KEYBOARD_PREFIX | INPUT_CONTROLLER_PREFIX));
    key2 = (key2 & ~(INPUT_KEYBOARD_PREFIX | INPUT_CONTROLLER_PREFIX));

    // Bind key to hotkey
    for (int i = 0; i < MAX_KEY_TO_HOTKEY; ++i) {
        if (key0 == 0 && key1 == 0 && key2 == 0) {
            break;
        }

        if (key0 != 0 && mapping->keys[key0 + key0_offset - 1][i] == 0) {
            mapping->keys[key0 + key0_offset - 1][i] = hotkey;
            key0 = 0; // prevent adding same key again
        }

        if (key1 != 0 && mapping->keys[key1 + key1_offset - 1][i] == 0) {
            mapping->keys[key1 + key1_offset - 1][i] = hotkey;
            key1 = 0; // prevent adding same key again
        }

        if (key2 != 0 && mapping->keys[key2 + key2_offset - 1][i] == 0) {
            mapping->keys[key2 + key2_offset - 1][i] = hotkey;
            key2 = 0; // prevent adding same key again
        }
    }
}

inline
bool hotkey_is_active(const InputState* state, uint8 hotkey)
{
    return state->state_hotkeys[0] == hotkey
        || state->state_hotkeys[1] == hotkey
        || state->state_hotkeys[2] == hotkey
        || state->state_hotkeys[3] == hotkey
        || state->state_hotkeys[4] == hotkey;
}

// similar to hotkey_is_active but instead of just performing a lookup in the input_hotkey_state created results
// this is actively checking the current input state (not the hotkey state)
inline
bool hotkey_keys_are_active(const InputState* __restrict state, const InputMapping* __restrict mapping, uint8 hotkey)
{
    int16 key0 = mapping->hotkeys[(hotkey - 1) * MAX_HOTKEY_COMBINATION];
    int16 key1 = mapping->hotkeys[(hotkey - 1) * MAX_HOTKEY_COMBINATION + 1];
    int16 key2 = mapping->hotkeys[(hotkey - 1) * MAX_HOTKEY_COMBINATION + 2];

    // This may seem a little bit confusing but we don't care if a input key is down or up
    // Any state means it was used recently BUT NOT YET HANDLED
    // If it was handled it would've been removed (at least in case of RELEASED)
    // Therefore, if a key has a state -> treat it as if active
    bool is_active = input_action_exists(state, key0);
    if (!is_active || key1 == 0) {
        return is_active || input_action_exists(state, -key0);
    }

    is_active &= input_action_exists(state, key1);
    if (!is_active || key2 == 0) {
        return is_active || input_action_exists(state, -key1);
    }

    return (is_active &= input_action_exists(state, key2)) || input_action_exists(state, -key2);
}

inline
void input_set_state(InputState* state, InputKey* __restrict new_key)
{
    InputKey* free_state = NULL;
    bool action_required = true;

    for (int j = 0; j < MAX_KEY_STATES; ++j) {
        if (!free_state && state->state_keys[j].key_id == 0) {
            free_state = &state->state_keys[j];
        } else if (state->state_keys[j].key_id == new_key->key_id) {
            state->state_keys[j].key_state = new_key->key_state;
            state->state_keys[j].value = new_key->value;
            state->state_keys[j].time = new_key->time;
            action_required = false;
        }
    }

    if (!action_required || !free_state) {
        return;
    }

    free_state->key_id = new_key->key_id;
    free_state->key_state = new_key->key_state;
    free_state->value = new_key->value;
    free_state->time = new_key->time;
}

// Controllers are a little bit special
// We need to manually check the specific buttons and set their key
// Since some controllers are constantly sending data like mad it's not possible to handle them event based
// We need to poll them and then check the old state against this new state (annoying but necessary)
// Mice are fully supported by RawInput and are fairly generalized in terms of their buttons -> no special function needed
inline
void input_set_controller_state(Input* input, ControllerInput* controller, uint64 time)
{
    // Check active keys that might need to be set to inactive
    for (int i = 0; i < MAX_KEY_PRESSES; ++i) {
        if ((input->state.state_keys[i].key_id & INPUT_CONTROLLER_PREFIX)
            && input->state.state_keys[i].key_state != KEY_STATE_RELEASED
        ) {
            uint32 key_id = input->state.state_keys[i].key_id & ~INPUT_CONTROLLER_PREFIX;

            if ((key_id == CONTROLLER_BUTTON_STICK_LEFT_BUTTON && controller->stick_left_button == 0)
                || (key_id == CONTROLLER_BUTTON_STICK_LEFT_HORIZONTAL && OMS_ABS(controller->stick_left_x) < input->deadzone)
                || (key_id == CONTROLLER_BUTTON_STICK_LEFT_VERTOCAL && OMS_ABS(controller->stick_left_y) < input->deadzone)
                || (key_id == CONTROLLER_BUTTON_STICK_RIGHT_BUTTON && controller->stick_right_button == 0)
                || (key_id == CONTROLLER_BUTTON_STICK_RIGHT_HORIZONTAL && OMS_ABS(controller->stick_right_x) < input->deadzone)
                || (key_id == CONTROLLER_BUTTON_STICK_RIGHT_VERTOCAL && OMS_ABS(controller->stick_right_y) < input->deadzone)
                || (key_id == CONTROLLER_BUTTON_SHOULDER_LEFT_TRIGGER && OMS_ABS(controller->shoulder_trigger_left) < input->deadzone)
                || (key_id == CONTROLLER_BUTTON_SHOULDER_LEFT_BUTTON && controller->shoulder_button_left == 0)
                || (key_id == CONTROLLER_BUTTON_SHOULDER_RIGHT_TRIGGER && OMS_ABS(controller->shoulder_trigger_right) < input->deadzone)
                || (key_id == CONTROLLER_BUTTON_SHOULDER_RIGHT_BUTTON && controller->shoulder_button_right == 0)
                || (key_id == CONTROLLER_BUTTON_X && controller->button_X == 0)
                || (key_id == CONTROLLER_BUTTON_C && controller->button_C == 0)
                || (key_id == CONTROLLER_BUTTON_T && controller->button_T == 0)
                || (key_id == CONTROLLER_BUTTON_S && controller->button_S == 0)
                || (key_id == CONTROLLER_BUTTON_DPAD_LEFT && controller->dpad_left == 0)
                || (key_id == CONTROLLER_BUTTON_DPAD_RIGHT && controller->dpad_right == 0)
                || (key_id == CONTROLLER_BUTTON_DPAD_UP && controller->dpad_up == 0)
                || (key_id == CONTROLLER_BUTTON_DPAD_DOWN && controller->dpad_down == 0)
                || (key_id == CONTROLLER_BUTTON_OTHER_0 && controller->button_other[0] == 0)
                || (key_id == CONTROLLER_BUTTON_OTHER_1 && controller->button_other[1] == 0)
                || (key_id == CONTROLLER_BUTTON_OTHER_2 && controller->button_other[2] == 0)
                || (key_id == CONTROLLER_BUTTON_OTHER_3 && controller->button_other[3] == 0)
                || (key_id == CONTROLLER_BUTTON_OTHER_4 && controller->button_other[4] == 0)
                || (key_id == CONTROLLER_BUTTON_OTHER_5 && controller->button_other[5] == 0)
                || (key_id == CONTROLLER_BUTTON_OTHER_6 && controller->button_other[6] == 0)
                || (key_id == CONTROLLER_BUTTON_OTHER_7 && controller->button_other[7] == 0)
            ) {
                input->state.state_keys[i].key_state = KEY_STATE_RELEASED;
            }
        }
    }

    // Special keys
    // @todo this code means we cannot change this behavior (e.g. swap mouse view to dpad, swap sticks, ...)
    if (OMS_ABS(controller->stick_right_x) > input->deadzone) {
        input->state.dx += controller->stick_right_x / 8;
        input->state_change_mouse = true;
    } else {
        input->state.dx = 0;
    }

    if (OMS_ABS(controller->stick_right_y) > input->deadzone) {
        input->state.dy += controller->stick_right_y / 8;
        input->state_change_mouse = true;
    } else {
        input->state.dy = 0;
    }

    if (OMS_ABS(controller->stick_left_x) > input->deadzone) {
        input->state.dx2 += controller->stick_left_x / 8;
        // @todo needs state change flag like mouse?!
    } else {
        input->state.dx2 = 0;
    }

    if (OMS_ABS(controller->stick_left_y) > input->deadzone) {
        input->state.dy2 += controller->stick_left_y / 8;
        input->state.y2 += controller->stick_left_y / 8;
        // @todo needs state change flag like mouse?!
    } else {
        input->state.dy2 = 0;
    }

    // General Keys
    int count = 0;
    InputKey keys[5];

    // @todo this logic below is painful, fix
    if (count < 5 && controller->stick_left_button != 0) {
        keys[count].key_id = CONTROLLER_BUTTON_STICK_LEFT_BUTTON | INPUT_CONTROLLER_PREFIX;
        keys[count].key_state = KEY_STATE_PRESSED;
        keys[count].value = controller->stick_left_button;
        keys[count].time = time;

        ++count;
    }

    if (count < 5 && OMS_ABS(controller->stick_left_x) > input->deadzone) {
        keys[count].key_id = CONTROLLER_BUTTON_STICK_LEFT_HORIZONTAL | INPUT_CONTROLLER_PREFIX;
        keys[count].key_state = KEY_STATE_PRESSED;
        keys[count].value = controller->stick_left_x;
        keys[count].time = time;

        ++count;
    }

    if (count < 5 && OMS_ABS(controller->stick_left_y) > input->deadzone) {
        keys[count].key_id = CONTROLLER_BUTTON_STICK_LEFT_VERTOCAL | INPUT_CONTROLLER_PREFIX;
        keys[count].key_state = KEY_STATE_PRESSED;
        keys[count].value = controller->stick_left_y;
        keys[count].time = time;

        ++count;
    }

    if (count < 5 && controller->stick_right_button != 0) {
        keys[count].key_id = CONTROLLER_BUTTON_STICK_RIGHT_BUTTON | INPUT_CONTROLLER_PREFIX;
        keys[count].key_state = KEY_STATE_PRESSED;
        keys[count].value = controller->stick_right_button;
        keys[count].time = time;

        ++count;
    }

    if (count < 5 && OMS_ABS(controller->stick_right_x) > input->deadzone) {
        keys[count].key_id = CONTROLLER_BUTTON_STICK_RIGHT_HORIZONTAL | INPUT_CONTROLLER_PREFIX;
        keys[count].key_state = KEY_STATE_PRESSED;
        keys[count].value = controller->stick_right_x;
        keys[count].time = time;

        ++count;
    }

    if (count < 5 && OMS_ABS(controller->stick_right_y) > input->deadzone) {
        keys[count].key_id = CONTROLLER_BUTTON_STICK_RIGHT_VERTOCAL | INPUT_CONTROLLER_PREFIX;
        keys[count].key_state = KEY_STATE_PRESSED;
        keys[count].value = controller->stick_right_y;
        keys[count].time = time;

        ++count;
    }

    if (count < 5 && OMS_ABS(controller->shoulder_trigger_left) > input->deadzone) {
        keys[count].key_id = CONTROLLER_BUTTON_SHOULDER_LEFT_TRIGGER | INPUT_CONTROLLER_PREFIX;
        keys[count].key_state = KEY_STATE_PRESSED;
        keys[count].value = controller->shoulder_trigger_left;
        keys[count].time = time;

        ++count;
    }

    if (count < 5 && controller->shoulder_button_left != 0) {
        keys[count].key_id = CONTROLLER_BUTTON_SHOULDER_LEFT_BUTTON | INPUT_CONTROLLER_PREFIX;
        keys[count].key_state = KEY_STATE_PRESSED;
        keys[count].value = controller->shoulder_button_left;
        keys[count].time = time;

        ++count;
    }

    if (count < 5 && OMS_ABS(controller->shoulder_trigger_right) > input->deadzone) {
        keys[count].key_id = CONTROLLER_BUTTON_SHOULDER_RIGHT_TRIGGER | INPUT_CONTROLLER_PREFIX;
        keys[count].key_state = KEY_STATE_PRESSED;
        keys[count].value = controller->shoulder_trigger_right;
        keys[count].time = time;

        ++count;
    }

    if (count < 5 && controller->shoulder_button_right != 0) {
        keys[count].key_id = CONTROLLER_BUTTON_SHOULDER_RIGHT_BUTTON | INPUT_CONTROLLER_PREFIX;
        keys[count].key_state = KEY_STATE_PRESSED;
        keys[count].value = controller->shoulder_button_right;
        keys[count].time = time;

        ++count;
    }

    if (count < 5 && controller->button_X != 0) {
        keys[count].key_id = CONTROLLER_BUTTON_X | INPUT_CONTROLLER_PREFIX;
        keys[count].key_state = KEY_STATE_PRESSED;
        keys[count].value = controller->button_X;
        keys[count].time = time;

        ++count;
    }

    if (count < 5 && controller->button_C != 0) {
        keys[count].key_id = CONTROLLER_BUTTON_C | INPUT_CONTROLLER_PREFIX;
        keys[count].key_state = KEY_STATE_PRESSED;
        keys[count].value = controller->button_C;
        keys[count].time = time;

        ++count;
    }

    if (count < 5 && controller->button_T != 0) {
        keys[count].key_id = CONTROLLER_BUTTON_T | INPUT_CONTROLLER_PREFIX;
        keys[count].key_state = KEY_STATE_PRESSED;
        keys[count].value = controller->button_T;
        keys[count].time = time;

        ++count;
    }

    if (count < 5 && controller->button_S != 0) {
        keys[count].key_id = CONTROLLER_BUTTON_S | INPUT_CONTROLLER_PREFIX;
        keys[count].key_state = KEY_STATE_PRESSED;
        keys[count].value = controller->button_S;
        keys[count].time = time;

        ++count;
    }

    if (count < 5 && controller->dpad_left != 0) {
        keys[count].key_id = CONTROLLER_BUTTON_DPAD_LEFT | INPUT_CONTROLLER_PREFIX;
        keys[count].key_state = KEY_STATE_PRESSED;
        keys[count].value = controller->dpad_left;
        keys[count].time = time;

        ++count;
    }

    if (count < 5 && controller->dpad_right != 0) {
        keys[count].key_id = CONTROLLER_BUTTON_DPAD_RIGHT | INPUT_CONTROLLER_PREFIX;
        keys[count].key_state = KEY_STATE_PRESSED;
        keys[count].value = controller->dpad_right;
        keys[count].time = time;

        ++count;
    }

    if (count < 5 && controller->dpad_up != 0) {
        keys[count].key_id = CONTROLLER_BUTTON_DPAD_UP | INPUT_CONTROLLER_PREFIX;
        keys[count].key_state = KEY_STATE_PRESSED;
        keys[count].value = controller->dpad_up;
        keys[count].time = time;

        ++count;
    }

    if (count < 5 && controller->dpad_down != 0) {
        keys[count].key_id = CONTROLLER_BUTTON_DPAD_DOWN | INPUT_CONTROLLER_PREFIX;
        keys[count].key_state = KEY_STATE_PRESSED;
        keys[count].value = controller->dpad_down;
        keys[count].time = time;

        ++count;
    }

    if (count < 5 && controller->button_other[0] != 0) {
        keys[count].key_id = CONTROLLER_BUTTON_OTHER_0 | INPUT_CONTROLLER_PREFIX;
        keys[count].key_state = KEY_STATE_PRESSED;
        keys[count].value = controller->button_other[0];
        keys[count].time = time;

        ++count;
    }

    if (count < 5 && controller->button_other[1] != 0) {
        keys[count].key_id = CONTROLLER_BUTTON_OTHER_1 | INPUT_CONTROLLER_PREFIX;
        keys[count].key_state = KEY_STATE_PRESSED;
        keys[count].value = controller->button_other[1];
        keys[count].time = time;

        ++count;
    }

    if (count < 5 && controller->button_other[2] != 0) {
        keys[count].key_id = CONTROLLER_BUTTON_OTHER_2 | INPUT_CONTROLLER_PREFIX;
        keys[count].key_state = KEY_STATE_PRESSED;
        keys[count].value = controller->button_other[2];
        keys[count].time = time;

        ++count;
    }

    if (count < 5 && controller->button_other[3] != 0) {
        keys[count].key_id = CONTROLLER_BUTTON_OTHER_3 | INPUT_CONTROLLER_PREFIX;
        keys[count].key_state = KEY_STATE_PRESSED;
        keys[count].value = controller->button_other[3];
        keys[count].time = time;

        ++count;
    }

    if (count < 5 && controller->button_other[4] != 0) {
        keys[count].key_id = CONTROLLER_BUTTON_OTHER_4 | INPUT_CONTROLLER_PREFIX;
        keys[count].key_state = KEY_STATE_PRESSED;
        keys[count].value = controller->button_other[4];
        keys[count].time = time;

        ++count;
    }

    if (count < 5 && controller->button_other[5] != 0) {
        keys[count].key_id = CONTROLLER_BUTTON_OTHER_5 | INPUT_CONTROLLER_PREFIX;
        keys[count].key_state = KEY_STATE_PRESSED;
        keys[count].value = controller->button_other[5];
        keys[count].time = time;

        ++count;
    }

    if (count < 5 && controller->button_other[6] != 0) {
        keys[count].key_id = CONTROLLER_BUTTON_OTHER_6 | INPUT_CONTROLLER_PREFIX;
        keys[count].key_state = KEY_STATE_PRESSED;
        keys[count].value = controller->button_other[6];
        keys[count].time = time;

        ++count;
    }

    if (count < 5 && controller->button_other[7] != 0) {
        keys[count].key_id = CONTROLLER_BUTTON_OTHER_7 | INPUT_CONTROLLER_PREFIX;
        keys[count].key_state = KEY_STATE_PRESSED;
        keys[count].value = controller->button_other[7];
        keys[count].time = time;

        ++count;
    }

    for (int i = 0; i < count; ++i) {
        input_set_state(&input->state, &keys[i]);
    }

    input->state_change_button = true;
}

void
input_hotkey_state(Input* input)
{
    memset(input->state.state_hotkeys, 0, sizeof(uint8) * MAX_KEY_PRESSES);

    int active_hotkeys = 0;

    // Check every key down state
    for (int key_state = 0; key_state < MAX_KEY_STATES; ++key_state) {
        if (input->state.state_keys[key_state].key_id == 0
            || input->state.state_keys[key_state].key_state == KEY_STATE_RELEASED
        ) {
            // no key defined for this down state
            continue;
        }

        // Is a key defined for this state AND is at least one hotkey defined for this key
        //      If no hotkey is defined we don't care
        //      Careful, remember MAX_MOUSE_KEYS offset
        InputKey* key = &input->state.state_keys[key_state];
        int32 internal_key_id = (key->key_id & ~(INPUT_KEYBOARD_PREFIX | INPUT_CONTROLLER_PREFIX))
            + ((bool) (key->key_id & INPUT_KEYBOARD_PREFIX)) * MAX_MOUSE_KEYS
            + ((bool) (key->key_id & INPUT_CONTROLLER_PREFIX)) * (MAX_MOUSE_KEYS + MAX_KEYBOARD_KEYS);

        // Handle 2 input devices (1 = keyboard + mouse, 2 = controller)
        for (int i = 0; i < 2; ++i) {
            InputMapping* mapping;
            if (i == 0) {
                mapping = &input->input_mapping1;
            } else {
                // @todo Maybe we want to ignore < INPUT_CONTROLLER_PREFIX since we could use this to handle alt keybinds
                if (!input->handle_controller || key->key_id < INPUT_CONTROLLER_PREFIX) {
                    continue;
                }

                mapping = &input->input_mapping2;
            }

            const uint8* hotkeys_for_key = mapping->keys[internal_key_id - 1];
            if (hotkeys_for_key[0] == 0) {
                // no possible hotkey associated with this key
                continue;
            }

            // Check every possible hotkey
            // Since multiple input devices have their own button/key indices whe have to do this weird range handling
            for (int possible_hotkey_idx = 0; possible_hotkey_idx < MAX_KEY_TO_HOTKEY; ++possible_hotkey_idx) {
                // We only support a slimited amount of active hotkeys
                if (active_hotkeys >= MAX_KEY_PRESSES) {
                    return;
                }

                bool is_pressed = hotkey_keys_are_active(&input->state, mapping, hotkeys_for_key[possible_hotkey_idx]);

                // store active hotkey, if it is not already active
                if (is_pressed && !hotkey_is_active(&input->state, hotkeys_for_key[possible_hotkey_idx])) {
                    input->state.state_hotkeys[active_hotkeys] = hotkeys_for_key[possible_hotkey_idx];
                    ++active_hotkeys;
                }
            }
        }
    }

    // @bug how to handle long press vs click
    // @bug how to handle priority? e.g. there might be a hotkey for 1 and one for alt+1
    //      in this case only the hotkey for alt+1 should be triggered
    // @bug how to handle other conditions besides buttons pressed together? some hotkeys are only available in certain situations
    // @bug how to handle alternative hotkeys (e.g. keyboard and controller at the same time)
    // @bug how to handle values (e.g. stick may or may not set the x/y or dx/dy in some situations)
}

#endif