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

#include "../stdlib/Types.h"
#include "../utils/BitUtils.h"
#include "ControllerInput.h"

// How many concurrent mouse/secondary input device presses to we recognize
#define MAX_MOUSE_PRESSES 3

// How many concurrent primary key/button presses can be handled?
#define MAX_KEY_PRESSES 5
#define MAX_KEY_STATES (2 * MAX_KEY_PRESSES)

// How many keys/buttons do we support for the devices
#define MAX_KEYBOARD_KEYS 255
#define MAX_MOUSE_KEYS 10

#define MIN_INPUT_DEVICES 2

// How often can a key be asigned to a different hotkey
#define MAX_KEY_TO_HOTKEY 5
#define MEX_KEY_LENGTH ((MAX_MOUSE_KEYS + MAX_KEYBOARD_KEYS + MAX_CONTROLLER_KEYS) * MAX_KEY_TO_HOTKEY)

// How many buttons together are allowed to form a hotkey
#define MAX_HOTKEY_COMBINATION 3

// These values are used as bit flags to hint32 if a "key" is a keyboard/primary or mouse/secondary input
// When adding a keybind the "key" can only be uint8 but we expand it to an int and set the first bit accordingly
#define INPUT_MOUSE_PREFIX 0
#define INPUT_KEYBOARD_PREFIX 8192
#define INPUT_CONTROLLER_PREFIX 16384

#define INPUT_TYPE_MOUSE_KEYBOARD 0x01
#define INPUT_TYPE_CONTROLLER 0x02
#define INPUT_TYPE_OTHER 0x03

#define MIN_CONTROLLER_DEVICES 4

#define INPUT_LONG_PRESS_DURATION 250

#ifdef _WIN32
    #include <windows.h>
#endif

typedef void (*InputCallback)(void* data);

// @todo I'm not sure if I like the general input handling
//      Having separate keyboard_down and mouse_down etc. is a little bit weird in the functions below

struct InputMapping {
    // A key/button can be bound to up to 5 different hotkeys
    // This is used to check if a key/button has a hotkey association
    uint8 keys[MEX_KEY_LENGTH];

    // A hotkey can be bound to a combination of up to 3 key/button presses
    uint8 hotkey_count;

    // negative hotkeys mean any of them needs to be matched, positive hotkeys means all of them need to be matched
    // mixing positive and negative keys for one hotkey is not possible
    // index = hotkey, value = key id
    int16* hotkeys;
    InputCallback* callbacks;
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
    int16 value; // e.g. stick/trigger keys have additional values
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

    // Do we want to capture mouse events = true,
    //  or do we want to poll the position whenever needed = false
    bool mouse_movement;

    InputState state;
    uint64 time_last_input_check;

    uint32 deadzone = 10;

    // This data is passed to the hotkey callback
    void* callback_data;

    InputMapping input_mapping1;
    InputMapping input_mapping2;
};

inline
void input_init(Input* input, uint8 size, void* callback_data, BufferMemory* buf)
{
    // Init input
    input->callback_data = callback_data;

    // Init mapping1
    input->input_mapping1.hotkey_count = size;

    input->input_mapping1.hotkeys = (int16 *) buffer_get_memory(
        buf,
        input->input_mapping1.hotkey_count * MAX_HOTKEY_COMBINATION * sizeof(int16)
    );

    input->input_mapping1.callbacks = (InputCallback *) buffer_get_memory(
        buf,
        input->input_mapping1.hotkey_count * sizeof(InputCallback),
        0, true
    );

    // Init mapping2
    input->input_mapping2.hotkey_count = size;

    input->input_mapping2.hotkeys = (int16 *) buffer_get_memory(
        buf,
        input->input_mapping2.hotkey_count * MAX_HOTKEY_COMBINATION * sizeof(int16)
    );

    input->input_mapping2.callbacks = (InputCallback *) buffer_get_memory(
        buf,
        input->input_mapping2.hotkey_count * sizeof(InputCallback),
        0, true
    );
}

