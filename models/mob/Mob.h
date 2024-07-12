/**
 * Jingga
 *
 * @copyright Jingga
 * @license   OMS License 2.0
 * @version   1.0.0
 * @link      https://jingga.app
 */
#ifndef TOS_MODELS_MOB_H
#define TOS_MODELS_MOB_H

#include "../../stdlib/Types.h"

#include "../settings/Settings.h"
#include "../item/Equipment.h"

#include "MobState.h"

struct Mob {
    byte category;
    byte mclass;

    byte level;
    byte rank;

    unsigned int id;
    unsigned int guild_id;

    MobState state;
    MobState state1;
    MobState state2;
};

#endif