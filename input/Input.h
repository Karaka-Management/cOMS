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

// @question Consider to change mouse to secondary input device and keyboard to primary input device and also rename the functions etc.

// How many concurrent mouse/secondary input device presses to we recognize
#define MAX_MOUSE_PRESSES 3

// How many concurrent primary key/button presses can be handled?
#define MAX_KEY_PRESSES 5

// How many keys/buttons do we support for the primary input device
#define MAX_KEYBOARD_KEYS 255

// How many mouse/secondary input device keys/buttons do we support
#define MAX_MOUSE_KEYS 5

#define MIN_INPUT_DEVICES 2

// How often can a key be asigned to a different hotkey
#define MAX_KEY_TO_HOTKEY 5

// How many buttons together are allowed to form a hotkey
#define MAX_HOTKEY_COMBINATION 3

// These values are used as bit flags to hint if a "key" is a keyboard/primary or mouse/secondary input
// When adding a keybind the "key" can only be uint8 but we expand it to an int and set the first bit accordingly
#define INPUT_KEYBOARD_PREFIX 80000000
#define INPUT_MOUSE_PREFIX 0

#define INPUT_TYPE_MOUSE_KEYBOARD 0x01
#define INPUT_TYPE_OTHER 0x03

#define MIN_CONTROLLER_DEVICES 4

#include "../stdlib/Types.h"
#include "../utils/BitUtils.h"

#ifdef _WIN32
    #include <windows.h>
#endif

// @todo I'm not sure if I like the general input handling
//      Having separate keyboard_down and mouse_down etc. is a little bit weird in the functions below

struct InputMapping {
    // A key/button can be bound to up to 5 different hotkeys
    // This is used to check if a key/button has a hotkey association
    uint8 keys[MAX_KEYBOARD_KEYS + MAX_MOUSE_KEYS][MAX_KEY_TO_HOTKEY];

    // A hotkey can be bound to a combination of up to 3 key/button presses
    uint8 hotkey_count;
    uint8* hotkeys;
};

// @question Maybe we should also add a third key_down array for controllers and some special controller functions here to just handle everything in one struct
//      Or think about completely splitting all states (mouse, keyboard, other)
struct InputState {
    // State of the hotkeys, resulting from the device input
    // @question maybe create a separate define and make it a little bit larger?
    uint8 state_hotkeys[MAX_KEY_PRESSES];

    uint8 keys_down[MAX_KEY_PRESSES];

    // @question Why do we even need this? shouldn't we only care about the current keys down?
    uint8 keys_up[MAX_KEY_PRESSES];

    uint32 mouse_down;

    int32 dx;
    int32 dy;

    uint32 x;
    uint32 y;

    int16 wheel_delta = 0;
    int16 hwheel_delta = 0;

    uint64 keys_down_time[MAX_MOUSE_PRESSES + MAX_KEY_PRESSES];
};

struct Input {
    // Device
    bool is_connected = false;
    byte type = INPUT_TYPE_OTHER;

    #ifdef _WIN32
        // @todo maybe replace with id?!
        //      -> remove _WIN32 section
        HANDLE handle_keyboard;
        HANDLE handle_mouse;
    #endif

    bool state_change_button = false;
    bool state_change_mouse = false;
    bool state_change_mouse_button = true;

    bool mouse_movement;

    InputState state;
    InputMapping input_mapping;

    // @todo we probably don't need this
    InputState state_old;
};

struct ControllerInput {
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

    // these are bitfields
    uint16 button_old;
    uint16 button;

    int16 stickl_x = 0;
    int16 stickl_y = 0;
    bool stickl_press = false;

    int16 stickr_x = 0;
    int16 stickr_y = 0;
    bool stickr_press = false;
};

inline
void mouse_backup_state(Input* input)
{
    input->state_old.mouse_down = input->state.mouse_down;

    input->state_old.x = input->state.x;
    input->state_old.y = input->state.y;

    input->state_old.wheel_delta = input->state.wheel_delta;
    input->state_old.hwheel_delta = input->state.wheel_delta;
}

inline
void keyboard_clean_state(InputState* state)
{
    memset(state->keys_down, 0, MAX_KEY_PRESSES * sizeof(uint8));
    memset(state->keys_up, 0, MAX_KEY_PRESSES * sizeof(uint8));
    memset(state->keys_down_time, 0, (MAX_MOUSE_PRESSES + MAX_KEY_PRESSES) * sizeof(uint64));
}