inline
void input_clean_state(InputState* state)
{
    for (int32 i = 0; i < MAX_KEY_STATES; ++i) {
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

void input_add_callback(InputMapping* mapping, uint8 hotkey, InputCallback callback)
{
    mapping->callbacks[hotkey] = callback;
}

// We are binding hotkeys bi-directional
void
input_add_hotkey(
    InputMapping* mapping, uint8 hotkey,
    int32 key0, int32 key1 = 0, int32 key2 = 0
)
{
    int32 count = 0;

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

    if (key0 < 0) {
        key0 *= -1;
    }

    if (key1 < 0) {
        key1 *= -1;
    }

    if (key2 < 0) {
        key2 *= -1;
    }

    int32 key0_offset = ((bool) (key0 & INPUT_KEYBOARD_PREFIX)) * MAX_MOUSE_KEYS
        + ((bool) (key0 & INPUT_CONTROLLER_PREFIX)) * (MAX_MOUSE_KEYS + MAX_KEYBOARD_KEYS);

    int32 key1_offset = ((bool) (key1 & INPUT_KEYBOARD_PREFIX)) * MAX_MOUSE_KEYS
        + ((bool) (key1 & INPUT_CONTROLLER_PREFIX)) * (MAX_MOUSE_KEYS + MAX_KEYBOARD_KEYS);

    int32 key2_offset = ((bool) (key2 & INPUT_KEYBOARD_PREFIX)) * MAX_MOUSE_KEYS
        + ((bool) (key2 & INPUT_CONTROLLER_PREFIX)) * (MAX_MOUSE_KEYS + MAX_KEYBOARD_KEYS);

    key0 = (key0 & ~(INPUT_KEYBOARD_PREFIX | INPUT_CONTROLLER_PREFIX));
    key1 = (key1 & ~(INPUT_KEYBOARD_PREFIX | INPUT_CONTROLLER_PREFIX));
    key2 = (key2 & ~(INPUT_KEYBOARD_PREFIX | INPUT_CONTROLLER_PREFIX));

    // Bind key to hotkey
    for (int32 i = 0; i < MAX_KEY_TO_HOTKEY; ++i) {
        if (key0 == 0 && key1 == 0 && key2 == 0) {
            break;
        }

        if (key0 != 0 && mapping->keys[(key0 + key0_offset - 1) * MAX_KEY_TO_HOTKEY + i] == 0) {
            mapping->keys[(key0 + key0_offset - 1) * MAX_KEY_TO_HOTKEY + i] = hotkey;
            key0 = 0; // prevent adding same key again
        }

        if (key1 != 0 && mapping->keys[(key1 + key1_offset - 1) * MAX_KEY_TO_HOTKEY + i] == 0) {
            mapping->keys[(key1 + key1_offset - 1) * MAX_KEY_TO_HOTKEY + i] = hotkey;
            key1 = 0; // prevent adding same key again
        }

        if (key2 != 0 && mapping->keys[(key2 + key2_offset - 1) * MAX_KEY_TO_HOTKEY + i] == 0) {
            mapping->keys[(key2 + key2_offset - 1) * MAX_KEY_TO_HOTKEY + i] = hotkey;
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
bool hotkey_keys_are_active(const InputState* state, const InputMapping* mapping, uint8 hotkey)
{
    int16 key0 = mapping->hotkeys[(hotkey - 1) * MAX_HOTKEY_COMBINATION];
    int16 key1 = mapping->hotkeys[(hotkey - 1) * MAX_HOTKEY_COMBINATION + 1];
    int16 key2 = mapping->hotkeys[(hotkey - 1) * MAX_HOTKEY_COMBINATION + 2];

    // This may seem a little bit confusing but we don't care if a input key is down or up
    // Any state means it was used recently BUT NOT YET HANDLED
    // If it was handled it would've been removed (at least in case of RELEASED)
    // Therefore, if a key has a state -> treat it as if active

    // The code below also allows optional keys which have a negative sign (at least one of the optional keys must be valid)
    bool is_active = input_action_exists(state, (int16) OMS_ABS(key0));
    if ((!is_active && (key0 > 0 || key1 >= 0)) || (is_active && key0 < 0)) {
        return is_active;
    }

    is_active = input_action_exists(state, (int16) OMS_ABS(key1));
    if ((!is_active && (key1 > 0 || key2 >= 0)) || (is_active && key1 < 0)) {
        return is_active;
    }

    return input_action_exists(state, (int16) OMS_ABS(key2));
}

inline
void input_set_state(InputState* state, InputKey* __restrict new_key)
{
    InputKey* free_state = NULL;
    bool action_required = true;

    for (int32 i = 0; i < MAX_KEY_STATES; ++i) {
        if (!free_state && state->state_keys[i].key_id == 0) {
            free_state = &state->state_keys[i];
        } else if (state->state_keys[i].key_id == new_key->key_id) {
            state->state_keys[i].key_state = new_key->key_state;
            state->state_keys[i].value = new_key->value;
            state->state_keys[i].time = new_key->time;
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
    for (int32 i = 0; i < MAX_KEY_PRESSES; ++i) {
        if ((input->state.state_keys[i].key_id & INPUT_CONTROLLER_PREFIX)
            && input->state.state_keys[i].key_state != KEY_STATE_RELEASED
        ) {
            uint32 key_id = input->state.state_keys[i].key_id & ~INPUT_CONTROLLER_PREFIX;

            if ((controller->is_analog[key_id] && OMS_ABS(controller->button[key_id]) < input->deadzone)
                || (!controller->is_analog[key_id] && controller->button[key_id] == 0)
            ) {
                input->state.state_keys[i].key_state = KEY_STATE_RELEASED;
            }
        }
    }

    // Special keys
    // @todo this code means we cannot change this behavior (e.g. swap mouse view to dpad, swap sticks, ...)
    // @todo This is also not very general, maybe we can fix it like we did with analog vs digital key (instead of bool flag maybe bit flag)
    if (OMS_ABS(controller->button[CONTROLLER_BUTTON_STICK_RIGHT_HORIZONTAL]) > input->deadzone) {
        input->state.dx += controller->button[CONTROLLER_BUTTON_STICK_RIGHT_HORIZONTAL] / 8;
        input->state_change_mouse = true;
    } else {
        input->state.dx = 0;
    }

    if (OMS_ABS(controller->button[CONTROLLER_BUTTON_STICK_RIGHT_VERTICAL]) > input->deadzone) {
        input->state.dy += controller->button[CONTROLLER_BUTTON_STICK_RIGHT_VERTICAL] / 8;
        input->state_change_mouse = true;
    } else {
        input->state.dy = 0;
    }

    if (OMS_ABS(controller->button[CONTROLLER_BUTTON_STICK_LEFT_HORIZONTAL]) > input->deadzone) {
        input->state.dx2 += controller->button[CONTROLLER_BUTTON_STICK_LEFT_HORIZONTAL] / 8;
        // @todo needs state change flag like mouse?!
    } else {
        input->state.dx2 = 0;
    }

    if (OMS_ABS(controller->button[CONTROLLER_BUTTON_STICK_LEFT_HORIZONTAL]) > input->deadzone) {
        input->state.dy2 += controller->button[CONTROLLER_BUTTON_STICK_LEFT_HORIZONTAL] / 8;
        // @todo needs state change flag like mouse?!
    } else {
        input->state.dy2 = 0;
    }

    // General Keys
    int32 count = 0;
    InputKey keys[5];

    for (uint16 i = 0; i < 32; ++i) {
        if ((controller->is_analog[i] && OMS_ABS(controller->button[i]) > input->deadzone)
            || (!controller->is_analog[i] && controller->button[i] != 0)
        ) {
            keys[count].key_id = i | INPUT_CONTROLLER_PREFIX;
            keys[count].key_state = KEY_STATE_PRESSED;
            keys[count].value = controller->button[i];
            keys[count].time = time;

            ++count;
        }
    }

    if (count > 0) {
        for (int32 i = 0; i < count; ++i) {
            input_set_state(&input->state, &keys[i]);
        }
    }

    input->state_change_button = true;
}

void
input_hotkey_state(Input* input)
{
    memset(input->state.state_hotkeys, 0, sizeof(uint8) * MAX_KEY_PRESSES);

    int32 active_hotkeys = 0;

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
        // @performance Could it make sense to only loop over one mapping (create a pointer that references the correct mapping)
        //      We then swap this pointer whenever we detect a input from keyboard+mouse vs controller
        //      This would allow us even to add context specific mappings
        for (int32 i = 0; i < 2; ++i) {
            InputMapping* mapping;
            if (i == 0) {
                mapping = &input->input_mapping1;
            } else if (input->handle_controller && key->key_id > INPUT_CONTROLLER_PREFIX) {
                mapping = &input->input_mapping2;
            } else {
                continue;
            }

            if (mapping->keys[(internal_key_id - 1) * MAX_KEY_TO_HOTKEY] == 0) {
                // no possible hotkey associated with this key
                continue;
            }

            const uint8* hotkeys_for_key = mapping->keys + (internal_key_id - 1) * MAX_KEY_TO_HOTKEY;

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

                    // Run callback if defined
                    if (input->input_mapping1.callbacks[hotkeys_for_key[possible_hotkey_idx]] != 0) {
                        input->input_mapping1.callbacks[hotkeys_for_key[possible_hotkey_idx]](input->callback_data);
                    }
                }
            }
        }
    }

    // @bug how to handle priority? e.g. there might be a hotkey for 1 and one for alt+1
    //      in this case only the hotkey for alt+1 should be triggered
    // @bug how to handle other conditions besides buttons pressed together? some hotkeys are only available in certain situations
    // @bug how to handle values (e.g. stick may or may not set the x/y or dx/dy in some situations)
    // @bug how to allow rebinding/swapping of left and right stick? (maybe create handful of events e.g. set dx/dy that fire based on the input?)
    // @bug There is a bug ONLY with the controller, when doing camera look around and holding the stick at and angle
    //          The hotkey seemingly loses activity after 1-2 sec if you then move the stick a little bit it works again
    //          It doesn't always happen but you can test it rather consistently within a couple of seconds
}

bool input_key_is_longpress(InputState* state, uint16 key, uint64 time, f32 dt = 0.0f) {
    for (int32 i = 0; i < MAX_KEY_STATES; ++i) {
        if (state->state_keys[i].key_id == key) {
            return (f32) (time - state->state_keys[i].time) / 1000.0f >= (dt == 0.0f ? INPUT_LONG_PRESS_DURATION : dt);
        }
    }

    return false;
}

// @todo I wrote this code at 9am after staying awake for the whole night and that is how that code looks like... fix it!
bool input_hotkey_is_longpress(Input* input, uint8 hotkey, uint64 time, f32 dt = 0.0f) {
    bool is_longpress = false;
    for (int32 i = 0; i < MAX_KEY_PRESSES; ++i) {
        if (input->state.state_hotkeys[i] != hotkey) {
            continue;
        }

        is_longpress = true;

        for (int32 j = 0; j < MAX_HOTKEY_COMBINATION; ++j) {
            bool potential_miss = true;
            bool both_empty = false;
            if (input->input_mapping1.hotkeys[hotkey * MAX_HOTKEY_COMBINATION + j] > 0) {
                if(!input_key_is_longpress(&input->state, input->input_mapping1.hotkeys[hotkey + j], time, dt)) {
                    potential_miss = true;
                } else {
                    potential_miss = false;
                }
            } else {
                both_empty = true;
            }

            if (!potential_miss) {
                continue;
            }

            if (input->input_mapping2.hotkeys[hotkey * MAX_HOTKEY_COMBINATION + j] > 0) {
                if(!input_key_is_longpress(&input->state, input->input_mapping2.hotkeys[hotkey + j], time, dt)) {
                    potential_miss = true;
                } else {
                    potential_miss = false;
                }
            } else {
                both_empty &= true;
            }

            if (both_empty) {
                continue;
            } else if (potential_miss) {
                return false;
            }
        }
    }

    return is_longpress;
}

#endif