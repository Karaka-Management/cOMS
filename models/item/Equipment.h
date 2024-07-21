/**
 * Jingga
 *
 * @copyright Jingga
 * @license   OMS License 2.0
 * @version   1.0.0
 * @link      https://jingga.app
 */
#ifndef TOS_MODELS_EQUIPMENT_H
#define TOS_MODELS_EQUIPMENT_H

#include "../../stdlib/Types.h"

#include "../../mob/MobStats.h"
#include "../../mob/skill/StatsTarget.h"

struct Equipment {
    byte type;
    char* name;

    // @tood how to handle multiplicative stats?
    // you can have 2 stats for 2 target types (e.g. you could create a buff and debuff in one skill)
    SMobStats stats1;
    StatsTarget stats1_target;

    SMobStats stats2;
    StatsTarget stats2_target;

    // @todo probably add more of the Skill attributes here
    // @question what should be part of skills and what should be part of items?!?!?!?
};

#endif