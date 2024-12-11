/**
 * Jingga
 *
 * @copyright Jingga
 * @license   OMS License 2.0
 * @version   1.0.0
 * @link      https://jingga.app
 */
#ifndef TOS_SCENE_STATE_H
#define TOS_SCENE_STATE_H

#include "../stdlib/Types.h"

enum SceneState : byte {
    SCENE_STATE_DEFAULT = 0,
    SCENE_STATE_WINDOW_CHANGED = 1,
    SCENE_STATE_SHOULD_SWITCH = 2,
    SCENE_STATE_STARTED_SETUP = 4,
    SCENE_STATE_WAITING_SETUP = 8,
    SCENE_STATE_READY = 16,
};

#endif