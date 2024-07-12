/**
 * Jingga
 *
 * @copyright Jingga
 * @license   OMS License 2.0
 * @version   1.0.0
 * @link      https://jingga.app
 */
#ifndef TOS_MODELS_MOB_STATE_H
#define TOS_MODELS_MOB_STATE_H

#include "../../stdlib/Types.h"
#include "../../stdlib/Mathtypes.h"
#include "MobAction.h"

struct MobState {
    v3_f32 location;
    v4_f32 orientation;

    float t;

    // Action performed
    // first byte = action category
    // last 3 bytes = animation to use
    uint32 action = (MOB_ACTION_INACTIVE << 24);

    int chunk_id;
};

#endif