inline
void keyboard_backup_state(Input* input)
{
    memcpy(input->state_old.keys_down, input->state.keys_down, MAX_KEY_PRESSES * sizeof(uint8));
    memcpy(input->state_old.keys_up, input->state.keys_up, MAX_KEY_PRESSES * sizeof(uint8));
}

inline
bool keyboard_is_pressed(const InputState* state, byte key)
{
    return state->keys_down[0] == key
        || state->keys_down[1] == key
        || state->keys_down[2] == key
        || state->keys_down[3] == key
        || state->keys_down[4] == key;
}

inline
bool keyboard_is_released(const InputState* state, byte key)
{
    return state->keys_up[0] == key
        || state->keys_up[1] == key
        || state->keys_up[2] == key
        || state->keys_up[3] == key
        || state->keys_up[4] == key;
}

inline
bool keyboard_are_pressed(
    const InputState* state,
    byte key0, byte key1 = 0, byte key2 = 0, byte key3 = 0, byte key4 = 0
) {
    return (key0 != 0 && keyboard_is_pressed(state, key0))
        && (key1 == 0 || keyboard_is_pressed(state, key1))
        && (key2 == 0 || keyboard_is_pressed(state, key2))
        && (key3 == 0 || keyboard_is_pressed(state, key3))
        && (key4 == 0 || keyboard_is_pressed(state, key4));
}

