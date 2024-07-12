/**
 * Jingga
 *
 * @copyright Jingga
 * @license   OMS License 2.0
 * @version   1.0.0
 * @link      https://jingga.app
 */
#ifndef TOS_MODELS_MOB_C
#define TOS_MODELS_MOB_C

#include "../../utils/MathUtils.h"
#include "Mob.h"
#include "MobState.h"

void mob_interpolate(Mob* mob, f32 time)
{
    MobState *s1 = &mob->state1;
    MobState *s2 = &mob->state2;

    float t1 = s2->t - s1->t;
    float t2 = time - s2->t;

    t1 = OMS_MIN(t1, 1.0f);
    t1 = OMS_MAX(t1, 0.1f);

    float p = OMS_MIN(t2 / t1, 1.0f);

    mob->state.location.x = s1->location.x + (s2->location.x - s1->location.x) * p;
    mob->state.location.y = s1->location.y + (s2->location.y - s1->location.y) * p;
    mob->state.location.z = s1->location.z + (s2->location.z - s1->location.z) * p;

    mob->state.orientation.x = s1->orientation.x + (s2->orientation.x - s1->orientation.x) * p;
    mob->state.orientation.y = s1->orientation.y + (s2->orientation.y - s1->orientation.y) * p;
    mob->state.orientation.z = s1->orientation.z + (s2->orientation.z - s1->orientation.z) * p;
    mob->state.orientation.w = s1->orientation.w + (s2->orientation.w - s1->orientation.w) * p;
}

#endif