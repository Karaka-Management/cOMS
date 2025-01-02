/**
 * Jingga
 *
 * @copyright Jingga
 * @license   OMS License 2.0
 * @version   1.0.0
 * @link      https://jingga.app
 */
#ifndef TOS_ANIMATION_ENTITY_H
#define TOS_ANIMATION_ENTITY_H

#include "../stdlib/Types.h"
#include "../animation/AnimationEaseType.h"
#include "../animation/Animation.h"
#include "../utils/BitUtils.h"
#include "EntityComponentSystem.h"

struct AnimationEntity {
    AnimationEaseType type;
    uint32 start_time;
    uint32 last_time;
    f32 interval;
    f32 progress;
    byte state_last;
    byte state;

    // @question Do we want another flag that indicates if the entity got handled by the main loop?
    // this way we could do the animation process in a thread and only overwrite the state_last whenever the flag is true
    // However, we would have to implement locking or atomics which might be really bad depending on how we use this data
};

void update_animation_entity(AnimationEntity* anim, uint32 time, uint32 delay)
{
    anim->state_last = anim->state;

    switch (anim->type) {
        case ANIMATION_EASE_DISCRETE: {
                anim->progress = anim_discrete((f32) (time - anim->start_time + delay) / (f32) anim->interval);
                anim->state = (int32) ((f32) anim->state - anim->progress);
            } break;
        default: {}
    }
}

void update_animation_entities(EntityComponentSystem* ecs, uint32 time, uint32 delay)
{
    int32 chunk_bytes = (ecs->entity_data_memory.size + 63) / 64;

    // @performance It might make sense to iterate by int16 or even int32 instead of byte. Needs profiling
    for (int32 i = 0; i < chunk_bytes; ++i) {
        // @question Do we want this to be the first case. It probably depends on how often a byte is realistically empty
        if (!ecs->entity_data_memory.free[i]) {
            continue;
        } else if (ecs->entity_data_memory.free[i] == 256) {
            // @performance If we go larger than 8bit in the outer loop we also have to adjust it here
            // AND maybe we would want to do sub checks then for 8bit again
            for (int32 j = 0; j < 8; ++j) {
                AnimationEntity* anim = (AnimationEntity *) chunk_get_element(&ecs->entity_data_memory, i * 8 + j);
                update_animation_entity(anim, time, delay);
            }

            continue;
        }

        // @performance If we go larger than 8bit in the outer loop we also have to adjust it here
        // AND maybe we would want to do sub checks then for 8bit again
        for (int32 j = 0; j < 8; ++j) {
            if (!IS_BIT_SET_L2R(ecs->entity_data_memory.free[i], j, 1)) {
                continue;
            }

            AnimationEntity* anim = (AnimationEntity *) chunk_get_element(&ecs->entity_data_memory, i * 8 + j);
            update_animation_entity(anim, time, delay);
        }
    }
}

#endif