// We are binding hotkeys bi-directional
void
input_add_hotkey(
    InputMapping* mapping, uint8 hotkey,
    uint32 key0, uint32 key1 = 0, uint32 key2 = 0
)
{
    int count = 0;

    int key0_offset = ((bool) (key0 & INPUT_KEYBOARD_PREFIX)) * MAX_MOUSE_KEYS;
    int key1_offset = ((bool) (key1 & INPUT_KEYBOARD_PREFIX)) * MAX_MOUSE_KEYS;
    int key2_offset = ((bool) (key2 & INPUT_KEYBOARD_PREFIX)) * MAX_MOUSE_KEYS;

    key0 = key0 & ~INPUT_KEYBOARD_PREFIX;
    key1 = key1 & ~INPUT_KEYBOARD_PREFIX;
    key2 = key2 & ~INPUT_KEYBOARD_PREFIX;

    // Define required keys for hotkey
    if (key0 != 0) {
        // Note: -1 since the hotkeys MUST start at 1 (0 is a special value for empty)
        mapping->hotkeys[(hotkey - 1) * MAX_HOTKEY_COMBINATION] = (uint8) (key0 + key0_offset);
        ++count;
    }

    if (key1 != 0) {
        // Note: -1 since the hotkeys MUST start at 1 (0 is a special value for empty)
        mapping->hotkeys[(hotkey - 1) * MAX_HOTKEY_COMBINATION + count] = (uint8) (key1 + key1_offset);
        ++count;
    }

    if (key2 != 0) {
        // Note: -1 since the hotkeys MUST start at 1 (0 is a special value for empty)
        mapping->hotkeys[(hotkey - 1) * MAX_HOTKEY_COMBINATION + count] = (uint8) (key2 + key2_offset);
    }

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
// @performance This seems like a much better simpler solution no?
//      However, it is probably a slower solution after calling this function many times?
//      Remember, we would call this function for almost every possible hotkey (depending on context) per frame
inline
bool hotkey_is_pressed(const InputState* __restrict state, const InputMapping* __restrict mapping, uint8 hotkey)
{
    uint8 key0 = mapping->hotkeys[(hotkey - 1) * MAX_HOTKEY_COMBINATION];
    uint8 key1 = mapping->hotkeys[(hotkey - 1) * MAX_HOTKEY_COMBINATION + 1];
    uint8 key2 = mapping->hotkeys[(hotkey - 1) * MAX_HOTKEY_COMBINATION + 2];

    bool is_pressed = false;
    if (key0 > MAX_MOUSE_KEYS) {
        key0 -= MAX_MOUSE_KEYS;
        is_pressed = keyboard_is_pressed(state, key0);
    } else if (key0 > 0) {
        is_pressed = IS_BIT_SET_R2L(state->mouse_down, key0 - 1);
    }

    if (!is_pressed || key1 == 0) {
        return is_pressed;
    }

    if (key1 > MAX_MOUSE_KEYS) {
        key1 -= MAX_MOUSE_KEYS;
        is_pressed &= keyboard_is_pressed(state, key1);
    } else if (key1 > 0) {
        is_pressed &= IS_BIT_SET_R2L(state->mouse_down, key1 - 1);
    }

    if (!is_pressed || key2 == 0) {
        return is_pressed;
    }

    if (key2 > MAX_MOUSE_KEYS) {
        key2 -= MAX_MOUSE_KEYS;
        is_pressed &= keyboard_is_pressed(state, key2);
    } else if (key2 > 0) {
        is_pressed &= IS_BIT_SET_R2L(state->mouse_down, key2 - 1);
    }

    return is_pressed;
}

void
input_hotkey_state(InputState* __restrict state, const InputMapping* mapping)
{
    // @bug isn't there a bug, MAX_KEY_PRESSES is the keyboard limit, what about additional mouse inputs?

    memset(state->state_hotkeys, 0, sizeof(uint8) * MAX_KEY_PRESSES);

    int i = 0;

    // @performance It would be nice if we could skip this loop by checking keyboard_changed similar to the mouse loop further down
    //      The problem is that this loop checks both mouse and keyboard

    // Check every key down state
    for (int down_state = 0; down_state < MAX_KEY_PRESSES; ++down_state) {
        if (state->keys_down[down_state] == 0) {
            // no key defined for this down state
            continue;
        }

        // Is a key defined for this state AND is at least one hotkey defined for this key
        //      If no hotkey is defined we don't care
        //      Careful, remember MAX_MOUSE_KEYS offset
        const uint8* hotkeys_for_key = mapping->keys[state->keys_down[down_state] + MAX_MOUSE_KEYS - 1];
        if (hotkeys_for_key[0] == 0) {
            // no possible hotkey associated with this key
            continue;
        }

        // Check every possible hotkey
        // Since multiple input devices have their own button/key indices whe have to do this weird range handling
        for (int possible_hotkey_idx = 0; possible_hotkey_idx < MAX_KEY_TO_HOTKEY; ++possible_hotkey_idx) {
            // We only support a slimited amount of active hotkeys
            if (i >= MAX_KEY_PRESSES) {
                return;
            }

            bool is_pressed = hotkey_is_pressed(state, mapping, hotkeys_for_key[possible_hotkey_idx]);

            // store active hotkey, if it is not already active
            if (is_pressed && !hotkey_is_active(state, hotkeys_for_key[possible_hotkey_idx])) {
                state->state_hotkeys[i] = hotkeys_for_key[possible_hotkey_idx];
                ++i;
            }
        }
    }

    // @performance we could also check if the mouse state even changed
    if (state->mouse_down == 0 || i >= MAX_KEY_PRESSES) {
        return;
    }

    // We now also need to check if there are hotkeys for the mouse buttons
    // Some are already handled in the previous section, but some might not be handled, since they are mouse only
    // But this also means, that we ONLY have to search for mouse only hotkeys. It's impossible to find NEW matches with keyboard keys.
    for (int down_state = 0; down_state < MAX_MOUSE_KEYS; ++down_state) {
        if (!IS_BIT_SET_R2L(state->mouse_down, down_state)) {
            continue;
        }

        const uint8* hotkeys_for_key = mapping->keys[down_state];
        if (hotkeys_for_key[0] == 0) {
            // no possible hotkey associated with this key
            continue;
        }

        for (int possible_hotkey_idx = 0; possible_hotkey_idx < MAX_KEY_TO_HOTKEY; ++possible_hotkey_idx) {
            // We only support a slimited amount of active hotkeys
            if (i >= MAX_KEY_PRESSES) {
                return;
            }

            bool is_pressed = hotkey_is_pressed(state, mapping, hotkeys_for_key[possible_hotkey_idx]);

            // store active hotkey, if it is not already active
            if (is_pressed && !hotkey_is_active(state, hotkeys_for_key[possible_hotkey_idx])) {
                state->state_hotkeys[i] = hotkeys_for_key[possible_hotkey_idx];
                ++i;
            }
        }
    }

    // @bug how to handle long press vs click
    // @bug how to handle priority? e.g. there might be a hotkey for 1 and one for alt+1
    //      in this case only the hotkey for alt+1 should be triggered
    // @bug how to handle other conditions besides buttons pressed together? some hotkeys are only available in certain situations
}

#endif